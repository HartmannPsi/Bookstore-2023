#pragma once
#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "account.hpp"
#include <iostream>
#include <string>

extern AccountSys accounts;

class Command {
private:
  std::string text = "";

public:
  Command(const std::string &str);

  Command() = default;

  ~Command() = default;

  void execute();
};

class CommandSys {
private:
  Command latest_command;

public:
  CommandSys() = default;

  ~CommandSys() = default;

  void read(const std::string text);

  void execute();
};

#endif