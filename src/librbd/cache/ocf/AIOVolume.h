// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#ifndef CEPH_LIBRBD_CACHE_OCF_AIO_VOLUME
#define CEPH_LIBRBD_CACHE_OCF_AIO_VOLUME

extern "C" {
#include "ocf/ocf.h"
}

#include "Volume.h"

#include <libaio.h>
#include <thread>

namespace librbd {
namespace cache {
namespace ocf {

class Ctx;

class AIOVolume : public Volume {
public:
  AIOVolume(ocf_volume_t volume, void *volume_params);

  virtual uint64_t get_length() override;
  virtual void submit_io(struct ocf_io *io) override;

private:

  void poll();

  int m_fd;
  uint64_t m_length;
  io_context_t m_io_context;

  std::unique_ptr<std::thread> m_thread{};
};

} // namespace ocf
} // namespace cache
} // namespace librbd

#endif // CEPH_LIBRBD_CACHE_OCF_VOLUME
