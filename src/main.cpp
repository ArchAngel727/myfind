#include "../headers/app.hpp"
#include "../headers/parsers.hpp"
#include <filesystem>
#include <print>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

void print_app(App *app) {
  std::println("Flags: {} {}", app->flags.r_flag, app->flags.i_flag);
  std::println("Search path: {}", app->search_path);
  std::println("File count: {}", app->files.size());

  std::print("\nFiles:");
  for (const auto &file : app->files)
    std::print(" {}", file);
  std::print("\n");
}

void read_dir(App *app, std::vector<std::filesystem::directory_entry> *dirs) {
  pid_t pid = getpid();
  auto dir = std::move(dirs->back());
  dirs->pop_back();

  for (const auto &entry : std::filesystem::directory_iterator(dir)) {
    std::string file_name = entry.path().filename().string();

    if (entry.is_directory() && app->flags.r_flag) {
      dirs->push_back(entry);
    }

    if (!entry.is_directory()) {
      auto it = std::find(app->files.begin(), app->files.end(), file_name);
      if (it != app->files.end())
        std::println("{}: {}: {}/{}", pid, *it, dir.path().string(), *it);
    }
  }
}

int main(int argc, char *argv[]) {
  App app = {.flags =
                 {
                     .r_flag = false,
                     .i_flag = false,
                 },
             .search_path = std::string(),
             .files = std::vector<std::string>()};

  if (argc < 3) {
    std::println(
        "myfind [-R] [-i] searchpath filename1 [filename2] [filenameN]");
    return 0;
  }

  parse_flags(argc, argv, &app);
  parse_parameters(argc, argv, &app);

  // TODO: handle error_code
  if (!std::filesystem::is_directory(app.search_path)) {
    std::println("Invalid path: {}", app.search_path);
    return -1;
  }

  std::vector<std::filesystem::directory_entry> dirs;
  dirs.push_back(std::filesystem::directory_entry(app.search_path));

  // TODO: Check if dir is valid
  if (app.flags.r_flag) {
    while (!dirs.empty()) {
      read_dir(&app, &dirs);
    }
  }

  return 0;
}
