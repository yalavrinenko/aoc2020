//
// Created by yalavrinenko on 02.12.2020.
//
#include "main.hpp"
#include <algorithm>
#include <functional>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

struct AOC_Output {
  long value;
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::unordered_map<std::string, std::string> fields;

  void append_fields(std::string const &line) {
    std::istringstream ss{line};
    std::string s;
    while (ss >> s) fields.emplace(parse_enrty(s));
  }

  static std::pair<std::string, std::string> parse_enrty(std::string const &entry) {
    auto ddot_pos = entry.find(':');
    return {entry.substr(0, ddot_pos), entry.substr(ddot_pos + 1)};
  }

  void clear() { fields.clear(); }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  static AOC_Input passport_entry;
  if (line.empty()) {
    auto tmp = passport_entry;
    passport_entry.clear();
    return tmp;
  } else {
    passport_entry.append_fields(line);
    return {};
  }
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto required_fields = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};
  auto count = 0l;
  for (auto const &document : v) {
    bool is_ok = true;
    for (auto const &field : required_fields)
      if (!document.fields.contains(field)) {
        is_ok = false;
        break;
      }
    if (is_ok) ++count;
  }

  return {count};
}

class Checkers {
public:
  bool check(std::string const &key, std::string const &value) const { return checkers_.at(key)(value); }

  Checkers() {
    checkers_["byr"] = [](auto const &s) { return in_range(s, 1920, 2002); };
    checkers_["iyr"] = [](auto const &s) { return in_range(s, 2010, 2020); };
    checkers_["eyr"] = [](auto const &s) { return in_range(s, 2020, 2030); };
    checkers_["hgt"] = Checkers::hgt_check;
    checkers_["hcl"] = [](auto const &s) { return s[0] == '#' && digits_check(s.substr(1), 6, true); };
    checkers_["pid"] = [](auto const &s) { return digits_check(s, 9, true); };
    checkers_["ecl"] = [](auto const &s) { return one_of(s); };
    checkers_["cid"] = [](auto const &s) { return true; };
    [[maybe_unused]] auto check = [this](auto const &key, auto const &value) {
      std::cout << key << " " << value << " " << std::boolalpha << checkers_[key](value) << std::endl;
    };
  }

private:
  static bool in_range_impl(long iv, long low, long hi) { return (low <= iv && iv <= hi); }
  static bool in_range(std::string const &v, long low, long hi) {
    try {
      auto iv = std::stol(v);
      return in_range_impl(iv, low, hi);
    } catch (...) { return false; }
  }
  static bool hgt_check(std::string const &v) {
    std::istringstream ss{v};
    long height = 0;
    ss >> height;
    std::string unit;
    if (!(ss >> unit)) return false;
    if (unit == "cm") return in_range_impl(height, 150, 193);
    else if (unit == "in")
      return in_range_impl(height, 59, 76);
    else
      return false;
  }
  static bool digits_check(std::string const &v, unsigned long digit_limits, bool hex = false) {
    if (v.length() != digit_limits) return false;

    auto check = [hex](char c) {
      if (hex) return ('0' <= c && c <= '9') || ('a' <= c && c <= 'f');
      else
        return ('0' <= c && c <= '9');
    };

    return std::ranges::all_of(v, check);
  }
  static bool one_of(std::string const &v) {
    static auto colors = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
    return std::ranges::any_of(colors, [&v](auto const &element) { return element == v; });
  }


  std::unordered_map<std::string, std::function<bool(std::string const &)>> checkers_;
};

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto required_fields = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};
  auto count = 0l;
  auto checker = Checkers();
  for (auto const &document : v) {
    bool is_ok = true;
    for (auto const &field : required_fields)
      if (!document.fields.contains(field)) {
        is_ok = false;
        break;
      }

    if (is_ok) {
      for (auto const &[key, value] : document.fields) {
        is_ok = checker.check(key, value);
        if (!is_ok) {
          //std::cout << "Bad: " << key << " " << value << std::endl;
          break;
        }
      }
      if (is_ok) ++count;
    }
  }

  return {count};
}

#include "exec.hpp"