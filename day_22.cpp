//
// Created by yalavrinenko on 02.12.2020.
//
#include "main.hpp"
#include <algorithm>
#include <numeric>
#include <queue>
#include <ranges>
#include <set>
#include <unordered_set>

struct AOC_Output {
  long value;
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::queue<long> player_1, player_2;
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  static AOC_Input input;
  static int player = 0;
  if (line.empty() && player == 2) {
    auto copy = input;
    input = AOC_Input();
    player = 0;
    return copy;
  }

  if (line.empty()) return {};
  if (line == "Player 1:") {
    player = 1;
    return {};
  }
  if (line == "Player 2:") {
    player = 2;
    return {};
  }

  if (player == 1) input.player_1.push(std::stol(line));
  if (player == 2) input.player_2.push(std::stol(line));

  return {};
}

long deck_score(auto deq) {
  auto transformed = std::views::iota(1l, static_cast<long>(deq.size()) + 1) | std::views::reverse |
                     std::views::transform([&deq](auto const &v) {
                       auto front = deq.front();
                       deq.pop();
                       return v * front;
                     });

  return {std::reduce(transformed.begin(), transformed.end())};
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto clone = v.front();
  auto &p1 = clone.player_1;
  auto &p2 = clone.player_2;
  while (!p1.empty() && !p2.empty()) {
    auto bet_1 = p1.front();
    p1.pop();
    auto bet_2 = p2.front();
    p2.pop();
    if (bet_1 > bet_2) {
      p1.push(bet_1);
      p1.push(bet_2);
    } else {
      p2.push(bet_2);
      p2.push(bet_1);
    }
  }

  auto &nempty = (p1.empty()) ? p2 : p1;

  return {deck_score(nempty)};
}

auto recursive_card_game(std::queue<long> &p1, std::queue<long> &p2) {
  std::set<std::pair<long, long>> states;

  auto create_slice = [](std::queue<long> dec, long n) {
    std::queue<long> slice;
    while (n > 0){
      --n;
      slice.emplace(dec.front()); dec.pop();
    }
    return slice;
  };

  while (!p1.empty() && !p2.empty()) {
    auto state_p1 = deck_score(p1), state_p2 = deck_score(p2);
    if (states.contains({state_p1, state_p2})) return 1;
    else
      states.insert({state_p1, state_p2});

    auto bet_1 = p1.front();    p1.pop();
    auto bet_2 = p2.front();    p2.pop();

    if (bet_1 <= static_cast<long>(p1.size()) && bet_2 <= static_cast<long>(p2.size())) {
      auto dec_slice_1 = create_slice(p1, bet_1);
      auto dec_slice_2 = create_slice(p2, bet_2);

      auto winner = recursive_card_game(dec_slice_1, dec_slice_2);
      if (winner == 1) {
        p1.emplace(bet_1);
        p1.emplace(bet_2);
      } else {
        p2.emplace(bet_2);
        p2.emplace(bet_1);
      }
    } else {
      if (bet_1 > bet_2) {
        p1.emplace(bet_1);
        p1.emplace(bet_2);
      } else {
        p2.emplace(bet_2);
        p2.emplace(bet_1);
      }
    }
  }

  return (p1.empty()) ? 2 : 1;
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto copy = v.front();
  recursive_card_game(copy.player_1, copy.player_2);

  return {deck_score((copy.player_1.empty()) ? copy.player_2 : copy.player_1)};
}

#include "exec.hpp"