//
// Created by yalavrinenko on 02.12.2020.
//

#ifndef AOC_2020_EXEC_HPP
#define AOC_2020_EXEC_HPP

std::vector<AOC_Input> AOC_ParceInput(std::string const &path) {
  std::ifstream in(path);
  std::string line;
  std::vector<AOC_Input> v;
  while (std::getline(in, line)){
    auto input_data = AOC_Reader::create_from_string(line);
    if (input_data)
      v.emplace_back(input_data.value());
  }
  return v;
}

int main(int argc, char** argv){
  std::cout << part_1(AOC_ParceInput(DAY_PATH))() << std::endl;
  std::cout << part_2(AOC_ParceInput(DAY_PATH))() << std::endl;
  return 0;
}


#endif//AOC_2020_EXEC_HPP
