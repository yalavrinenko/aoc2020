//
// Created by yalavrinenko on 08.12.2020.
//

#ifndef AOC_2020_AOC_COMPUTER_HPP
#define AOC_2020_AOC_COMPUTER_HPP
#include <functional>
#include <sstream>
#include <vector>

using instruction_function = std::function<void(void)>;

class AOCC_Instruction{
public:
  explicit AOCC_Instruction(instruction_function action): action_{std::move(action)}{
  }

  void execute() {
    action_();
  }

protected:
  instruction_function action_;
};

class AOC_Computer{
public:
  struct AOCC_Registers {
    long eax{0};
  };

  struct AOCC_State{
    enum class State{
      OK,
      STOP,
    };

    State state;
    size_t ip;
  };

  using InstructionLine = std::vector<AOCC_Instruction>;
  template<typename CodeLine>
  explicit AOC_Computer(std::vector<CodeLine> const& code): ip_{0}{
    instruction_.reserve(code.size());

    using AOCC_InstructionBuilder = std::function<AOCC_Instruction(std::istringstream&)>;

    std::unordered_map<std::string, AOCC_InstructionBuilder> builders;
    builders["nop"] = [this](auto &s){
      return AOCC_Instruction([this](){ this->ip_++; });
    };
    builders["jmp"] = [this](auto &s){
      long jump; s >> jump;
      return AOCC_Instruction([this, jump]() { this->ip_ += jump; });
    };
    builders["acc"] = [this](auto &s){
      long value; s >> value;
      return AOCC_Instruction([this, value]() { this->regs_.eax += value; this->ip_ ++; });
    };

    for (auto &line : code){
      std::istringstream iss{line.to_string()};
      std::string command; iss >> command;
      instruction_.emplace_back(builders[command](iss));
    }
  }

  AOCC_State step() {
    if (ip_ < instruction_.size()) {
      instruction_[ip_].execute();
      return {AOCC_State::State::OK, ip_};
    } else {
      return {AOCC_State::State::STOP, 0};
    }
  }

  auto const& computer_state() const {
    return regs_;
  }

private:
  size_t ip_;
  AOCC_Registers regs_;
  InstructionLine instruction_;
};

#endif//AOC_2020_AOC_COMPUTER_HPP
