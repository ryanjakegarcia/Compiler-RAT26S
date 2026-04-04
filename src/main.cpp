#include "parser.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

std::vector<fs::path> collectRatFiles(const fs::path& directory) {
  std::vector<fs::path> files;

  if (!fs::exists(directory) || !fs::is_directory(directory)) {
    return files;
  }

  for (const auto& entry : fs::directory_iterator(directory)) {
    if (entry.is_regular_file() && entry.path().extension() == ".rat") {
      files.push_back(entry.path());
    }
  }

  std::sort(files.begin(), files.end());
  return files;
}

int main() {
  std::vector<fs::path> passFiles = collectRatFiles("pass");
  std::vector<fs::path> failFiles = collectRatFiles("fail");

  if (passFiles.empty() && failFiles.empty()) {
    std::cout << "No .rat files found in pass or fail.\n";
    return 1;
  }

  int passed = 0;
  int failed = 0;

  for (const auto& inputPath : passFiles) {
    fs::path outputPath = inputPath;
    outputPath += ".parse.txt";

    try {
      Lexer lexer(inputPath.string());
      Parser parser(lexer, outputPath.string());
      parser.parse();
      ++passed;
      std::cout << "[PASS] " << inputPath.string() << " parsed as expected -> " << outputPath.string() << "\n";
    } catch (const std::exception& e) {
      ++failed;
      std::cout << "[FAIL] " << inputPath.string() << " expected parse success\n";
      std::cout << "       " << e.what() << "\n";
    }
  }

  for (const auto& inputPath : failFiles) {
    fs::path outputPath = inputPath;
    outputPath += ".parse.txt";

    try {
      Lexer lexer(inputPath.string());
      Parser parser(lexer, outputPath.string());
      parser.parse();
      ++failed;
      std::cout << "[FAIL] " << inputPath.string() << " expected parse failure, but parsed successfully\n";
    } catch (const std::exception& e) {
      ++passed;
      std::cout << "[PASS] " << inputPath.string() << " failed as expected\n";
      std::cout << "       " << e.what() << "\n";
    }
  }

  std::cout << "\nSummary: " << passed << " passed, " << failed << " failed.\n";
  return failed;
}
