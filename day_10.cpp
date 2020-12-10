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
  size_t voltage;
  explicit AOC_Input(std::string const &s): voltage{std::stoul(s)}{
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  std::vector<size_t> adapters(v.size());
  std::transform(v.begin(), v.end(), adapters.begin(), [](auto const &a) { return a.voltage; });
  std::sort(adapters.begin(), adapters.end());
  adapters.push_back(adapters.back() + 3);

  std::array<size_t, 4> diff_count{0};
  auto current_voltage{0ull};
  for (auto const &volt : adapters){
    ++diff_count[volt - current_voltage];
    current_voltage = volt;
  }
  //std::cout << diff_count[0] << " " << diff_count[1] << " " << diff_count[2] << " " << diff_count[3] << std::endl;
  return {diff_count[1] * diff_count[3]};
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  std::vector<size_t> adapters(v.size());
  std::transform(v.begin(), v.end(), adapters.begin(), [](auto const &a) { return a.voltage; });
  std::sort(adapters.begin(), adapters.end());
  adapters.push_back(adapters.back() + 3);

  std::vector<size_t> ways(adapters.back() + 1, 0);
  ways[0] = 1;
  for (auto const &joltage: adapters){
    ways[joltage] = ((joltage >= 3) ? ways[joltage - 3] : 0) +
        ((joltage >= 2) ? ways[joltage - 2] : 0) +
        ((joltage >= 1) ? ways[joltage - 1] : 0);
  }

  return {ways[adapters.back()]};
}

#include "exec.hpp"