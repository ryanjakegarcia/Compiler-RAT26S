#include "parser.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;
static const char* kColorRed = "\033[31m";
static const char* kColorRedBold = "\033[1;31m";
static const char* kColorGreen = "\033[32m";
static const char* kColorReset = "\033[0m";

std::string colorText(const std::string& message, const char* color){
  return std::string(color) + message + kColorReset;
}

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

int main(int argc, char** argv) {
  fs::path exePath = fs::absolute(argc > 0 ? fs::path(argv[0]) : fs::path());
  fs::path repoRoot = exePath.parent_path().parent_path();

  std::vector<fs::path> passFiles = collectRatFiles(repoRoot / "pass");
  std::vector<fs::path> failFiles = collectRatFiles(repoRoot / "fail");

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
      std::cout << colorText("[PASS] ", kColorGreen) << inputPath.string() << " parsed as expected -> " << outputPath.string() << "\n";
    } catch (const std::exception& e) {
      ++failed;
      std::cout << colorText("[FAIL] ", kColorRed) << inputPath.string() << " expected parse success\n";
      std::cout << "       " << colorText(e.what(), kColorRedBold) << "\n";
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
      std::cout << colorText("[FAIL] ", kColorRed) << inputPath.string() << " expected parse failure, but parsed successfully\n";
    } catch (const std::exception& e) {
      ++passed;
      std::cout << colorText("[PASS] ", kColorGreen) << inputPath.string() << " failed as expected\n";
      std::cout << "       " << colorText(e.what(), kColorRedBold) << "\n";
    }
  }

  std::cout << "\nSummary: " << passed << " passed, " << failed << " failed.\n";
  return failed;
}
