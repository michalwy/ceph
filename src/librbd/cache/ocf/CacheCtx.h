// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#ifndef CEPH_LIBRBD_CACHE_OCF_CACHE_CTX
#define CEPH_LIBRBD_CACHE_OCF_CACHE_CTX

#include <string>

#include "librbd/io/AioCompletion.h"
#include "common/WorkQueue.h"

namespace librbd {
namespace cache {
namespace ocf {

class Ctx;
class CachedVolumeCtx;
class QueuePriv;
class CacheCtxPriv;

class CacheCtx {
public:

  CacheCtx(Ctx *context, IoCtx& io_ctx);
  ~CacheCtx();

  class Queue {
  public:
    Queue(CacheCtx *cache);
    ~Queue();

    void kick();

  private:
    CacheCtx *m_cctx;
    ContextWQ *m_wq;
    QueuePriv *m_priv;

    friend class CacheCtx;
    friend class CachedVolumeCtx;
  };

  int start();
  int attach(const std::string& file, ::Context *c);

  CachedVolumeCtx *add_core(ImageCtx *image, ::Context *c);

  struct ocf_add_core_ctx;


  void finish_add_core(struct ocf_add_core_ctx *ctx);

  void run_queue(Queue *q);

private:
  Ctx *m_ctx;
  CephContext *m_cct;
  CacheCtxPriv *m_priv;

  friend class Queue;
};

} // namespace ocf
} // namespace cache
} // namespace librbd

#endif // CEPH_LIBRBD_CACHE_OCF_CACHE_CTX
