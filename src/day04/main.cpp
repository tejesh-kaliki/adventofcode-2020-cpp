#include <iostream>
#include <format>
#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include <array>
#include <stdexcept>
#include <sstream>
#include <bitset>
#include <utility>

using namespace std;

using Grid = array<array<int, 5>, 5>;

struct Input {
  vector<int> nums;
  vector<Grid> grids;
};

Input parseInput(istream& in) {
  string line;

  // First line is list of numbers split by ','
  if (!getline(in, line)) throw runtime_error("File is empty");
  istringstream iss(line);
  vector<int> nums;
  string tok;
  while (getline(iss, tok, ',')) {
    nums.push_back(stoi(tok));
  }

  vector<Grid> grids;
  while (getline(in, line)) {
    if (!line.empty()) throw runtime_error("Must read a empty line (\\n\\n) before each grid");

    if (!getline(in, line) || line.empty()) break; // likely input end, otherwise it has the first row

    Grid grid;
    // Read 5 rows for grid
    for (int i = 0; i < 5; i++) {
      // First row is already read, otherwise make sure the row <i> exists
      if (i > 0 && (!getline(in, line) || line.empty())) throw runtime_error(format("Only managed to read {} rows", i));

      istringstream iss(line);
      int buf;
      for (int j = 0; j < 5; j++) {
        if (!(iss >> buf)) throw runtime_error(format("Only managed to read {} nums in row {}", j, i + 1));

        grid[i][j] = buf;
      }
    }

    grids.push_back(std::move(grid));
  }

  return {std::move(nums), std::move(grids)};
}

void crossNum(const Grid& grid, int num, bitset<25>& crossed) {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      if (grid[i][j] == num) {
        crossed.set(i * 5 + j);
        return;
      }
    }
  }
}

bool gridComplete(const bitset<25>& crossed) {
  bitset<25> rowMask("0000000000000000000011111");
  for (int i = 0; i < 5; i++) {
    if ((crossed & rowMask) == rowMask) return true;
    rowMask <<= 5;
  }

  bitset<25> colMask("0000100001000010000100001");
  for (int i = 0; i < 5; i++) {
    if ((crossed & colMask) == colMask) return true;
    colMask <<= 1;
  }

  return false;
}

int gridScore(const Grid& grid, const bitset<25>& crossed) {
  int sum = 0;
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      if (!crossed.test(i * 5 + j)) sum += grid[i][j];
    }
  }
  return sum;
}

int solvePart1(const Input& input) {
  vector<bitset<25>> crossedVec(input.grids.size());
  for (int num : input.nums) {
    for (size_t i = 0; i < input.grids.size(); i++) {
      auto& grid = input.grids[i];
      auto& crossed = crossedVec[i];
      crossNum(grid, num, crossed);
      if (gridComplete(crossed)) return num * gridScore(grid, crossed);
    }
  }
  throw runtime_error("No solution");
}

int solvePart2(const Input& input) {
  vector<bitset<25>> crossedVec(input.grids.size());
  vector<bool> completed(input.grids.size());
  size_t completedCount = 0;
  for (int num : input.nums) {
    for (size_t i = 0; i < input.grids.size(); i++) {
      if (completed[i]) continue;

      auto& grid = input.grids[i];
      auto& crossed = crossedVec[i];
      crossNum(grid, num, crossed);
      if (gridComplete(crossed)) {
        completed[i] = true;
        completedCount++;
      }

      if (completedCount == input.grids.size()) return num * gridScore(grid, crossed);
    }
  }
  throw runtime_error("No solution");
}

int main(int argc, char** argv) {
  string fileName = argc > 1 ? argv[1] : "inputs/day04.txt";
  ifstream file(fileName);
  if (!file) {
    cerr << "Could not open file " << fileName << "\n";
    return 1;
  }

  Input input = parseInput(file);

  if (input.nums.empty() || input.grids.empty()) {
    cerr << "No numbers or grids present" << "\n";
    return 1;
  }

  cout << "Nums length: " << input.nums.size() << "\n";
  cout << "No of grids: " << input.grids.size() << "\n";

  int part1Ans = solvePart1(input);
  int part2Ans = solvePart2(input);

  cout << "Part 1 answer: " << part1Ans << "\n";
  cout << "Part 2 answer: " << part2Ans << "\n";
}
