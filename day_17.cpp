//
// Created by yalavrinenko on 02.12.2020.
//
#include "main.hpp"
#include <set>

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

struct cell_3d{
  long x, y, z;

  cell_3d(long _x, long _y): x{_x}, y{_y}, z{0} {}
  cell_3d(long _x, long _y, long _z): x{_x}, y{_y}, z{_z} {}

  [[nodiscard]] std::set<cell_3d> neighlist() const {
    std::set<cell_3d> neigh;
    for (auto i : {x - 1, x, x + 1})
      for (auto j : {y - 1, y, y + 1})
        for (auto k : {z - 1, z, z + 1}){
          if (!(i == x && j == y && k == z)){
            neigh.insert({i, j, k});
          }
        }
    return neigh;
  }

  bool operator < (cell_3d const &rhs) const {
    if (x == rhs.x) {
      if (y == rhs.y) return z < rhs.z;
      else
        return y < rhs.y;
    } else
      return x < rhs.x;
  }
};

struct cell_4d{
  long x, y, z, w;

  cell_4d(long _x, long _y): x{_x}, y{_y}, z{0}, w{0} {}
  cell_4d(long _x, long _y, long _z, long w): x{_x}, y{_y}, z{_z}, w{w} {}

  [[nodiscard]] std::set<cell_4d> neighlist() const {
    std::set<cell_4d> neigh;
    for (auto i : {x - 1, x, x + 1})
      for (auto j : {y - 1, y, y + 1})
        for (auto k : {z - 1, z, z + 1})
          for (auto l : {w - 1, w, w + 1}){
          if (!(i == x && j == y && k == z && l == w)){
            neigh.insert({i, j, k, l});
          }
        }
    return neigh;
  }

  bool operator < (cell_4d const &rhs) const {
    if (x == rhs.x) {
      if (y == rhs.y) {
        if (z == rhs.z) return w < rhs.w;
        else return z < rhs.z;
      }
      else
        return y < rhs.y;
    } else
      return x < rhs.x;
  }
};


template<typename cell_t>
class InfiniteLife{
public:
  explicit InfiniteLife(std::vector<AOC_Input> const &map){
    for (auto idx = 0; auto const &line : map){
      for (auto i = 0; auto const &c : line.line){
        if (c == '#'){
          active_cell_.insert(cell_t(idx, i));
        }
        ++i;
      }
      ++idx;
    }
  }

  template<class switch_state_t>
  auto next_iteration(switch_state_t switch_callback){
    std::set<cell_t> epoch;

    std::set<cell_t> may_active;

    auto process_neight = [&may_active, this](auto c, bool construct_neight){
      auto active = 0;
      auto inactive = 0;
      for (auto const& nc : c.neighlist()) {
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
private:
  std::set<cell_t> active_cell_;
};

AOC_Output part_1(std::vector<AOC_Input> const &v){
  InfiniteLife<cell_3d> life(v);
  auto active_cells = 0l;

  auto switch_function = [](auto is_active, long active, long inactive){
    if (is_active && (active == 2 || active == 3))
      return true;
    if (!is_active && active == 3)
      return true;

    return false;
  };

  for (auto i = 0; i < 6; ++i) {
    active_cells = life.next_iteration(switch_function);
  }
  return {active_cells};
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  InfiniteLife<cell_4d> life(v);
  auto active_cells = 0l;

  auto switch_function = [](auto is_active, long active, long inactive){
    if (is_active && (active == 2 || active == 3))
      return true;
    if (!is_active && active == 3)
      return true;

    return false;
  };

  for (auto i = 0; i < 6; ++i) {
    active_cells = life.next_iteration(switch_function);
  }
  return {active_cells};
}

#include "exec.hpp"