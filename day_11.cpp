//
// Created by yalavrinenko on 02.12.2020.
//
#include "main.hpp"
#include <numeric>
#include <algorithm>

struct AOC_Output{
  long value;
  auto operator() () const { return value; }
};

struct AOC_Input{
  std::string line;
  explicit AOC_Input(std::string s): line{std::move(s)}{
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

class EvolutionMap{
public:
  explicit EvolutionMap(std::vector<AOC_Input> const &v){
    seats_.emplace_back(std::string(v.back().line.size(), '.'));
    seats_.reserve(v.size() + 2);
    std::transform(v.begin(), v.end(), std::back_inserter(seats_), [](auto const& vi){
      return vi.line;
    });
    seats_.emplace_back(std::string(v.back().line.size(), '.'));

    for (auto &sline : seats_) {
      sline = "." + sline + ".";
    }
  }

  [[nodiscard]] auto get_occupied() const {
    auto count = std::accumulate(seats_.begin() + 1, seats_.end() - 1, 0l, [](auto sum, auto const &s){
      return sum + std::count_if(s.begin() + 1, s.end() - 1, [](auto const &c) {
               return c == '#';
             });
    });

    return count;
  }

  template<typename change_state_t>
  bool iteration(change_state_t func) {
    auto next_state = seats_;
    for (auto i = 1u; i < seats_.size() - 1; ++i)
      for (auto j = 1u; j < seats_.size() - 1; ++j){
        next_state[i][j] = func(seats_, i, j);
      }

    auto flag = next_state == seats_;
    seats_ = next_state;
    return flag;
  }

  void dump() const {
    std::cout << "\n\nEpoch" << std::endl;
    std::copy(seats_.begin(), seats_.end(), std::ostream_iterator<std::string>{std::cout, "\n"});
  }

private:
  std::vector<std::string> seats_;
};

AOC_Output part_1(std::vector<AOC_Input> const &v){
  EvolutionMap map(v);

  auto empty_filled = [](auto const& map, auto x, auto y){
    auto filled = 0ul;
    auto empty = 0ul;
    for (auto i : {x - 1, x, x + 1})
      for (auto j : {y - 1, y, y + 1}){
        if (!(i == x && j == y)) {
          if (map[i][j] == 'L') ++empty;
          if (map[i][j] == '#') ++filled;
        }
      }
    return std::pair{empty, filled};
  };

  auto func = [empty_filled](auto const& map, auto i, auto j){
    auto [e, f] = empty_filled(map, i, j);
    if (map[i][j] == 'L' && f == 0)
      return '#';
    if (map[i][j] == '#' && f >= 4)
      return 'L';
    return map[i][j];
  };

  while (!map.iteration(func)){
  }

  return {map.get_occupied()};
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  auto direction_count = [](auto const& map, auto x, auto y, auto dx, auto dy){
    auto i = x + dx, j = y + dy;
    while (i > 0 && i < map.size() - 1 && j > 0 && j < map.front().size() - 1){
      if (map[i][j] == '#') return std::pair{0, 1};
      if (map[i][j] == 'L') return std::pair{1, 0};
      i += dx; j += dy;
    }
    return std::pair{1, 0};
  };
  auto empty_occupied = [direction_count](auto const& map, auto x, auto y){
    auto occ = 0ul, emp = 0ul;
    for (auto dx : {-1, 0, 1})
      for (auto dy : {-1, 0, 1}){
        if (!(dx == 0 && dy == 0)) {
          auto [demp, docc] = direction_count(map, x, y, dx, dy);
          occ += docc;
          emp += demp;
        }
      }
    return std::pair{emp, occ};
  };

  auto next_state = [empty_occupied](auto const& map, auto i, auto j){
    auto [emp, occ] = empty_occupied(map, i, j);
    if (map[i][j] == 'L' && occ == 0)
      return '#';
    if (map[i][j] == '#' && occ >= 5)
      return 'L';
    return map[i][j];
  };

  EvolutionMap map(v);

  while (!map.iteration(next_state)){
  }
  return {map.get_occupied()};
}

#include "exec.hpp"