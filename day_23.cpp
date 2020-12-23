//
// Created by yalavrinenko on 02.12.2020.
//
#include "main.hpp"
#include <ranges>
#include <algorithm>
#include <chrono>

struct AOC_Output{
  std::string value;
  auto operator() () const { return value; }
};

struct AOC_Input{
  std::vector<int> cups;
  explicit AOC_Input(std::string const &s){
    std::ranges::transform(s, std::back_inserter(cups), [](auto const &c) { return c - '0'; });
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

using smart_list = std::vector<long>;

template <typename ostream_t>
ostream_t& print(smart_list const &cc, auto begin, ostream_t &oss){
  auto it = begin;
  do{
    oss << it;
    it = cc[it];
  }while (it != begin);
  return oss;
}

void shufle_cups(auto N, auto current, auto size, smart_list &cc){
  auto next_value = [&size] (auto rep){
    if (rep == 1)
      return size;
    else
      return (rep - 1) % size;
  };

  auto take_after = [&cc](auto ref, auto n){
    auto begin = cc[ref];
    auto end = begin;
    while (n - 1 > 0){
      end = cc[end];
      --n;
    }
    cc[ref] = cc[end];
    return std::pair{begin, end};
  };

  auto in_range = [&cc](auto begin, auto end, auto v){
    while (begin != end && begin != static_cast<long>(v)){
      begin = cc[begin];
    }
    return begin;
  };

  auto insert_after = [&cc](auto after, auto begin, auto end){
    cc[end] = cc[after];
    cc[after] = begin;
  };

  while (N > 0){
    auto [begin, end] = take_after(current, 3);

    unsigned long insert_after_value = next_value(current);
    while (in_range(begin, cc[end], insert_after_value) != cc[end]){
      insert_after_value = next_value(insert_after_value);
    }

    insert_after(insert_after_value, begin, end);

    current = cc[current];
    --N;
  }
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  auto &cups = v.front().cups;
  smart_list cc(cups.size() + 1);

  for (auto i = 1u; i < cups.size(); ++i){
    cc[cups[i - 1]] = cups[i];
  }
  cc[cups.back()] = cups.front();

  shufle_cups(100, cups.front(), v.front().cups.size(), cc);

  std::ostringstream oss;
  print(cc, cc[1], oss);

  auto str = oss.str(); str.pop_back();
  return {str};
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  auto &cups = v.front().cups;
  auto const SIZE = 1000000ul;
  smart_list cc(SIZE + 1);

  for (auto i = 1u; i < cups.size(); ++i){
    cc[cups[i - 1]] = cups[i];
  }
  cc[cups.back()] = 10;
  for (auto i = 10u; i < cc.size(); ++i)
    cc[i] = i + 1;
  cc.back() = cups.front();


  shufle_cups(10000000, cups.front(), SIZE, cc);

  auto v1 = cc[1];
  auto v2 = cc[cc[1]];
  return {std::to_string(v1 * v2)};
}

#include "exec.hpp"