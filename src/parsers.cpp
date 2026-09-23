#include "../headers/app.hpp"
#include <unistd.h>

void parse_flags(int argc, char *argv[], App *app) {
  int option;

  while ((option = getopt(argc, argv, "Ri")) != -1) {
    switch (option) {
    case 'R':
      app->flags.r_flag = true;
      break;

    case 'i':
      app->flags.i_flag = true;
      break;
    }
  }
}

void parse_parameters(int argc, char *argv[], App *app) {
  // NOTE: Removing `./myfind` from the argument list by incrementing the
  // array pointer
  argc -= 1;
  argv += 1;

  for (int i = 0; i < argc; i++) {
    if (argv[i][0] != '-') {
      // NOTE: Getting the first non flag parameter (search_path)
      if (app->search_path.empty()) {
        app->search_path = argv[i];
        continue;
      }

      // NOTE: Getting the other parameters (files)
      app->files.push_back(argv[i]);
    }
  }
}
