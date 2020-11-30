// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab
#ifndef CEPH_LATENCYTRACKER_H
#define CEPH_LATENCYTRACKER_H

#include <chrono>
#include <string>
#include <vector>

class LatencyTracker {
public:
  LatencyTracker() {}

  void start();

  void checkpoint(const char *function, const char *file, unsigned int line);

  void stop(const std::string &out);

private:
  struct tracep {
    std::string file;
    std::string function;
    unsigned int line;
    unsigned long long tm;
  };

  unsigned long long _seq{0};

  std::chrono::time_point<std::chrono::high_resolution_clock> _start{};
  std::chrono::time_point<std::chrono::high_resolution_clock> _last{};

  std::vector<struct tracep> _tracepoints{};
};

extern LatencyTracker lat_tracker;

#define LT_CP() lat_tracker.checkpoint(__func__, __FILE__, __LINE__);

#endif
