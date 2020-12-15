//
// Created by yalavrinenko on 02.12.2020.
//
#include "main.hpp"

#include <string>
#include <sstream>

struct AOC_Output{
  long value;
  auto operator() () const { return value; }
};

struct AOC_Input{
  size_t min{}, max{};
  char symbol{};
  std::string line{};

  explicit AOC_Input(std::string const &s){
    std::istringstream ss{s};
    ss >> min; ss.get(); ss >> max; ss >> symbol; ss.get(); ss >> line;
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  auto count_lines_func = [](AOC_Input const &data){
    size_t csymbols = std::count(data.line.begin(), data.line.end(), data.symbol);
    return data.min <= csymbols && csymbols <= data.max;
  };
  return AOC_Output{std::count_if(v.begin(), v.end(), count_lines_func)};
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  auto count_lines_func = [](AOC_Input const &data){
    return (data.line[data.max - 1] != data.line[data.min - 1]) &&
           (data.line[data.min - 1] == data.symbol || data.line[data.max - 1] == data.symbol);
  };
  return AOC_Output{std::count_if(v.begin(), v.end(), count_lines_func)};
}

#define DAY_PATH "../day_2.in"
#include "exec.hpp"
