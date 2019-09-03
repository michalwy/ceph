// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#include "Ctx.h"
#include "CtxPriv.h"
#include "VolumeType.h"
#include "AIOVolume.h"
#include "RBDVolume.h"
#include "VolumeData.h"

namespace librbd {
namespace cache {
namespace ocf {

static int ocf_ctx_logger_print(ocf_logger_t logger, ocf_logger_lvl_t lvl, const char *fmt, va_list args) {
	return vfprintf(stdout, fmt, args);
}

static int ocf_ctx_metadata_updater_init(ocf_metadata_updater_t mu) {
  return 0;
}

static void ocf_ctx_metadata_updater_kick(ocf_metadata_updater_t mu) {
}

static void ocf_ctx_metadata_updater_stop(ocf_metadata_updater_t mu) {
}

static int ocf_ctx_cleaner_init(ocf_cleaner_t c)
{
	return 0;
}

static void ocf_ctx_cleaner_kick(ocf_cleaner_t c)
{
}

static void ocf_ctx_cleaner_stop(ocf_cleaner_t c)
{
}

static ctx_data_t *ocf_ctx_data_alloc(uint32_t pages)
{
  return new VolumeData(pages);
}

static void ocf_ctx_data_free(ctx_data_t *ctx_data)
{
  VolumeData *p = (VolumeData *)ctx_data;

	if (!p)
		return;

  delete p;
}

static int ocf_ctx_data_mlock(ctx_data_t *ctx_data)
{
	return 0;
}

static void ocf_ctx_data_munlock(ctx_data_t *ctx_data)
{
}

static uint32_t ocf_ctx_data_read(void *dst, ctx_data_t *src, uint32_t size)
{
	VolumeData *p = (VolumeData *)src;
  return p->read(dst, size);
}

static uint32_t ocf_ctx_data_write(ctx_data_t *dst, const void *src, uint32_t size)
{
	VolumeData *p = (VolumeData *)dst;
  return p->write(src, size);
}

static uint32_t ocf_ctx_data_zero(ctx_data_t *dst, uint32_t size)
{
	VolumeData *p = (VolumeData *)dst;
  return p->zero(size);
}

static uint32_t ocf_ctx_data_seek(ctx_data_t *dst, ctx_data_seek_t seek,
		uint32_t offset)
{
	VolumeData *p = (VolumeData *)dst;
  return p->seek(seek, offset);
}

static uint64_t ocf_ctx_data_copy(ctx_data_t *dst, ctx_data_t *src,
		uint64_t to, uint64_t from, uint64_t bytes)
{
  VolumeData *d = (VolumeData *)dst;
  VolumeData *s = (VolumeData *)src;

  return d->copy_from(to, s, from, bytes);
}

static void ocf_ctx_data_secure_erase(ctx_data_t *ctx_data)
{
}


static const struct ocf_ctx_config ocf_ctx_cfg = {
  .name = "OCF LIBRBD",

  .ops = {
    .data = {
      .alloc = ocf_ctx_data_alloc,
      .free = ocf_ctx_data_free,
      .mlock = ocf_ctx_data_mlock,
      .munlock = ocf_ctx_data_munlock,
      .read = ocf_ctx_data_read,
      .write = ocf_ctx_data_write,
      .zero = ocf_ctx_data_zero,
      .seek = ocf_ctx_data_seek,
      .copy = ocf_ctx_data_copy,
      .secure_erase = ocf_ctx_data_secure_erase,
    },

    .cleaner = {
      .init = ocf_ctx_cleaner_init,
      .kick = ocf_ctx_cleaner_kick,
      .stop = ocf_ctx_cleaner_stop,
    },

    .metadata_updater = {
      .init = ocf_ctx_metadata_updater_init,
      .kick = ocf_ctx_metadata_updater_kick,
      .stop = ocf_ctx_metadata_updater_stop,
    },

    .logger = {
      .print = ocf_ctx_logger_print,
      .dump_stack = NULL,
    },
  },
};

Ctx::Ctx()
{
  m_priv = new CtxPriv;
  ocf_ctx_create(&m_priv->ocf_ctx, &ocf_ctx_cfg);
  ocf_ctx_set_priv(m_priv->ocf_ctx, this);

  new VolumeType<AIOVolume, VolumeIO>(this, "AIO volume", 1);
  new VolumeType<RBDVolume, VolumeIO>(this, "RBD volume", 2);
}

Ctx::~Ctx()
{
  ocf_ctx_put(m_priv->ocf_ctx);
}

} // namespace ocf
} // namespace cache
} // namespace librbd

