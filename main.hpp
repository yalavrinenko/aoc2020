//
// Created by yalavrinenko on 02.12.2020.
//

#ifndef AOC_2020_MAIN_HPP
#define AOC_2020_MAIN_HPP
#include <vector>
#include <iostream>
#include <fstream>
#include <optional>

#include <unordered_map>
#include <sstream>
#include <numeric>


struct AOC_Input;
struct AOC_Output;

AOC_Output part_1(std::vector<AOC_Input> const &in);
AOC_Output part_2(std::vector<AOC_Input> const &in);

struct AOC_Reader{
  static std::optional<AOC_Input> create_from_string(std::string const &line);
};


#endif//AOC_2020_MAIN_HPP
