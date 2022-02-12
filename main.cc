#include <algorithm>
#include <cmath>
#include <execution>
#include <filesystem>
#include <iostream>
#include <numeric>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "checksum.h"

namespace fs = std::filesystem;

struct HumanReadable {
  std::uintmax_t size{};
};

std::ostream &operator<<(std::ostream &os, HumanReadable hr) {
  int i{};
  double mantissa = hr.size;
  for (; mantissa >= 1024.; mantissa /= 1024., ++i) {
  }
  mantissa = std::ceil(mantissa * 10.) / 10.;
  os << mantissa << "BKMGTPE"[i];
  return i == 0 ? os : os << "B (" << hr.size << ')';
}

struct DuplicatesRemover {
  fs::path path;
  std::uintmax_t total_removed{};
  std::unordered_map<std::uintmax_t, std::vector<fs::path>> m;
  std::set<std::uintmax_t, std::greater<std::uintmax_t>> sizes;
  int total;
  int max_files;

  DuplicatesRemover(const std::string &p)
      : path(p), total(0), max_files(1 << 20) {}

  DuplicatesRemover &set_max_files(int m) {
    max_files = m;
    return *this;
  }

  void run() {
    std::for_each(
        // std::execution::par, ?
        fs::recursive_directory_iterator(path),
        fs::recursive_directory_iterator(),
        [this](const fs::directory_entry &e) {
          if (!e.is_regular_file() || e.is_symlink()) return;
          if (total == max_files) return;
          sizes.insert(e.file_size());
          m[e.file_size()].push_back(e);
          ++total;
        });
    std::cout << "total of " << total << " files browsed" << std::endl;
    for (const auto &sz : sizes) {
      const auto &v = m[sz];
      if (v.size() == 1) continue;

      std::unordered_map<std::string, std::vector<fs::path>> mm;
      for (const auto &p : v) {
        mm[sha256(p)].push_back(p);
      }

      for (auto &[h, v] : mm) {
        int v_size = static_cast<int>(v.size());
        if (v_size == 1) {
          v.clear();
          continue;
        }
        std::cout << "Found duplicates" << std::endl;
        int i = 1;
        for (const auto &p : v) {
          std::cout << i++ << ")" << p << " " << HumanReadable{sz} << std::endl;
        }

        int index;
        do {
          std::cout
              << "Give index of the one to keep | 0 to keep all | q to quit"
              << std::endl;
          std::string s;
          std::cin >> s;
          if (s == "q") {
            std::cout << "Removed " << HumanReadable{total_removed}
                      << std::endl;
            std::cout << "Done" << std::endl;
            return;
          }
          index = std::stoi(s);
        } while (index < 0 || index > v_size);

        if (index > 0) {
          for (int a = 0; a < (int)v.size(); ++a) {
            if (a != index - 1) {
              total_removed += fs::file_size(v[a]);
              fs::remove(v[a]);
              std::cout << v[a] << " removed" << std::endl;
            }
          }
        } else {
          std::cout << "all files were kept" << std::endl;
        }
        v.clear();
      }
    }

    std::cout << "Removed " << HumanReadable{total_removed} << std::endl;
    std::cout << "Done" << std::endl;
  }
};

int main(int argc, const char *argv[]) {
  // TODO add a second arg for files filter
  DuplicatesRemover(argv[1]).run();
  return 0;
}
