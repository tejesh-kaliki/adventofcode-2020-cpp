#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility>

struct Input {
  std::vector<int> nums;
  int lineSize;
};

Input parseInput(std::istream& in) {
  std::vector<int> nums;
  std::string line;
  int lineSize = 0;
  while (std::getline(in, line)) {
    if (line.empty()) continue;

    lineSize = line.size(); // Input condition is all lines are same size. Optimize this out of loop later
    int num = 0;
    for (int i = 0; i < lineSize; i++) {
      num |= (line[i] == '1' ? 1 : 0) << (lineSize - i - 1);
    }
    nums.push_back(num);
    
    // std::cout << num << "\n";
  }

  return {std::move(nums), lineSize};
}

bool mostUsedBitAtPos(const std::vector<int>& nums, int index, bool tieBreak) {
  int count = 0;
  for (const auto& num : nums) {
    count += (num >> index) & 1;
  }

  if (nums.size() % 2 == 0 && count == (int) nums.size() / 2) return tieBreak;
  if (count > (int) nums.size() / 2) return true;
  return false;
}

int solvePart1(const Input& input) {
  int gamma = 0, epsilon = 0;
  for (int i = 0; i < input.lineSize; i++) {
    if (mostUsedBitAtPos(input.nums, i, /*tieBreak=*/false)) gamma |= 1<<i;
    else epsilon |= 1 << i;
  }

  return gamma * epsilon;
}

int rating(std::vector<int> pool, int bits, bool keepMostCommon) {
  for (int i = bits - 1; i >= 0 && pool.size() > 1; i--) {
    bool most = mostUsedBitAtPos(pool, i, /*tieBreak=*/true);
    int keep = keepMostCommon ? most : !most;
    std::erase_if(pool, [&](int n) { return ((n >> i) & 1) != keep; });
  }
  return pool[0];
}

int solvePart2(const Input& input) {
  int oxyRating = rating(input.nums, input.lineSize, true);
  int co2Rating = rating(input.nums, input.lineSize, false);
  return oxyRating * co2Rating;
}

int main(int argc, char** argv) {
  std::string fileName = argc > 1 ? argv[1] : "inputs/day03.txt";
  std::ifstream file(fileName);
  if (!file) {
    std::cerr << "Could not open file " << fileName << "\n";
    return 1;
  }

  Input input = parseInput(file);
  if (input.nums.size() == 0) {
    std::cout << "Input is empty\n";
    return 1;
  }

  int part1Ans = solvePart1(input);
  int part2Ans = solvePart2(input);

  std::cout << "Part 1 answer: " << part1Ans << "\n";
  std::cout << "Part 2 answer: " << part2Ans << "\n";
}
