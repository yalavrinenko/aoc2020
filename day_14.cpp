//
// Created by yalavrinenko on 02.12.2020.
//
#include "main.hpp"
#include <bitset>

struct AOC_Output{
  long value;
  auto operator() () const { return value; }
};

struct AOC_Input{
  std::string entry;
  explicit AOC_Input(std::string s): entry{std::move(s)}{
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

using value_t = std::bitset<36>;

class aoc_bitmask{
public:
  aoc_bitmask() = default;

  explicit aoc_bitmask(std::string const& mask){
    from_str(mask);
  }

  void from_str(std::string const& mask){
    replace_map_.clear();
    for (auto i = 0u; i < mask.size(); ++i)
      if (mask[i] != 'X')
        replace_map_[mask.size() - i - 1] = mask[i] - '0';
  }

  void apply(value_t &value){
    for (auto const &[k, v] : replace_map_)
      value[k] = v;
  }

private:
  std::unordered_map<int, int> replace_map_;
};

AOC_Output part_1(std::vector<AOC_Input> const &v){
  aoc_bitmask bitmask;
  std::unordered_map<long long, value_t> mem;
  for (auto const &e : v){
    if (e.entry.find("mask") != std::string::npos){
      std::string mask;
      std::istringstream iss{e.entry}; iss.ignore(7); iss >> mask;
      bitmask.from_str(mask);
    } else {
      std::istringstream iss{e.entry};
      iss.ignore(4);
      long long addr; iss >> addr;
      iss.ignore(4);
      long long value; iss >> value;
      mem[addr] = value_t (value);
      bitmask.apply(mem[addr]);
    }
  }
  auto sum = std::accumulate(mem.begin(), mem.end(), 0ll, [](auto sum, auto const &kv){
    auto const& [k, v] = kv;
    return sum + v.to_ullong();
  });

  return {sum};
}

void fill_map(std::unordered_map<value_t, long long> &mem, std::string mask, long long value){
  if (mask.find('X') == std::string::npos){
    value_t bm(mask);
    mem[bm.to_ullong()] = value;
  } else {
    auto id = mask.find('X');
    mask[id] = '0';
    fill_map(mem, mask, value);
    mask[id] = '1';
    fill_map(mem, mask, value);
  }
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  std::string bitmask;
  std::unordered_map<value_t , long long> mem;
  for (auto const &e : v){
    if (e.entry.find("mask") != std::string::npos){
      std::istringstream iss{e.entry}; iss.ignore(7); iss >> bitmask;
    } else {
      std::istringstream iss{e.entry};
      iss.ignore(4);
      long long addr; iss >> addr;
      iss.ignore(4);
      long long value; iss >> value;

      value_t addr_bit(addr);
      auto address_mask = addr_bit.to_string();

      for (auto i =0; i < 36; ++i){
        if (bitmask[i] != '0')
          address_mask[i] = bitmask[i];
      }

      fill_map(mem, address_mask, value);
    }
  }
  auto sum = std::accumulate(mem.begin(), mem.end(), 0ll, [](auto sum, auto const &kv){
    auto const& [k, v] = kv;
    return sum + v;
  });

  return {sum};
}

#include "exec.hpp"