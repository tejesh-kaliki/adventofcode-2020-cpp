#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

enum class Dir { Forward, Down, Up };

struct Command {
  Dir dir;
  int steps;
};

std::vector<Command> parseInput(std::istream& in) {
  std::vector<Command> commands;
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty()) continue;

    std::istringstream iss(line);
    std::string word;
    int steps;
    if (!(iss >> word >> steps)) throw std::runtime_error("Bad line: " + line);

    Dir dir;
    if (word == "forward") dir = Dir::Forward;
    else if (word == "up") dir = Dir::Up;
    else if (word == "down") dir = Dir::Down;
    else throw std::runtime_error("Bad command: " + word);

    commands.push_back({dir, steps});
  }

  return commands;
}

int solvePart1(const std::vector<Command>& input) {
  int x = 0, y = 0;

  for (const auto& c : input) {
    switch (c.dir) {
      case Dir::Forward: x += c.steps; break;
      case Dir::Down: y += c.steps; break;
      case Dir::Up: y -= c.steps; break;
    }
  }

  return x * y;
}

int solvePart2(const std::vector<Command>& input) {
  int x = 0, y = 0, aim = 0;

  for (const auto& c : input) {
    switch (c.dir) {
      case Dir::Forward: x += c.steps; y += aim * c.steps; break;
      case Dir::Down: aim += c.steps; break;
      case Dir::Up: aim -= c.steps; break;
    }
  }

  return x * y;
}

int main(int argc, char** argv) {
  std::string fileName = argc > 1 ? argv[1] : "inputs/day02.txt";
  std::ifstream file(fileName);
  if (!file) {
    std::cerr << "Could not open file " << fileName << "\n";
    return 1;
  }

  std::vector<Command> input = parseInput(file);

  int part1Ans = solvePart1(input);
  int part2Ans = solvePart2(input);

  std::cout << "Part 1 answer: " << part1Ans << "\n";
  std::cout << "Part 2 answer: " << part2Ans << "\n";
}
