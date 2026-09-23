#pragma once

#include <string>
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
