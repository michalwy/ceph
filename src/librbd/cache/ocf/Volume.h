// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#ifndef CEPH_LIBRBD_CACHE_OCF_VOLUME
#define CEPH_LIBRBD_CACHE_OCF_VOLUME

extern "C" {
#include "ocf/ocf.h"
}

namespace librbd {
namespace cache {
namespace ocf {

class Ctx;

class Volume {
public:
  Volume(ocf_volume_t volume, void *volume_params);

  virtual ~Volume();

  virtual uint64_t get_length() = 0;
  virtual void submit_io(struct ocf_io *io) = 0;
};

} // namespace ocf
} // namespace cache
} // namespace librbd

#endif // CEPH_LIBRBD_CACHE_OCF_VOLUME
