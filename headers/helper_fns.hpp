#pragma once

#include "./shared_semaphore.hpp"
#include <filesystem>
#include <optional>
#include <semaphore.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace fs = std::filesystem;

void to_lower(std::string &string);
std::optional<fs::path> get_absolute(const fs::path &path);
void print(SharedSemaphore &output_lock, pid_t pid, const std::string &file,
           const fs::path &abs_path);
bool check_dir(const std::string &search_path);
