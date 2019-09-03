// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#ifndef CEPH_LIBRBD_CACHE_OCF_CACHED_VOLUME_CTX
#define CEPH_LIBRBD_CACHE_OCF_CACHED_VOLUME_CTX

#include "../ImageCache.h"
#include "CacheCtx.h"
#include <string>

namespace librbd {

struct ImageCtx;

namespace cache {
namespace ocf {

struct CachedVolumeCtxPriv;

class CachedVolumeCtx : public ImageCache {
public:
  CachedVolumeCtx(ImageCtx *image, CacheCtx::Queue *q);
  ~CachedVolumeCtx();

  virtual void aio_read(Extents&& image_extents, ceph::bufferlist* bl,
                        int fadvise_flags, ::Context *on_finish);
  virtual void aio_write(Extents&& image_extents, ceph::bufferlist&& bl,
                         int fadvise_flags, ::Context *on_finish);
  virtual void aio_discard(uint64_t offset, uint64_t length,
                           uint32_t discard_granularity_bytes,
                           ::Context *on_finish);
  virtual void aio_flush(::Context *on_finish);
  virtual void aio_writesame(uint64_t offset, uint64_t length,
                             ceph::bufferlist&& bl,
                             int fadvise_flags, ::Context *on_finish);
  virtual void aio_compare_and_write(Extents&& image_extents,
                                     ceph::bufferlist&& cmp_bl,
                                     ceph::bufferlist&& bl,
                                     uint64_t *mismatch_offset,
                                     int fadvise_flags,
                                     ::Context *on_finish);

  /// internal state methods
  virtual void init(::Context *on_finish);
  virtual void shut_down(::Context *on_finish);

  virtual void invalidate(::Context *on_finish);
  virtual void flush(::Context *on_finish);

private:

  void submit_io(uint32_t dir, uint64_t offset, uint64_t length, void *buffer, ::Context *on_finish);
  void submit_read(uint64_t offset, uint64_t length, void *buffer, ::Context *on_finish);
  void submit_write(uint64_t offset, uint64_t length, void *buffer, ::Context *on_finish);

  struct CachedVolumeCtxPriv *m_priv;
  ImageCtx *m_image;
  CacheCtx::Queue *m_queue;

  friend class CacheCtx;
};

} // namespace ocf
} // namespace cache
} // namespace librbd

#endif // CEPH_LIBRBD_CACHE_OCF_CACHED_VOLUME_CTX
