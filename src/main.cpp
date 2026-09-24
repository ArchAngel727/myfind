#include "../headers/app.hpp"
#include "../headers/helper_fns.hpp"
#include "../headers/parsers.hpp"
#include "../headers/search_fns.hpp"
#include <cerrno>
#include <cstdio>
#include <filesystem>
#include <print>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
  App app = {.flags =
                 {
                     .r_flag = false,
                     .i_flag = false,
                 },
             .search_path = std::string(),
             .files = std::vector<std::string>()};

  // NOTE: if theres less than three args (aka no program, searchpath, filename)
  // just return
  if (argc < 3) {
    std::println(
        "myfind [-R] [-i] searchpath filename1 [filename2] [filenameN]");
    return 0;
  }

  parse_flags(argc, argv, &app);
  parse_parameters(argc, argv, &app);

  // NOTE: Checking if the given starting dir is a valid dir
  if (!check_dir(app.search_path)) {
    return -1;
  }

  std::vector<fs::directory_entry> dirs;
  dirs.push_back(fs::directory_entry(app.search_path));

  // NOTE: Recursivly searching for dirs
  if (app.flags.r_flag) {
    search_for_dirs(app.search_path, dirs);
  }

  std::vector<pid_t> children;

  // NOTE: Spawning children to search for files
  for (const std::string &file : app.files) {
    const pid_t pid = fork();

    if (pid == -1) {
      std::perror("fork");

      // NOTE: Failed to fork; waiting for children to finish
      for (const pid_t child_pid : children) {
        int status;

        while (waitpid(child_pid, &status, 0) == -1) {
          if (errno != EINTR) {
            break;
          }
        }
      }
    }

    if (pid == 0) {
      search_dir_for_file(file, dirs, app.flags.i_flag);

      _exit(0);
    }

    children.push_back(pid);
  }

  for (const pid_t child_pid : children) {
    int status = 0;
    pid_t result = 0;

    // NOTE: Waiting for the child to exit
    do {
      result = waitpid(child_pid, &status, 0);
    } while (result == -1 && errno == EINTR);

    // NOTE: Checking if waitpid failed for a reason other than EINTR
    if (result == -1) {
      std::perror("waitpid");
      continue;
    }
  }

  return 0;
}
