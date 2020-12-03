//
// Created by yalavrinenko on 02.12.2020.
//
#include <utility>

#include "main.hpp"

struct AOC_Output{
  long value;
  auto operator() () const { return value; }
};

struct AOC_Input{
  std::string line;
  explicit AOC_Input(std::string s): line{std::move(s)}{
  }
  auto& operator[] (int i) const {
    return line[i];
  }

  auto& operator[] (int i){
    return line[i];
  }
};

AOC_Input AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

long count_collision(std::vector<AOC_Input> const &v, unsigned right_shift, unsigned down_shift){
  auto j = 0u, i = 0u;
  auto count = 0;
  while (i < v.size()){
    if (v[i][j] == '#')
      ++count;
    i += down_shift;
    j = (j + right_shift) % v.back().line.size();
  }
  return count;
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  return AOC_Output{count_collision(v, 3, 1)};
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  std::vector<std::pair<unsigned, unsigned>> collision_vector{{1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}};
  auto product = 1l;
  for (auto const &[r, d] : collision_vector)
    product *= count_collision(v, r, d);
  return AOC_Output{product};
}

#include "exec.hpp"