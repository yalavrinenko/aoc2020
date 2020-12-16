//
// Created by yalavrinenko on 02.12.2020.
//
#include "main.hpp"
#include <algorithm>
#include <functional>
#include <unordered_set>

struct AOC_Output{
  size_t value;
  auto operator() () const { return value; }
};

struct AOC_Input{
  std::unordered_map<std::string, std::function<bool(long)>> restriction;
  std::vector<long> ticket;
  std::vector<std::vector<long>> nearby;

  explicit AOC_Input(){
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  static AOC_Input data;
  static int stage = 0;

  if (line == "your ticket:"){
    stage = 1; return{};
  }

  if (line == "nearby tickets:"){
    stage = 2; return {};
  }

  if (line.empty() && stage == 2){
    auto copy = data;
    data = AOC_Input();
    stage = 0;

    return copy;
  }

  if (line.empty()){
    return {};
  }

  if (stage == 0){
    std::istringstream iss{line};
    std::string field; std::getline(iss, field, ':');
    long sr1, fr1, sr2, fr2;

    iss >> sr1; iss.ignore(); iss >> fr1;
    std::string OR; iss >> OR;
    iss >> sr2; iss.ignore(); iss >> fr2;

    data.restriction[field] = [sr1, fr1, sr2, fr2](auto v) {
      return (sr1 <= v && v <= fr1) || (sr2 <= v && v <= fr2);
    };
  }

  auto parse_line = [](auto str){
    std::replace(str.begin(), str.end(), ',', ' ');
    std::istringstream iss{str};
    std::vector<long> data;
    std::copy(std::istream_iterator<long>{iss}, std::istream_iterator<long>{}, std::back_inserter(data));
    return data;
  };

  if (stage == 1){
    data.ticket = parse_line(line);
  }

  if (stage == 2){
    data.nearby.emplace_back(parse_line(line));
  }
  return {};
}

bool check_restriction_set(auto const& restriction, long v) {
  return std::ranges::any_of(restriction.begin(), restriction.end(), [v](auto const& restr){
    auto const& [_, checker] = restr;
    return checker(v);
  });
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  auto const &restriction = v.front().restriction;
  auto const &near = v.front().nearby;

  auto check = [&restriction](long v){
    return check_restriction_set(restriction, v);
  };

  auto sum = std::accumulate(near.begin(), near.end(), 0ull, [&check](auto sum, auto const& ticket_info){
    auto ticket_error = std::accumulate(ticket_info.begin(), ticket_info.end(), 0ull, [&check](auto ticket_error_sum, auto v){
      return ticket_error_sum + (check(v) ? 0 : v);
    });
    return sum + ticket_error;
  });

  return {sum};
}

template <typename ForwardIterator>
void backtrack_exclusion(ForwardIterator begin, ForwardIterator end){
  if (begin == end)
    return;

  auto v = *(begin->second.begin());
  for (auto it = begin + 1; it != end; ++it){
    it->second.erase(v);
  }

  backtrack_exclusion(begin + 1, end);
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  auto &restriction = v.front().restriction;
  std::vector<std::vector<long>> near;
  [[maybe_unused]] auto &self = v.front().ticket;

  auto check = [&restriction](long v){
    return check_restriction_set(restriction, v);
  };

  std::copy_if(v.front().nearby.begin(), v.front().nearby.end(), std::back_inserter(near), [&check](auto const &vec){
    return std::ranges::all_of(vec.begin(), vec.end(), [&check](auto const &e){
      return check(e);
    });
  });

  near.emplace_back(self);

  std::vector<std::pair<unsigned long, std::unordered_set<std::string>>> fields_name(self.size());

  for (auto i = 0u; i < self.size(); ++i){
    fields_name[i].first = i;

    for (auto const &[checker_name, checker] : restriction) {
      auto is_ok = true;
      for (auto const &ticket : near) {
        if (!checker(ticket[i])) {
          is_ok = false;
          break;
        }
      }
      if (is_ok) {
        fields_name[i].second.insert(checker_name);
      }
    }
  }

  std::sort(fields_name.begin(), fields_name.end(), [](auto const &lhs, auto const &rhs){
    return lhs.second.size() < rhs.second.size();
  });

  backtrack_exclusion(fields_name.begin(), fields_name.end());

  auto sum = 1ull;
  for (auto const &fset : fields_name){
    if ((*(fset.second.begin())).find("departure") != std::string::npos){
      sum *= self[fset.first];
    }
  }

  return {sum};
}

#include "exec.hpp"