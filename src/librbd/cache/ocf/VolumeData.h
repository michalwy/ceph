// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#ifndef CEPH_LIBRBD_CACHE_OCF_VOLUME_DATA
#define CEPH_LIBRBD_CACHE_OCF_VOLUME_DATA

#include <string>

extern "C" {
#include "ocf/ocf.h"
}

#include "Volume.h"

namespace librbd {
namespace cache {
namespace ocf {

class VolumeData {
public:
  VolumeData(void *buffer) {
    m_own_buffer = false;
    m_data = buffer;
    m_offset = 0;
  }

  VolumeData(uint32_t pages) : VolumeData(malloc(pages * 4096)) {
    m_own_buffer = true;
  }

  ~VolumeData() {
    if (m_own_buffer) {
      free(m_data);
    }
  }

  void *get_data() const {
    return m_data;
  }

  uint32_t get_offset() const {
    return m_offset;
  }

  uint32_t read(void *d, uint32_t sz) {
    memcpy(d, m_data + m_offset, sz);
    return sz;
  }

  uint32_t write(const void *s, uint32_t sz) {
    memcpy(m_data + m_offset, s, sz);
    return sz;
  }

  uint32_t seek(ctx_data_seek_t seek, uint32_t offset) {
    switch (seek) {
    case ctx_data_seek_begin:
      m_offset = offset;
      break;
    case ctx_data_seek_current:
      m_offset += offset;
      break;
    }

    return offset;
  }

  uint32_t zero(uint32_t sz) {
    memset(m_data + m_offset, 0, sz);
    return sz;
  }

  uint32_t copy_from(uint64_t d_offset, VolumeData *src, uint64_t s_offset, uint64_t sz) {
    memcpy(m_data + d_offset, src->m_data + s_offset, sz);
    return sz;
  }

private:
  bool m_own_buffer;
  void *m_data;
  uint32_t m_offset;
};

} // namespace ocf
} // namespace cache
} // namespace librbd

#endif // CEPH_LIBRBD_CACHE_OCF_VOLUME_TYPE
