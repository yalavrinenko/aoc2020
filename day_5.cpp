//
// Created by yalavrinenko on 02.12.2020.
//
#include <utility>

#include "main.hpp"
#include <iterator>
#include <concepts>
#include <ranges>

struct AOC_Output{
  long value;
  auto operator() () const { return value; }
  AOC_Output(long v): value{v} {}
};

struct AOC_Input{
  std::string order;
  explicit AOC_Input(std::string s): order{std::move(s)}{
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

template <typename ForwardIterator>
decltype(auto) binary_search(long int lo, long int hi, ForwardIterator const &action){
  if (lo >= hi - 1)
    return lo;
  else {
    auto middle = (hi + lo) / 2;
    if (*action == 'L')
      hi = middle;
    else
      lo = middle;
    return binary_search(lo, hi, std::next(action));
  }
}

auto get_seat_id(std::string board_id){
  auto replaces = {std::tuple{'F', 'L'}, {'B', 'H'}, {'R', 'H'}};
  for (auto const &[who, what] : replaces)
    std::replace(board_id.begin(), board_id.end(), who, what);

  auto row = binary_search(0l, 128l, board_id.substr(0, 7).cbegin());
  auto col = binary_search(0l, 8l, board_id.substr(7).cbegin());
  return row * 8 + col;
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  auto max_id = 0l;
  for (auto &e : v){
    max_id = std::max(max_id, get_seat_id(e.order));
  }
  return max_id;
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  std::vector<int> mask(part_1(v).value + 1, 0);
  for (auto const &sentry : v){
    mask[get_seat_id(sentry.order)] = 1;
  }
  for (auto i = 1u; i < mask.size() - 1; ++i)
    if (mask[i] == 0 && mask[i - 1] != 0 && mask[i + 1] != 0)
      return i;
  return 0;
}

#include "exec.hpp"