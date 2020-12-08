//
// Created by yalavrinenko on 02.12.2020.
//
#include "AOC_Computer.hpp"
#include "main.hpp"
#include <set>
#include <unordered_set>

struct AOC_Output {
  long value;
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::string instruction;
  explicit AOC_Input(std::string s) : instruction{std::move(s)} {}

  [[nodiscard]] auto const &to_string() const { return instruction; }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) { return AOC_Input(line); }

auto examine_program(std::vector<AOC_Input> const &v){
  AOC_Computer computer{v};
  std::unordered_set<size_t> passed_instruction{0};
  auto ip = computer.step();
  while (ip.state == AOC_Computer::AOCC_State::State::OK && !passed_instruction.contains(ip.ip)){
    passed_instruction.insert(ip.ip);
    ip = computer.step();
  }

  return std::pair{ip.state == AOC_Computer::AOCC_State::State::STOP, computer.computer_state()};
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  return {examine_program(v).second.eax};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto code = v;
  for (auto &line : code){
    auto line_copy = line.instruction;

    bool rerun = false;

    if (line.instruction.find("jmp") != std::string::npos){
      line.instruction.replace(line.instruction.find("jmp"), 3, "nop");
      rerun = true;
    } else if (line.instruction.find("nop") != std::string::npos){
      line.instruction.replace(line.instruction.find("nop"), 3, "jmp");
      rerun = true;
    }

    if (rerun) {
      auto wstatus = examine_program(code);
      if (wstatus.first) return {wstatus.second.eax};
      line.instruction = line_copy;
    }
  }

  return {0};
}

#include "exec.hpp"