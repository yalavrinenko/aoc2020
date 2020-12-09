//
// Created by yalavrinenko on 02.12.2020.
//
#include "main.hpp"
#include <array>

struct AOC_Output{
  size_t value;
  auto operator() () const { return value; }
};

struct AOC_Input{
  size_t v;
  explicit AOC_Input(std::string const &s): v{std::stoul(s)}{
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

template<size_t WindowSize>
class MovingWindow{
public:
  void add(size_t value){
    data_[it_] = value;
    it_ = (it_ + 1) % WindowSize;
  }

  [[nodiscard]] bool find_pair(size_t sum) const {
    for (auto i = 0u; i < WindowSize - 1; ++i)
      for (auto j = i + 1; j < WindowSize; ++j)
        if (data_[i] + data_[j] == sum)
          return true;
    return false;
  }
private:
  std::array<size_t, WindowSize> data_{0};
  size_t it_{0};
};

AOC_Output part_1(std::vector<AOC_Input> const &v){
  auto const skip_value = 25;
  MovingWindow<skip_value> w;
  auto skip_it = skip_value;
  for (auto const &in : v){
    if (skip_it > 0){
      w.add(in.v);
      --skip_it;
    } else {
      if (!w.find_pair(in.v))
        return {in.v};
      else
        w.add(in.v);
    }
  }
  return {0};
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  auto target = part_1(v)();
  auto rbegin = v.begin();
  auto rend = v.begin();
  auto sum = rbegin->v;
  while (sum != target){
    if (sum < target) {
      ++rend; sum += (*rend).v;
    } else {
      sum -= rbegin->v; ++rbegin;
    }
  }

  auto [min, max] = std::minmax_element(rbegin, ++rend, [](auto const &lhs, auto const &rhs) { return lhs.v < rhs.v; });
  return {min->v + max->v};
}

#include "exec.hpp"