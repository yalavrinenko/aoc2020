//
// Created by yalavrinenko on 02.12.2020.
//
#include "main.hpp"
#include <set>
#include <unordered_set>

struct AOC_Output{
  std::string value;
  auto operator() () const { return value; }
};

struct AOC_Input{
  std::vector<std::string> ingridients;
  std::vector<std::string> restriction;
  explicit AOC_Input(std::string const &line){
    std::istringstream iss{line};
    std::string s;
    do {
      iss >> s;
      if (s[0] != '(')
        ingridients.emplace_back(s);
    } while (s[0] != '(');

    while (std::getline(iss, s, ',')){
      s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
      if (s.find(')') == std::string::npos)
        restriction.emplace_back(s);
      else
        restriction.emplace_back(s.substr(0, s.length() - 1));
    }
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

using ingridients_dictionary = std::unordered_map<std::string, std::set<std::string>>;

ingridients_dictionary create_dictionary(std::vector<AOC_Input> const &v){
  ingridients_dictionary dic;
  for (auto const &entry : v){
    std::set food_set(entry.ingridients.begin(), entry.ingridients.end());
    for (auto const &allergens : entry.restriction){
      if (dic.contains(allergens)){
        std::set<std::string> intersection;
        std::set_intersection(food_set.begin(), food_set.end(), dic[allergens].begin(), dic[allergens].end(),
                              std::inserter(intersection, intersection.end()));
        dic[allergens] = intersection;
      } else {
        dic[allergens] = food_set;
      }
    }
  }
  return dic;
}

using dictionary_vector = std::vector<std::pair<std::string, std::set<std::string>>>;

template <typename RandomIter>
void relaxation(RandomIter begin, RandomIter end){
  if (begin == end)
    return;

  auto const &[k, s] = *begin;
  auto erased_element = *(s.begin());
  if (s.size() > 1)
    std::cout << "WARNING!" << std::endl;

  std::for_each(begin + 1, end, [&erased_element](auto &element){
    element.second.erase(erased_element);
  });

  std::sort(begin + 1, end, [](auto const &lhs, auto const &rhs){
    return lhs.second.size() < rhs.second.size();
  });

  relaxation(begin + 1, end);
}

dictionary_vector simplyfi_dictionary(ingridients_dictionary const &dic){
  std::unordered_map<std::string, std::string> final_dict;

  std::vector<std::pair<std::string, std::set<std::string>>> vec(dic.size());
  std::copy(dic.begin(), dic.end(), vec.begin());

  std::sort(vec.begin(), vec.end(), [](auto const &lhs, auto const &rhs){
    return lhs.second.size() < rhs.second.size();
  });

  relaxation(vec.begin(), vec.end());
  return vec;
}

auto danger_list(auto const &v){
  auto dictionary = create_dictionary(v);
  auto final = simplyfi_dictionary(dictionary);

  std::unordered_set<std::string> failed_ingridients;
  std::transform(final.begin(), final.end(), std::inserter(failed_ingridients, failed_ingridients.end()),
                 [](auto const &kv) {
                   return *(kv.second.begin());
                 });
  return failed_ingridients;
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  auto failed_ingridients = danger_list(v);
  auto count = std::accumulate(v.begin(), v.end(), 0ll, [&failed_ingridients](auto sum, AOC_Input const& in){
    return sum + std::count_if(in.ingridients.begin(), in.ingridients.end(), [&failed_ingridients](auto const &ingr){
      return !failed_ingridients.contains(ingr);
    });
  });
  return {std::to_string(count)};
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  auto dictionary = create_dictionary(v);
  auto final = simplyfi_dictionary(dictionary);

  std::sort(final.begin(), final.end(), [](auto const &lhs, auto const &rhs){
    return lhs.first < rhs.first;
  });

  auto result = std::accumulate(final.begin(), final.end(), std::string{""}, [](auto sum, auto const &el){
    return sum += *(el.second.begin()) + ",";
  });
  result.pop_back();
  return {result};
}

#include "exec.hpp"