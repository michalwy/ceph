// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#ifndef CEPH_LIBRBD_CACHE_OCF_VOLUME_TYPE
#define CEPH_LIBRBD_CACHE_OCF_VOLUME_TYPE

#include <string>

extern "C" {
#include "ocf/ocf.h"
}

#include "Volume.h"

namespace librbd {
namespace cache {
namespace ocf {

class Ctx;
class VolumeData;

class VolumeIO {
public:
  int set_data(VolumeData *d, uint32_t o) {
    m_data = d;
    m_offset = o;
    return 0;
  }

  VolumeData *get_data() const {
    return m_data;
  }
private:
  VolumeData *m_data;
  uint32_t m_offset;
};

template<typename T, typename IO>
class VolumeType {
public:
  VolumeType(Ctx *ctx, const std::string& name, int id);

  const std::string& get_name() const { return m_name; }
  int get_id() const { return m_id; }


private:
  struct ocf_volume_properties m_properties;
  std::string m_name;
  int m_id;
};

} // namespace ocf
} // namespace cache
} // namespace librbd

#endif // CEPH_LIBRBD_CACHE_OCF_VOLUME_TYPE
