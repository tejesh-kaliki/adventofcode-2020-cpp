#include <iostream>
#include <string>
#include <fstream>
#include <vector>

std::vector<int> getValues(std::istream& in) {
  std::vector<int> values;
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty()) continue;
    int num = std::stoi(line);
    values.push_back(num);
  }
  return values;
}

int solvePart1(const std::vector<int>& input) {
  int count = 0;
  for (size_t i = 1; i < input.size(); i++) {
    if (input[i] > input[i-1]) count++;
  }
  return count;
}

int solvePart2(const std::vector<int>& input) {
  if (input.size() < 4) return 0;

  int count = 0;
  for (size_t i = 3; i < input.size(); i++) {
    // int sum = previous - input[i-3] + input[i];
    // if (sum > previous) count++;
    //
    //   previous = <i-3> + <i-2> + <i-1>
    //   sum      = <i-2> + <i-1> + <i>
    //   sum > previous -> <i> > <i-3>
    if (input[i] > input[i-3]) count++;
  }

  return count;
}

int main(int argc, char** argv) {
  std::string fileName = argc > 1 ? argv[1] : "inputs/day01.txt";
  std::ifstream file(fileName);
  if (!file) {
    std::cerr << "Could not open file " << fileName << "\n";
    return 1;
  }

  std::vector<int> input = getValues(file);

  std::cout << "Part 1 answer: " << solvePart1(input) << "\n";
  std::cout << "Part 2 answer: " << solvePart2(input) << "\n";
}
