//
// Created by yalavrinenko on 02.12.2020.
//
#include "main.hpp"
#include <sstream>
#include <numeric>

struct AOC_Output {
  long long value;
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::string entry;
  explicit AOC_Input(std::string s) : entry{std::move(s)} {}
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) { return AOC_Input(line); }

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto target_time = std::stol(v[0].entry);
  std::istringstream iss{v[1].entry};

  auto min_diff = target_time;
  auto bus = -1;
  std::string s;
  while (std::getline(iss, s, ',')) {
    if (s != "x") {
      auto time = std::stol(s);
      auto diff = ((target_time % time != 0) ? target_time / time + 1 : target_time / time) * time - target_time;
      if (diff < min_diff) {
        min_diff = diff;
        bus = time;
      }
    }
  }

  return {bus * min_diff};
}

long long gcdex(long long a, long long b, long long &x, long long &y) {
  if (a == 0) {
    x = 0;
    y = 1;
    return b;
  }
  long long x1, y1;
  long long d = gcdex(b % a, a, x1, y1);
  x = y1 - (b / a) * x1;
  y = x1;
  return d;
}

long long reverse(long long a, long long m) {
  long long x, y;
  [[maybe_unused]] long long g = gcdex(a, m, x, y);
  return (x % m + m) % m;
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  std::string timeline;
  std::istringstream iss{v[1].entry};
  std::vector<long long> r, m;
  auto index = 0;
  while (std::getline(iss, timeline, ',')) {
    if (timeline != "x") {
      auto time = std::stoll(timeline);
      m.emplace_back(time);
      r.emplace_back(index % time);
    }
    ++index;
  }

  auto M = std::accumulate(m.begin(), m.end(), 1ll, std::multiplies<long long>());
  auto X = 0ll;
  for (auto i = 0u; i < m.size(); ++i){
    auto y = M / m[i];
    auto si = reverse(y, m[i]);
    auto ci = (r[i] * si) % m[i];
    X = (X + ci * y) % M;
  }

  return {M - X};
}

#include "exec.hpp"