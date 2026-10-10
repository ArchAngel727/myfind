#include "../headers/search_fns.hpp"
#include "../headers/helper_fns.hpp"
#include <filesystem>
#include <optional>
#include <print>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

void search_dir_for_file(std::string file,
                         std::vector<fs::directory_entry> &dirs,
                         bool case_sensitive, SharedSemaphore &output_lock) {
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

            print(output_lock, pid, file_name, *abs_path);
          }
        } else {
          std::string file_lowercase = file;
          const std::optional<fs::path> abs_path = get_absolute(entry.path());

          to_lower(file_name);
          to_lower(file_lowercase);

          if (file_name == file_lowercase) {
            print(output_lock, pid, file_name, *abs_path);
          }
        }
      }
    } catch (const fs::filesystem_error &e) {
      auto path = e.path1();
      const auto abs_path = get_absolute(e.path1());

      if (abs_path) {
        path = abs_path->string();
      }

      std::println(stderr, "Cannot read {}: {}", path.string(),
                   e.code().message());
    }
  }
}

void search_for_dirs(const std::string &root,
                     std::vector<fs::directory_entry> &dirs) {
  for (const auto &entry : fs::recursive_directory_iterator(
           root, fs::directory_options::skip_permission_denied)) {
    if (entry.is_directory()) {
      dirs.push_back(entry);
    }
  }
}
