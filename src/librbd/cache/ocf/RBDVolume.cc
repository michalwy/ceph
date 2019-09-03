// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#include "Ctx.h"
#include "RBDVolume.h"
#include "VolumeData.h"
#include "librbd/ImageCtx.h"
#include "librbd/io/AioCompletion.h"
#include "librbd/io/ImageRequest.h"
#include "librbd/io/ReadResult.h"
#include "librbd/internal.h"
#include "include/Context.h"

namespace librbd {
namespace cache {
namespace ocf {

RBDVolume::RBDVolume(ocf_volume_t volume, void *volume_params) : Volume(volume, volume_params) {
	const struct ocf_volume_uuid *uuid = ocf_volume_get_uuid(volume);
  m_image = (ImageCtx *)uuid->runtime_data;
}

uint64_t RBDVolume::get_length() {
  std::uint64_t s;
  librbd::get_size(m_image, &s);
  return s;
}

void RBDVolume::submit_io(struct ocf_io *io) {
  if (io->dir == OCF_WRITE) {
    submit_write(io);
  } else if (io->dir == OCF_READ) {
    submit_read(io);
  }
}

class RBDVolumeComplContext : public ::Context {
public:
  RBDVolumeComplContext(struct ocf_io *io) {
    m_io = io;
  }

  void finish(int err) override {
    m_io->end(m_io, err < 0 ? err : 0);
  }

private:
  struct ocf_io *m_io;
};

void RBDVolume::submit_write(struct ocf_io *io) {
	VolumeData *data;

	data = (VolumeData *)ocf_io_get_data(io);

  ::Context *c = new RBDVolumeComplContext(io);
  auto aio_comp = io::AioCompletion::create_and_start(c, m_image, io::AIO_TYPE_WRITE);

  ceph::bufferlist bl;
  bl.append((const char *)data->get_data() + data->get_offset(), io->bytes);

  io::ImageWriteRequest<ImageCtx> req(*m_image, aio_comp, {{io->addr, io->bytes}},
                               std::move(bl), 0, {});

  req.set_bypass_image_cache();
  req.send();
}

void RBDVolume::submit_read(struct ocf_io *io) {
	VolumeData *data;

	data = (VolumeData *)ocf_io_get_data(io);

  ::Context *c = new RBDVolumeComplContext(io);
  auto aio_comp = io::AioCompletion::create_and_start(c, m_image, io::AIO_TYPE_READ);

  ceph::bufferlist *bl = new ceph::bufferlist();
  bl->append((const char *)data->get_data() + data->get_offset(), io->bytes);
  io::ImageReadRequest<ImageCtx> req(*m_image, aio_comp, {{io->addr, io->bytes}},
                              io::ReadResult{bl}, 0, {});
  req.set_bypass_image_cache();
  req.send();
}

} // namespace ocf
} // namespace cache
} // namespace librbd

