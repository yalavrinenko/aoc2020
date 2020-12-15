//
// Created by yalavrinenko on 02.12.2020.
//
#include "main.hpp"

struct AOC_Output{
  long value;
  auto operator() () const { return value; }
};

struct AOC_Input{
  std::vector<long> numbers;
  explicit AOC_Input(std::string const &s){
    std::string num;
    std::istringstream iss{s};
    while (std::getline(iss, num, ',')){
      numbers.emplace_back(std::stol(num));
    }
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

auto play_game(auto const& v, size_t max_turns){
  auto &in = v.front().numbers;
  std::unordered_map<long, std::pair<long, long>> turns;
  long last_number;
  for (auto i = 1l; auto &e : in){
    turns[e] = {i++, -1};
    last_number = e;
  }

  auto turn_it = in.size() + 1;
  while (turn_it <= max_turns){
    auto const &[prev, pprev] = turns[last_number];
    auto say = prev - pprev;
    if (pprev == -1){
      say = 0;
    }
    if (turns.contains(say)){
      turns[say] = std::pair{static_cast<long>(turn_it), turns[say].first};
    } else {
      turns[say] = std::pair{static_cast<long>(turn_it), -1};
    }
    last_number = say;
    ++turn_it;
  }
  return last_number;
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  return {play_game(v, 2020)};
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  return {play_game(v, 30000000)};
}

#include "exec.hpp"