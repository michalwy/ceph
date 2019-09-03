// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#ifndef CEPH_LIBRBD_CACHE_OCF_RBD_VOLUME
#define CEPH_LIBRBD_CACHE_OCF_RBD_VOLUME

extern "C" {
#include "ocf/ocf.h"
}

#include "Volume.h"

#include <libaio.h>
#include <thread>

namespace librbd {

struct ImageCtx;

namespace cache {
namespace ocf {

class Ctx;

class RBDVolume : public Volume {
public:
  RBDVolume(ocf_volume_t volume, void *volume_params);

  virtual uint64_t get_length() override;
  virtual void submit_io(struct ocf_io *io) override;

private:

  void submit_write(struct ocf_io *io);
  void submit_read(struct ocf_io *io);

  ImageCtx *m_image;

};

} // namespace ocf
} // namespace cache
} // namespace librbd

#endif // CEPH_LIBRBD_CACHE_OCF_RBD_VOLUME
