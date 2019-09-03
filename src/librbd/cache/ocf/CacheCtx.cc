// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#include "Ctx.h"
#include "CacheCtx.h"
#include "CacheCtxPriv.h"
#include "Ctx.h"
#include "CtxPriv.h"
#include "CachedVolumeCtx.h"
#include "CachedVolumeCtxPriv.h"

extern "C" {
#include "ocf/ocf.h"
}

namespace librbd {
namespace cache {
namespace ocf {

static void queue_kick(ocf_queue_t q) {
  CacheCtx::Queue *p = (CacheCtx::Queue *)ocf_queue_get_priv(q);
  p->kick();
}

static void queue_stop(ocf_queue_t q) {
  CacheCtx::Queue *p = (CacheCtx::Queue *)ocf_queue_get_priv(q);
  delete p;
}

const struct ocf_queue_ops queue_ops = {
	.kick = queue_kick,
	.kick_sync = NULL,
	.stop = queue_stop
};

class QueueKickContext : public ::Context {
public:
  QueueKickContext(CacheCtx *cctx, CacheCtx::Queue *q) {
    m_cctx = cctx;
    m_q = q;
  }
protected:
  virtual void finish(int r) {
    m_cctx->run_queue(m_q);
  }

private:
  CacheCtx *m_cctx;
  CacheCtx::Queue *m_q;
};

CacheCtx::Queue::Queue(CacheCtx *cache) {
  int ret;

  m_cctx = cache;

  m_priv = new QueuePriv;
  ret = ocf_queue_create(cache->m_priv->cache, &m_priv->queue, &queue_ops);
  if (ret)
    throw std::exception();
  
  ocf_queue_set_priv(m_priv->queue, this);

  ThreadPool *thread_pool;
  ContextWQ *op_work_queue;
  ImageCtx::get_thread_pool_instance(cache->m_cct, &thread_pool, &op_work_queue);

  m_wq = new ContextWQ("OCF queue", 0, thread_pool);
}

CacheCtx::Queue::~Queue() {
  m_wq->drain();
  delete m_wq;
}

void CacheCtx::Queue::kick() {
  QueueKickContext *c = new QueueKickContext(m_cctx, this);
  m_wq->queue(c);
}

CacheCtx::CacheCtx(Ctx *context, IoCtx& io_ctx) {
  m_priv = new CacheCtxPriv;
  m_ctx = context;
  m_cct = (CephContext *)io_ctx.cct();
}

CacheCtx::~CacheCtx() {

}

int CacheCtx::start() {
  struct ocf_mngt_cache_config cache_cfg = { };
  struct ocf_mngt_cache_device_config device_cfg = { };
  int ret;
  char name[] = "cache";

  ocf_mngt_cache_config_set_default(&cache_cfg);
  cache_cfg.metadata_volatile = true;
  cache_cfg.name = "cache1";
  cache_cfg.cache_mode = ocf_cache_mode_wt;

  ocf_mngt_cache_device_config_set_default(&device_cfg);
	device_cfg.volume_type = 1;
	ret = ocf_uuid_set_str(&device_cfg.uuid, name);
  if (ret)
    return ret;
  
  ret = ocf_mngt_cache_start(m_ctx->m_priv->ocf_ctx, &m_priv->cache, &cache_cfg);
  if (ret)
    return ret;

  ocf_cache_set_priv(m_priv->cache, this);

  Queue *q = new Queue(this);
  ocf_mngt_cache_set_mngt_queue(m_priv->cache, q->m_priv->queue);

  return 0;
}

static void ocf_cache_attach_complete(ocf_cache_t cache, void *priv, int error) {
  ::Context *c = (::Context *)priv;
  c->complete(error);
}

int CacheCtx::attach(const std::string& file, ::Context *c) {
	struct ocf_mngt_cache_device_config device_cfg = { };
  int ret;

	ocf_mngt_cache_device_config_set_default(&device_cfg);
	device_cfg.volume_type = 1;
	ret = ocf_uuid_set_str(&device_cfg.uuid, strdup(file.c_str()));
	if (ret)
		return ret;

  ocf_mngt_cache_attach(m_priv->cache, &device_cfg, ocf_cache_attach_complete, c);

  return 0;
}

void CacheCtx::run_queue(CacheCtx::Queue *q) {
  ocf_queue_run(q->m_priv->queue);
}

struct CacheCtx::ocf_add_core_ctx {
  CacheCtx *cache;
  CachedVolumeCtx *cv_ctx;
  ocf_core_t core;
  ImageCtx *image;
  ::Context *c;
  int result;
};

static void ocf_add_core_complete(ocf_cache_t cache, ocf_core_t core, void *priv, int error) {
  struct CacheCtx::ocf_add_core_ctx *c = (struct CacheCtx::ocf_add_core_ctx *)priv;
  c->core = core;
  c->result = error;
  c->cache->finish_add_core(c);
  c->c->complete(error);
  delete c;
}

CachedVolumeCtx *CacheCtx::add_core(ImageCtx *image, ::Context *c) {
	struct ocf_mngt_core_config core_cfg = { };
  CachedVolumeCtx *cv_ctx;
	int ret;
  struct ocf_add_core_ctx *ctx;

	/* Core configuration */
	ocf_mngt_core_config_set_default(&core_cfg);
	core_cfg.name = "core";
	core_cfg.volume_type = 2;
	ret = ocf_uuid_set_str(&core_cfg.uuid, "core");
  core_cfg.uuid.runtime_data = image;
	if (ret) {
		return NULL;
  }

  Queue *q = new Queue(this);

  cv_ctx = new CachedVolumeCtx(image, q);
  ctx = new struct ocf_add_core_ctx;
  ctx->cv_ctx = cv_ctx;
  ctx->c = c;
  ctx->cache = this;
  ctx->image = image;

	/* Add core to cache */
	ocf_mngt_cache_add_core(m_priv->cache, &core_cfg, ocf_add_core_complete, ctx);

	return cv_ctx;;
}

void CacheCtx::finish_add_core(struct CacheCtx::ocf_add_core_ctx *ctx) {
  ctx->cv_ctx->m_priv->core = ctx->core;
  ctx->image->image_cache = ctx->cv_ctx;
}

} // namespace ocf
} // namespace cache
} // namespace librbd

