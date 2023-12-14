#include "diary.hpp"
#include <iomanip>

Linear::~Linear() {
  file.close();
#ifdef DEBUG
  remove(file_name.c_str());
#endif
}

Linear::Linear(const std::string &name) : file_name(name) {

  file.open(file_name);

  if (!file.is_open()) {

    file.open(file_name, std::ios::out);

    file.close();
    file.open(file_name);
  }

  char buffer[MAX];
  file.rdbuf()->pubsetbuf(buffer, sizeof(buffer));
}

void Linear::write(const double &val) {
  file.seekp(0, std::ios::end);
  file.write(reinterpret_cast<const char *>(&val), sizeof(double));
}

void Linear::finance(const int &count) {

  if (accounts.telllvl() < 7) {
    throw 0;
    return;
  }

  if (count == 0) {
    std::cout << '\n';
    return;
  }

  double plus = 0, minus = 0;
  file.seekg(0, std::ios::end);
  const ul end = file.tellg();

  if (count > static_cast<int>(end / sizeof(double))) {
    throw 0;
    return;
  }

  if (count == -1) { // all

    ul pos = 0;
    while (pos < end) {
      double num;
      file.seekg(pos);
      file.read(reinterpret_cast<char *>(&num), sizeof(double));
      if (num > 0.0) {
        plus += num;
      } else {
        minus -= num;
      }
      pos += sizeof(double);
    }
  } else { // count

    for (int i = 1; i <= count; ++i) {
      file.seekg(-i * sizeof(double), std::ios::end);
      double num;
      file.read(reinterpret_cast<char *>(&num), sizeof(double));
      if (num > 0.0) {
        plus += num;
      } else {
        minus -= num;
      }
    }
  }

  std::cout << "+ " << std::fixed << std::setprecision(2) << plus << " - "
            << std::fixed << std::setprecision(2) << minus << '\n';
}