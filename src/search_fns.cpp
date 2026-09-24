#include "../headers/search_fns.hpp"
#include "../headers/helper_fns.hpp"
#include "../headers/stdout_lock.hpp"
#include <filesystem>
#include <mutex>
#include <optional>
#include <print>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

void search_dir_for_file(std::string file,
                         std::vector<fs::directory_entry> &dirs,
                         bool case_sensitive) {
  pid_t pid = getpid();

  for (const auto &dir : dirs) {
    try {
      for (const auto &entry : fs::directory_iterator(dir)) {
        std::string file_name = entry.path().filename().string();

        if (entry.is_directory()) {
          continue;
        }

        if (case_sensitive) {
          if (file_name == file) {
            const std::optional<fs::path> abs_path = get_absolute(entry.path());

            if (!abs_path) {
              // NOTE: Failed to get abs path
              return;
            }

            std::scoped_lock lock(stdout_mutex);
            print(pid, file_name, *abs_path);
          }
        } else {
          std::string file_lowercase = file;
          const std::optional<fs::path> abs_path = get_absolute(entry.path());

          to_lower(file_name);
          to_lower(file_lowercase);

          if (file_name == file_lowercase) {
            std::scoped_lock lock(stdout_mutex);
            print(pid, file_name, *abs_path);
          }
        }
      }
    } catch (const fs::filesystem_error &e) {
      std::println(stderr, "Cannot read {}: {}", e.path1().string(),
                   e.code().message());
    }
  }
}

void search_for_dirs(const std::string &root,
                     std::vector<fs::directory_entry> &dirs) {
  for (const auto &entry : fs::recursive_directory_iterator(root)) {
    if (entry.is_directory()) {
      dirs.push_back(entry);
    }
  }
}
