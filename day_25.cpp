//
// Created by yalavrinenko on 02.12.2020.
//
#include "main.hpp"

struct AOC_Output{
  size_t value;
  auto operator() () const { return value; }
};

struct AOC_Input{
  long key;
  explicit AOC_Input(std::string const &s): key{std::stoll(s)}{
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

long get_transform_loop(size_t target){
  auto value = 1ull;
  auto loop_size = 0;
  while (value != target){
    value = (value * 7) % 20201227;
    ++loop_size;
  }
  return loop_size;
}

size_t perform_key_loop(size_t subject, size_t loop_size){
  auto value = 1ull;
  while (loop_size > 0){
    value = (value * subject) % 20201227;
    --loop_size;
  }
  return value;
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  auto &key_pkey = v[0].key;
  auto &door_pkey = v[1].key;

  auto key_loop = get_transform_loop(key_pkey);

  auto encription = perform_key_loop(door_pkey, key_loop);

  return {encription};
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  return {2};
}

#include "exec.hpp"