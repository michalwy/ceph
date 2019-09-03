// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#include "Ctx.h"
#include "CachedVolumeCtx.h"
#include "CachedVolumeCtxPriv.h"
#include "CacheCtx.h"
#include "CacheCtxPriv.h"
#include "VolumeData.h"
#include "include/Context.h"

extern "C" {
#include "ocf/ocf.h"
}

namespace librbd {
namespace cache {
namespace ocf {

CachedVolumeCtx::CachedVolumeCtx(ImageCtx *image, CacheCtx::Queue *q) {
  m_priv = new CachedVolumeCtxPriv();
  m_image = image;
  m_queue = q;
}

CachedVolumeCtx::~CachedVolumeCtx() {
  delete m_priv;
}

static void ocf_submit_io_completion(struct ocf_io *io, int error) {
  ::Context *on_finish = (::Context *)io->priv1;
  //std::cout << __func__ << std::endl;
	VolumeData *data = (VolumeData *)ocf_io_get_data(io);
  on_finish->complete(error);
	ocf_io_put(io);
  delete data;
}

void CachedVolumeCtx::submit_io(uint32_t dir, uint64_t offset, uint64_t length, void *buffer, ::Context *on_finish) {
	struct ocf_io *io;

	io = ocf_core_new_io(m_priv->core);
	if (!io) {
    on_finish->complete(-ENOMEM);
    return;
  }

	ocf_io_configure(io, offset, length, dir, 0, 0);
	ocf_io_set_data(io, new VolumeData(buffer), 0);
	ocf_io_set_queue(io, m_queue->m_priv->queue);
	ocf_io_set_cmpl(io, on_finish, NULL, ocf_submit_io_completion);
	ocf_core_submit_io(io);
}

void CachedVolumeCtx::submit_write(uint64_t offset, uint64_t length, void *buffer, ::Context *on_finish) {
  submit_io(OCF_WRITE, offset, length, buffer, on_finish);
}

void CachedVolumeCtx::submit_read(uint64_t offset, uint64_t length, void *buffer, ::Context *on_finish) {
  submit_io(OCF_READ, offset, length, buffer, on_finish);
}

void CachedVolumeCtx::aio_read(Extents&& image_extents, ceph::bufferlist* bl,
                        int fadvise_flags, ::Context *on_finish) {
  /*
   * TODO: Handle multiple extends and buffers in bufferlist
   */
  //std::cout << __func__ << std::endl;
  if (bl->length() < image_extents[0].second) {
    size_t n = image_extents[0].second - bl->length();
    bl->append((const char *)malloc(n), n);
  }
  submit_read(image_extents[0].first, image_extents[0].second, bl->c_str(), on_finish);
}

void CachedVolumeCtx::aio_write(Extents&& image_extents, ceph::bufferlist&& bl,
                         int fadvise_flags, ::Context *on_finish) {

  /*
   * TODO: Handle multiple extends and buffers in bufferlist
   */
  //std::cout << __func__ << std::endl;
  submit_write(image_extents[0].first, image_extents[0].second, bl.c_str(), on_finish);
}

void CachedVolumeCtx::aio_discard(uint64_t offset, uint64_t length,
                           uint32_t discard_granularity_bytes,
                           ::Context *on_finish) {

                           }

void CachedVolumeCtx::aio_flush(::Context *on_finish) {
  //std::cout << __func__ << std::endl;
    on_finish->complete(0);
}

void CachedVolumeCtx::aio_writesame(uint64_t offset, uint64_t length,
                             ceph::bufferlist&& bl,
                             int fadvise_flags, ::Context *on_finish) {
  //std::cout << __func__ << std::endl;
    on_finish->complete(0);
                             }

void CachedVolumeCtx::aio_compare_and_write(Extents&& image_extents,
                                     ceph::bufferlist&& cmp_bl,
                                     ceph::bufferlist&& bl,
                                     uint64_t *mismatch_offset,
                                     int fadvise_flags,
                                     ::Context *on_finish) {
  //std::cout << __func__ << std::endl;
    on_finish->complete(0);
                                     }

  /// internal state methods
void CachedVolumeCtx::init(::Context *on_finish) {
  //std::cout << __func__ << std::endl;
    on_finish->complete(0);
}

void CachedVolumeCtx::shut_down(::Context *on_finish) {
  //std::cout << __func__ << std::endl;
    on_finish->complete(0);
}

void CachedVolumeCtx::invalidate(::Context *on_finish) {
  //std::cout << __func__ << std::endl;
    on_finish->complete(0);
}

void CachedVolumeCtx::flush(::Context *on_finish) {
  //std::cout << __func__ << std::endl;
    on_finish->complete(0);
}

} // namespace ocf
} // namespace cache
} // namespace librbd

