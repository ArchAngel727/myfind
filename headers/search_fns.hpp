#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

void search_dir_for_file(std::string file,
                         std::vector<fs::directory_entry> &dirs,
                         bool case_sensitive);

void search_for_dirs(const std::string &root,
                     std::vector<fs::directory_entry> &dirs);
