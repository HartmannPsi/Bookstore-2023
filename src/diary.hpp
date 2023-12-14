#pragma once
#ifndef DIARY_HPP
#define DIARY_HPP

class Finance;
class Alteration;
template <class T> class Report;

//#include "account.hpp"
#include "block_list.hpp"
#include "headfiles.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
class AccountSys;
extern AccountSys accounts;

using ul = unsigned long;

class Finance {
private:
  std::string file_name = "";
  std::fstream file;

public:
  Finance() = default;

  ~Finance();

  Finance(const std::string &name);

  void write(const double &val);

  void finance(const int &count = -1);
};

class WorkerLog {
public:
  char id[31] = "";
  char text[1 << 10] = "";
  bool status = true;

  WorkerLog() = default;

  ~WorkerLog() = default;

  WorkerLog(const std::string &id_, std::string &text_, bool status_);

  friend std::ostream &operator<<(std::ostream &op, const WorkerLog &rhs);
};

class Alteration {
public:
  char id[31] = "";
  bool type = false;
  char isbn[21] = "";
  int count = 0;
  double sum = 0.0;

  Alteration() = default;

  Alteration(const std::string &id_, const std::string &isbn_, bool type_,
             int count_, double sum_);

  ~Alteration() = default;

  friend std::ostream &operator<<(std::ostream &op, const Alteration &rhs);
};

template <class T> class Report {
private:
  std::string file_name = "";
  std::fstream file;

public:
  Report(const std::string &name) : file_name(name) {
    file.open(file_name);

    if (!file.is_open()) {

      file.open(file_name, std::ios::out);

      file.close();
      file.open(file_name);
    }

    char buffer[MAX];
    file.rdbuf()->pubsetbuf(buffer, sizeof(buffer));
  }

  ~Report() {
    file.close();
#ifdef DEBUG
    remove(file_name.c_str());
#endif
  }

  void write(const T &val) {
    file.seekp(0, std::ios::end);
    file.write(reinterpret_cast<const char *>(&val), sizeof(T));
  }

  void execute();
};

#endif