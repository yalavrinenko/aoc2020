//
// Created by yalavrinenko on 01.12.2020.
//
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <fstream>
auto part_one(auto v, auto target) {
  for (auto const &e: v){
    auto mult2 = std::lower_bound(v.begin(), v.end(), target - e);
    if (mult2 != v.end() && *mult2 == target  - e)
      return e * (*mult2);
  }
  return 0ll;
}

auto part_two(auto v, auto target) {
  for (auto const &e: v){
    auto double_prod = part_one(v, target - e);
    if (double_prod != 0)
      return double_prod * e;
  }
  return 0ll;
}

int main(int argc, char **argv) {
  std::ifstream in("../day_1.in");
  std::vector<long long> v;
  std::copy(std::istream_iterator<long long>{in}, std::istream_iterator<long long>{}, std::back_inserter(v));
  std::sort(v.begin(), v.end());
  std::cout << "\nPart 1: " << part_one(v, 2020) << std::endl;
  std::cout << "\nPart 2: " << part_two(v, 2020) << std::endl;
  return 0;
}
