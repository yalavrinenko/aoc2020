//
// Created by yalavrinenko on 02.12.2020.
//
#include "main.hpp"
#include <stack>

struct AOC_Output{
  long value;
  auto operator() () const { return value; }
};

struct AOC_Input{
  std::string expression;
  explicit AOC_Input(std::string s): expression{std::move(s)}{
    expression.erase(std::remove(expression.begin(), expression.end(), ' '), expression.end());
  }
};

class ReversePoland{
public:
  ReversePoland(std::string const &line, std::unordered_map<char, int> const& prioryty){
    std::stack<char> s;
    for (auto const &c: line){
      if (c == '(') s.push(c);
      else if (c == ')') {
        while (s.top() != '('){
          expression_ += s.top(); s.pop();
        }
        s.pop();
      }
      else if (c == '+' || c == '*') {
        while (!s.empty() && (prioryty.contains(s.top()) && prioryty.at(c) <= prioryty.at(s.top()))){
          expression_ += s.top(); s.pop();
        }
        s.push(c);
      }
      else
        expression_ += c;
    }
    for (; !s.empty(); s.pop())
      expression_ += s.top();
  }

  long evaluate() const {
    std::stack<long> s;
    for (auto const &c : expression_){
      if ('0' <= c && c <= '9')
        s.push(c - '0');
      else if (c == '+' || c == '*'){
        auto arg1 = s.top(); s.pop();
        auto arg2 = s.top(); s.pop();
        s.push((c == '+') ? arg1 + arg2 : arg1 * arg2);
      }
    }

    return s.top();
  }

private:
  std::string expression_;
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  std::unordered_map priority{std::pair{'+', 0}, {'*', 0}};
  auto sum = std::accumulate(v.begin(), v.end(), 0ll, [&priority](auto sum, auto const& l){
    return sum + ReversePoland(l.expression, priority).evaluate();
  });

  return {sum};
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  std::unordered_map priority{std::pair{'+', 1}, {'*', 0}};
  auto sum = std::accumulate(v.begin(), v.end(), 0ll, [&priority](auto sum, auto const& l){
    return sum + ReversePoland(l.expression, priority).evaluate();
  });

  return {sum};
}

#include "exec.hpp"