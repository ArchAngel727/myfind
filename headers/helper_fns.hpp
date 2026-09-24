#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace fs = std::filesystem;

void to_lower(std::string &string);
std::optional<fs::path> get_absolute(const fs::path &path);
void print(pid_t pid, const std::string &file, const fs::path &abs_path);
bool check_dir(const std::string &search_path);
