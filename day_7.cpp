//
// Created by yalavrinenko on 02.12.2020.
//
#include "main.hpp"
#include <sstream>
#include <list>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <ranges>

struct AOC_Output{
  long value;
  auto operator() () const { return value; }
};

struct AOC_Input{
  struct Bag{
    size_t count{0};
    std::string color{};
  };
  std::string main_color;
  std::list<Bag> bags;

  explicit AOC_Input(std::string s){
    using namespace std::string_literals;
    s.replace(s.find(" bags contain "), " bags contain "s.size(), "=");

    std::istringstream iss{s};
    std::getline(iss, main_color, '=');

    for (auto bag = get_entry(iss); bag; bag = get_entry(iss))
      bags.emplace_back(bag.value());
  }
  static std::optional<Bag> get_entry(std::istringstream& iss){
    Bag entry;
    if (iss >> entry.count){
      std::getline(iss, entry.color, ',');
      entry.color = entry.color.substr(1, entry.color.find_last_of(' ') - 1);
      return entry;
    } else {
      return {};
    }
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

bool is_contain(std::unordered_map<std::string, std::list<AOC_Input::Bag>> const &restriction,
                std::string const &key, std::string const &value){
  if (std::find_if(restriction.at(key).begin(), restriction.at(key).end(), [&value](auto const &v){
        return v.color == value;
      }) != restriction.at(key).end())
    return true;
  else
    return std::ranges::any_of(restriction.at(key), [&value, &restriction](auto const &bag){
      return is_contain(restriction, bag.color, value);
    });
}

size_t count_bags(std::unordered_map<std::string, std::list<AOC_Input::Bag>> const &restriction,
                  std::string const &key){
//  if (restriction.at(key).empty())
//    return 1;

  auto count_function = [&restriction](auto sum, AOC_Input::Bag const &bag){
    return sum + bag.count * count_bags(restriction, bag.color);
  };

  return std::accumulate(restriction.at(key).begin(), restriction.at(key).end(), 1, count_function);
}

auto convert(auto const &v){
  std::unordered_map<std::string, std::list<AOC_Input::Bag>> restriction;
  std::transform(v.begin(), v.end(), std::inserter(restriction, restriction.begin()), [](AOC_Input const &i){
    return std::pair{i.main_color, i.bags};
  });
  return restriction;
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  std::unordered_map<std::string, std::list<AOC_Input::Bag>> restriction = convert(v);

  std::string value = "shiny gold";
  return { std::accumulate(v.begin(), v.end(), 0, [&restriction, &value](auto sum, AOC_Input const &v){
      return sum + is_contain(restriction, v.main_color, value);
    }) };
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  std::unordered_map<std::string, std::list<AOC_Input::Bag>> restriction = convert(v);

  std::string value = "shiny gold";
  return {static_cast<long int>(count_bags(restriction, value)) - 1 };
}

#include "exec.hpp"