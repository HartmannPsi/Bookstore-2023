#pragma once
#ifndef DIARY_HPP
#define DIARY_HPP

class Linear;

#include "account.hpp"
#include "block_list.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

class AccountSys;

extern AccountSys accounts;

using ul = unsigned long;

class Linear {
private:
  std::string file_name = "";
  std::fstream file;

public:
  Linear() = default;

  ~Linear();

  Linear(const std::string &name);

  void write(const double &val);

  void finance(const int &count = -1);
};

#endif