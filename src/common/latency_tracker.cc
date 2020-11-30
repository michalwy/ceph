#include "latency_tracker.h"
#include <fstream>

LatencyTracker lat_tracker;

void LatencyTracker::start() {
  _start = _last = std::chrono::high_resolution_clock::now();
}

void LatencyTracker::checkpoint(const char *function, const char *file,
                                unsigned int line) {
  auto t = std::chrono::high_resolution_clock::now();
  struct tracep tp;

  tp.function = function;
  tp.file = file;
  tp.line = line;
  tp.tm =
      std::chrono::duration_cast<std::chrono::nanoseconds>(t - _last).count();
  _tracepoints.push_back(tp);

  _last = std::chrono::high_resolution_clock::now();
}

void LatencyTracker::stop(const std::string &out) {
  std::ofstream ofs;

  ofs.open(out, std::ofstream::out | std::ofstream::app);

  unsigned long long total = 0;
  unsigned int step = 0;
  for (auto tp : _tracepoints) {
    ofs << _seq << ";";
    ofs << step << ";";
    ofs << tp.function << ";";
    ofs << tp.file << ";";
    ofs << tp.line << ";";
    ofs << tp.tm << ";";
    total += tp.tm;
    ofs << total << std::endl;
    step++;
  }

  _seq++;
  _tracepoints.clear();

  ofs.close();
}
