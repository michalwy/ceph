// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#include "Ctx.h"
#include "AIOVolume.h"
#include "VolumeData.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <exception>
#include <libaio.h>
#include <iostream>
#include <iomanip>

namespace librbd {
namespace cache {
namespace ocf {

AIOVolume::AIOVolume(ocf_volume_t volume, void *volume_params) : Volume(volume, volume_params) {
	const struct ocf_volume_uuid *uuid = ocf_volume_get_uuid(volume);

  m_fd = open(ocf_uuid_to_str(uuid), O_RDWR);
  if (m_fd < 0) {
    throw std::exception();
  }

  off_t l = lseek(m_fd, 0, SEEK_END);
  if (l < 0) {
    throw std::exception();
  }

  m_length = l;

  l = lseek(m_fd, 0, SEEK_SET);
  if (l < 0) {
    throw std::exception();
  }

  memset(&m_io_context, 0, sizeof(m_io_context));
  if (io_setup(256, &m_io_context)) {
    throw std::exception();
  }

  m_thread.reset(new std::thread([this] {
    poll();
  }));
}

uint64_t AIOVolume::get_length() {
  return m_length;
}

void AIOVolume::submit_io(struct ocf_io *io) {
  struct iocb *iocb;
	VolumeData *data;

  //std::cout << __func__ << std::endl;

	data = (VolumeData *)ocf_io_get_data(io);

  iocb = new struct iocb;
  if (io->dir == OCF_READ) {
    //std::cout << "prep read" << std::endl;
    io_prep_pread(iocb, m_fd, data->get_data(), io->bytes, io->addr);
  } else if (io->dir == OCF_WRITE) {
    io_prep_pwrite(iocb, m_fd, data->get_data(), io->bytes, io->addr);
  } else {
    throw std::exception();
  }
  iocb->data = io;

  if (io_submit(m_io_context, 1, &iocb) != 1) {
    io->end(io, -1);
  }
}

void AIOVolume::poll() {
  struct io_event events[256];

  while (true) {
    int r = io_getevents(m_io_context, 1, 256, events, NULL);
    if (r < 0) {
      if (r == -EINTR) {
        continue;
      }
      break;
    }

    for (int i = 0; i < r; i++) {
      //std::cout << "completion" << std::endl;
      struct ocf_io *io = (struct ocf_io *)events[i].data;
      io->end(io, events[i].res < 0 ? events[i].res : 0);
      delete events[i].obj;
    }
  }
}

} // namespace ocf
} // namespace cache
} // namespace librbd

