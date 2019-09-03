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

template<typename T>
static int volume_open(ocf_volume_t volume, void *volume_params) {
  T* vol = reinterpret_cast<T *>(ocf_volume_get_priv(volume));

  try {
    new(vol) T(volume, volume_params);
    return 0;
  } catch (...) {
    return -EIO;
  }
}

template<typename T>
static void volume_close(ocf_volume_t volume) {
  T* vol = reinterpret_cast<T *>(ocf_volume_get_priv(volume));
  vol->~T();
}

template<typename T>
static void volume_submit_io(struct ocf_io *io) {
  T* vol = reinterpret_cast<T *>(ocf_volume_get_priv(io->volume));
  return vol->submit_io(io);
}

template<typename T>
static unsigned int volume_get_max_io_size(ocf_volume_t volume) {
  throw std::exception();
}

template<typename T>
static uint64_t volume_get_length(ocf_volume_t volume) {
  T* vol = reinterpret_cast<T *>(ocf_volume_get_priv(volume));
  return vol->get_length();
}

template<typename IO>
static int volume_io_set_data(struct ocf_io *io, ctx_data_t *data,
		uint32_t offset)
{
	IO *p = (IO *)ocf_io_get_priv(io);
  return p->set_data((VolumeData *)data, offset);
}

template<typename IO>
static ctx_data_t *volume_io_get_data(struct ocf_io *io)
{
	IO *p = (IO *)ocf_io_get_priv(io);
  return p->get_data();
}

template<typename T, typename IO>
VolumeType<T, IO>::VolumeType(Ctx *ctx, const std::string& name, int id) : 
  m_properties({}),
  m_name(name),
  m_id(id) {

  m_properties.name = m_name.c_str();
  m_properties.caps.atomic_writes = 0;
  m_properties.volume_priv_size = sizeof(T);
  m_properties.io_priv_size = sizeof(IO);

  m_properties.ops.open = &volume_open<T>;
  m_properties.ops.close = &volume_close<T>;
  m_properties.ops.submit_io = &volume_submit_io<T>;
  m_properties.ops.get_max_io_size = &volume_get_max_io_size<T>;
  m_properties.ops.get_length = &volume_get_length<T>;

  m_properties.io_ops.set_data = &volume_io_set_data<IO>;
  m_properties.io_ops.get_data = &volume_io_get_data<IO>;

  ocf_ctx_register_volume_type(ctx->m_priv->ocf_ctx, m_id, &m_properties);
}

template class VolumeType<AIOVolume, VolumeIO>;
template class VolumeType<RBDVolume, VolumeIO>;

} // namespace ocf
} // namespace cache
} // namespace librbd

