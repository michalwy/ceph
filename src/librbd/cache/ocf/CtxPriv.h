// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#ifndef CEPH_LIBRBD_CACHE_OCF_CTX_PRIV
#define CEPH_LIBRBD_CACHE_OCF_CTX_PRIV

#include <vector>

extern "C" {
#include "ocf/ocf.h"
}

#include "VolumeType.h"

namespace librbd {
namespace cache {
namespace ocf {

struct CtxPriv {
  ocf_ctx_t ocf_ctx;
};

} // namespace ocf
} // namespace cache
} // namespace librbd

#endif // CEPH_LIBRBD_CACHE_OCF_CTX_PRIV


