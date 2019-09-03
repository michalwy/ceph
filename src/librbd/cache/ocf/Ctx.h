// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#ifndef CEPH_LIBRBD_CACHE_OCF_CTX
#define CEPH_LIBRBD_CACHE_OCF_CTX

namespace librbd {
namespace cache {
namespace ocf {

struct CtxPriv;
class Cache;

struct Ctx {
  struct CtxPriv *m_priv;

  Ctx();
  ~Ctx();
};

} // namespace ocf
} // namespace cache
} // namespace librbd

#endif // CEPH_LIBRBD_CACHE_OCF_CTX
