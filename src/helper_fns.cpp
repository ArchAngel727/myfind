#include "../headers/helper_fns.hpp"
#include <algorithm>
#include <optional>
#include <print>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void to_lower(std::string &string) {
  std::transform(
      string.begin(), string.end(), string.begin(),
      [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
}

std::optional<fs::path> get_absolute(const fs::path &path) {
  std::error_code ec;
  const fs::path abs_path = fs::absolute(path, ec).lexically_normal();

  if (ec) {
    std::println(stderr, "Could not get absolute path {}: {}", path.string(),
                 ec.message());

    return std::nullopt;
  }

  return abs_path;
}

void print(pid_t pid, const std::string &file, const fs::path &abs_path) {
  std::println("{}: {}: {}", pid, file, abs_path.string());
}

bool check_dir(const std::string &search_path) {
  std::error_code ec;
  const bool is_dir = fs::is_directory(search_path, ec);

  if (ec) {
    std::println(stderr, "Cannot access {}: {}", search_path, ec.message());
    return false;
  }

  if (!is_dir) {
    std::println(stderr, "Invalid path: {}", search_path);
    return false;
  }

  return true;
}
