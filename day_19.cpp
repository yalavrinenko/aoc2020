//
// Created by yalavrinenko on 02.12.2020.
//
#define _GLIBCXX_REGEX_STATE_LIMIT 1000000
#include "main.hpp"
#include <regex>

struct AOC_Output {
  long value;
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::vector<std::string> rules;
  std::vector<std::string> message;
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  static AOC_Input in;
  static int stage = 0;
  if (line.empty()) {
    if (stage == 1) {
      stage = 0;
      auto copy = in;
      in = AOC_Input();
      return copy;
    }
    stage = 1;
    return {};
  }

  if (stage == 0) {
    in.rules.emplace_back(line);
    return {};
  }

  if (stage == 1) {
    in.message.emplace_back(line);
    return {};
  }

  return {};
}

class MatchExpressions {
public:
  struct match_rule {
    std::vector<int> branch_a;
    std::vector<int> branch_b;
    std::string final;
  };

  explicit MatchExpressions(std::vector<std::string> const &rules) { process_rules(rules); }

  std::string get_regexp(int rule_id) {
    //if (!rules_[rule_id].final.empty()) return "(" + rules_[rule_id].final + ")";
    if (!rules_[rule_id].final.empty()) return rules_[rule_id].final;

    std::string expr = "(";
    auto create_expr = [&expr, this](auto const &v) {
      for (auto rid : v) expr += get_regexp(rid);
    };

    create_expr(rules_[rule_id].branch_a);
    if (!rules_[rule_id].branch_b.empty()) {
      expr += "|";
      create_expr(rules_[rule_id].branch_b);
    }

    expr += ")";
    return expr;
  }

  std::string get_regexp_depthed(int rule_id, long max_depth) {
    if (depths[rule_id] >= max_depth) return {};

    if (!rules_[rule_id].final.empty()) return rules_[rule_id].final;

    std::string expr;
    auto create_expr = [&expr, this](auto const &v) {
      for (auto rid : v) {
        ++depths[rid];
        expr += get_regexp(rid);
        --depths[rid];
      }
    };

    create_expr(rules_[rule_id].branch_a);
    if (!rules_[rule_id].branch_b.empty()) {
      expr += "|";
      create_expr(rules_[rule_id].branch_b);
    }

    //expr += ")";
    return expr;
  }

  auto &rules() { return rules_; }

private:
  void process_rules(std::vector<std::string> const &rules) {
    for (auto &line : rules) {
      std::istringstream iss{line};
      long rule_id;
      iss >> rule_id;
      iss.ignore();
      match_rule rule;

      if (line.find('\"') == std::string::npos) {

        auto read_ref_rules = [&iss](auto &branch) {
          int ref;
          while (iss >> ref) branch.emplace_back(ref);
        };

        read_ref_rules(rule.branch_a);
        if (line.find('|') != std::basic_string<char, std::char_traits<char>, std::allocator<char>>::npos) {
          iss = std::istringstream(line.substr(line.find('|') + 1));
          read_ref_rules(rule.branch_b);
        }
      } else {
        iss >> rule.final;
        rule.final.erase(std::remove(rule.final.begin(), rule.final.end(), '\"'), rule.final.end());
      }

      rules_.emplace(std::pair{rule_id, std::move(rule)});
    }
  }

  std::unordered_map<long, match_rule> rules_;
  std::unordered_map<long, long> depths;
};

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  MatchExpressions matcher(v.front().rules);

  auto regexp = matcher.get_regexp(0);
  std::regex expr{regexp};
  auto count = std::count_if(v.front().message.begin(), v.front().message.end(),
                             [&expr](auto const &message) { return std::regex_match(message, expr); });
  return {count};
}

bool match_to_death(auto &matcher, std::string const &s, long times){
  auto rule_42 = matcher.get_regexp(42);
  auto rule_31 = matcher.get_regexp(31);

//  auto k = std::to_string(times);
//  std::string rule = "^("+rule_42+")+("+rule_42+"){"+k+"}("+rule_31+"){"+k+"}";
//  std::regex expr {rule};
//
//  return std::regex_match(s, expr);

  std::regex expr_42{"^" + rule_42};
  std::regex expr_31{"^" + rule_31};

  auto check_expr = [](auto &rexpr, auto str) -> std::optional<std::string>{
    std::smatch base_match;
    if (std::regex_search(str, base_match, rexpr)) {
      for (auto sub_match : base_match) {
        if (sub_match.first == str.begin()) {
          return {str.substr(sub_match.str().size())};
        }
      }
      return {};
    } else {
      return {};
    }
  };

  auto check_reduce = [&check_expr](auto rexpr, auto str){
    auto it = 0;
    while (!str.empty()){
      auto check_r = check_expr(rexpr, str);
      if (check_r){
        str = check_r.value();
        ++it;
      } else {
        break;
      }
    }
    return std::pair{str, it};
  };

  auto [str, it_42] = check_reduce(expr_42, s);
  auto [fstr, it_31] = check_reduce(expr_31, str);

  return it_31 != 0 && fstr.empty() && it_31 < it_42;
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  MatchExpressions matcher(v.front().rules);

  matcher.rules()[8] = {{42}, {42, 8}, ""};
  matcher.rules()[11] = {{42, 31}, {42, 11, 31}, ""};


  //for (auto k = 1; k < 20; ++k) {
    auto count = std::count_if(v.front().message.begin(), v.front().message.end(), [&matcher](auto const &message) {
      auto v = match_to_death(matcher, message, 0);
      return v;
    });
//

  return {count};
}

#include "exec.hpp"