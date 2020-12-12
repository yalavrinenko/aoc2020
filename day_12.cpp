//
// Created by yalavrinenko on 02.12.2020.
//
#include "main.hpp"
#include <functional>
#include <sstream>
#include <utility>
struct AOC_Output{
  long value;
  auto operator() () const { return value; }
};

struct AOC_Input{
  std::string s;
  explicit AOC_Input(std::string s) : s{std::move(s)}{
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

struct MovingInstruction{
public:
  using moving_function_t = std::function<std::pair<long, long>&(std::pair<long, long> &, std::pair<long, long> &, long)>;

private:
  static auto& rotate(auto &heading, long angle, char direction){
    auto sign = (direction == 'L') ? -1 : 1;
    while (angle >0){
      auto [hx, hy] = heading;
      heading = {sign * hy, -sign * hx};
      angle -= 90;
    }
    return heading;
  }

  static auto& init() {
    static std::unordered_map<char, moving_function_t> func_;

    func_['N'] = moving_function_t(
        [](auto &c, auto &h, long v) -> decltype(auto) {
          c.second += v;
          return c;
        });

    func_['S'] = moving_function_t(
        [](auto &c, auto &h, long v) -> decltype(auto) {
          c.second -= v;
          return c;
        });

    func_['E'] = moving_function_t(
        [](auto &c, auto &h, long v) -> decltype(auto) {
          c.first += v;
          return c;
        });


    func_['W'] = moving_function_t(
        [](auto &c, auto &h, long v) -> decltype(auto) {
          c.first -= v;
          return c;
        });

    func_['L'] = moving_function_t(
        [](auto &c, auto &h, long v) -> decltype(auto) {
          h = rotate(h, v, 'L');
          return c;
        });

    func_['R'] = moving_function_t(
        [](auto &c, auto &h, long v) -> decltype(auto) {
          h = rotate(h, v, 'R');
          return c;
        });

    func_['F'] = moving_function_t(
        [](auto &c, auto &h, long v) -> decltype(auto) {
          auto &[cx, cy] = c; auto &[hx, hy] = h;
          cx += hx * v; cy += hy * v;
          return c;
        });

    return func_;
  }

public:
  static decltype(auto) move(auto &current, auto& heading, char direction, long step){
    auto& func_ = MovingInstruction::init();
    return func_.at(direction)(current, heading, step);
  }
};

AOC_Output part_1(std::vector<AOC_Input> const &v){
  auto direction = std::pair{1l, 0l};
  auto position = std::pair{0l, 0l};

  for (auto const &line : v){
    std::istringstream  iss{line.s};
    char dir; iss >> dir;
    long step; iss >> step;
    position = MovingInstruction::move(position, direction, dir, step);
  }

  return {std::abs(position.first) + std::abs(position.second)};
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  auto waypoints = std::pair{10l, 1l};
  auto position = std::pair{0l, 0l};

  for (auto const &line : v){
    std::istringstream  iss{line.s};
    char dir; iss >> dir;
    long step; iss >> step;
    if (dir == 'F')
      position = MovingInstruction::move(position, waypoints, dir, step);
    else {
      waypoints = MovingInstruction::move(waypoints, waypoints, dir, step);
    }
  }

  return {std::abs(position.first) + std::abs(position.second)};
}

#include "exec.hpp"