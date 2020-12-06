//
// Created by yalavrinenko on 02.12.2020.
//
#include "main.hpp"
#include <array>

struct AOC_Output{
  long value;
  auto operator() () const { return value; }
};

struct AOC_Input{
  std::vector<std::string> person;
  void add_person(std::string info){
    person.emplace_back(std::move(info));
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  static AOC_Input in;
  if (line.empty()){
    auto tmp = in;
    in.person.clear();
    return tmp;
  } else {
    in.add_person(line);
    return {};
  }
}

template <typename condition_t>
auto count_yes(AOC_Input const &inp, condition_t inc_condition){
  std::array<unsigned, 'z' - 'a' + 1> answ{};
  std::for_each(inp.person.begin(), inp.person.end(), [&answ](std::string const &line){
    std::for_each(line.begin(), line.end(), [&answ](char const &symbol){
      answ[symbol - 'a'] += 1;
    });
  });
  return std::count_if(answ.begin(), answ.end(), inc_condition);
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  auto count = 0l;
  std::for_each(v.begin(), v.end(), [&count](AOC_Input const &inp){
    count += count_yes(inp, [](auto v) { return v != 0; });
  });
  return {count};
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  auto count = 0l;
  std::for_each(v.begin(), v.end(), [&count](AOC_Input const &inp){
    auto cyes = count_yes(inp, [size = inp.person.size()](auto v){ return v == size; });
    count += cyes;
  });
  return {count};
}

#include "exec.hpp"