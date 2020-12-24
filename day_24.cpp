//
// Created by yalavrinenko on 02.12.2020.
//
#include "main.hpp"
#include <algorithm>
#include <set>

struct AOC_Output{
  unsigned long value;
  auto operator() () const { return value; }
};

auto operator+ (std::pair<int, int> const &lhs, std::pair<int, int> const &rhs){
  return std::pair{lhs.first + rhs.first, lhs.second + rhs.second};
}

class TilesShifts{
public:
  static auto& shift_map(){
    static std::unordered_map<std::string, std::pair<int, int>> shifts {
        {"e", {2, 0}},
        {"se", {1, -2}},
        {"sw", {-1, -2}},
        {"w", {-2, 0}},
        {"nw", {-1, 2}},
        {"ne", {1, 2}}
    };

    return shifts;
  }

  static auto neighlist(std::pair<int, int> const &ref){
    std::vector<std::pair<int, int>> neigh(shift_map().size());
    std::transform(shift_map().begin(), shift_map().end(), neigh.begin(), [&ref](auto const &shift) {
      return ref + shift.second;
    });
    return neigh;
  }
};

struct AOC_Input{
  std::vector<std::pair<int, int>> shifts;
  explicit AOC_Input(std::string const &s){
    std::istringstream iss{s};

    char c;
    while (iss >> c){
      std::string direction{c};
      if (c == 'n' || c == 's') {
        iss >> c; direction += c;
      }
      shifts.emplace_back(TilesShifts::shift_map()[direction]);
    }
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

auto initial_tile_set(auto const &input){
  std::set<std::pair<long, long>> black_tiles;

  for (auto const &tile_shifts : input){
    std::pair ref_tile{0, 0};
    for (auto const &shift : tile_shifts.shifts)
      ref_tile = ref_tile + shift;
    if (black_tiles.contains(ref_tile))
      black_tiles.erase(ref_tile);
    else
      black_tiles.insert(ref_tile);
  }
  return black_tiles;
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  std::set<std::pair<long, long>> black_tiles = initial_tile_set(v);

  return {black_tiles.size()};
}

template<typename cell_t>
class InfiniteLife{
public:
  explicit InfiniteLife(std::set<cell_t> map) : active_cell_{std::move(map)}{
  }

  template<class switch_state_t>
  auto next_iteration(switch_state_t switch_callback){
    std::set<cell_t> epoch;

    std::set<cell_t> may_active;

    auto process_neight = [&may_active, this](auto c, bool construct_neight){
      auto active = 0;
      auto inactive = 0;
      for (auto const& nc : TilesShifts::neighlist(c)) {
        if (active_cell_.contains(nc)) ++active;
        else {
          ++inactive;
          if (construct_neight) may_active.insert(nc);
        }
      }

      return std::pair{active, inactive};
    };

    for (auto const& c : active_cell_){
      auto [active, inactive] = process_neight(c, true);
      if (switch_callback(1, active, inactive))
        epoch.insert(c);
    }

    for (auto const& c : may_active){
      auto [active, inactive] = process_neight(c, false);
      if (switch_callback(0, active, inactive))
        epoch.insert(c);
    }

    active_cell_ = std::move(epoch);
    return active_cell_.size();
  }

  auto const& state() const {return active_cell_;}
private:
  std::set<cell_t> active_cell_;
};


AOC_Output part_2(std::vector<AOC_Input> const &v){
  auto tiles = initial_tile_set(v);

  InfiniteLife<std::pair<long int, long int>> cell_automaton{tiles};

  auto switch_callback = [](auto state, auto active, auto inactive){
    if (state == 1 && (active == 0 || active > 2))
      return 0;
    if (state == 0 && (active == 2))
      return 1;
    return state;
  };

  int N = 100;
  while (N > 0){
    cell_automaton.next_iteration(switch_callback);
    --N;
  }

  return {cell_automaton.state().size()};
}

#include "exec.hpp"