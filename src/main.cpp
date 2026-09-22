#include <print>
#include <string>
#include <unistd.h>
#include <vector>

typedef struct {
  bool r_flag;
  bool i_flag;
} Flags;

typedef struct {
  Flags flags;
  std::string search_path;
  std::vector<std::string> files;
} App;

int main(int argc, char *argv[]) {
  App app = {.flags =
                 {
                     .r_flag = false,
                     .i_flag = false,
                 },
             .search_path = std::string(),
             .files = std::vector<std::string>()};

  std::println("argc: {}", argc);

  for (int i = 0; i < argc; i++) {
    std::println("[{}]: {}", i, argv[i]);
  }

  int option;
  while ((option = getopt(argc, argv, "Ri")) != -1) {
    switch (option) {
    case 'R':
      app.flags.r_flag = true;
      break;

    case 'i':
      app.flags.i_flag = true;
      break;

    case '?':
      std::println("Hellnah");
      return 0;
    }
  }

  // INFO:./myfind [-R] [-i] searchpath filename1 [filename2] [filenameN]
  // R - toggle recursive search on and search for all matches
  // i - toggle case sensitive off
  // searchpath - absolute or relative path
  // filename - plain string file name
  // no support for paths, subpaths, wildcards required.
  // Example: ./myfind ./ test.txt test.doc test

  return 0;
}
