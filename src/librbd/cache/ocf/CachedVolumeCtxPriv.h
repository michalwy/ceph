// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#ifndef CEPH_LIBRBD_CACHE_OCF_CACHED_VOLUME_CTX_PRIV
#define CEPH_LIBRBD_CACHE_OCF_CACHED_VOLUME_CTX_PRIV

extern "C" {
#include "ocf/ocf.h"
}

namespace librbd {
namespace cache {
namespace ocf {

struct CachedVolumeCtxPriv {
  ocf_core_t core;
};

} // namespace ocf
} // namespace cache
} // namespace librbd

#endif // CEPH_LIBRBD_CACHE_OCF_CACHED_VOLUME_CTX_PRIV


