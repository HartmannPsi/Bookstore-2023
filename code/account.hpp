#pragma once
#include <ostream>
#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include "block_list.hpp"
#include <cstring>
#include <stack>
#include <string>

enum Level { visitor = 0, customer = 1, assistant = 3, admin = 7 };

class Account {
public:
  char name[31] = "";
  char id[31] = "";
  char password[31] = "";
  Level prev = visitor;

  Account(const std::string &name_, const std::string &id_,
          const std::string &password_, const Level &prev_);

  Account(const std::string &id_, const Level &prev_ = visitor);

  Account() = default;

  ~Account() = default;

  bool operator>(const Account &rhs) const;

  bool operator<(const Account &rhs) const;

  bool operator>=(const Account &rhs) const;

  bool operator<=(const Account &rhs) const;

  bool operator==(const Account &rhs) const;

  Account operator=(const Account &rhs);

  friend std::ostream &operator<<(std::ostream &op, const Account &rhs);
};

class AccountSys {
private:
  List<Account> database;
  std::stack<Account> log_stack;

public:
  AccountSys();

  ~AccountSys() = default;

  void login(const std::string &id,
             const std::string &password = ""); // prev = 0

  void logout(); // prev = 1

  void regin(const std::string &id, const std::string &password,
             const std::string &name); // prev = 0

  void passwd(const std::string &id, const std::string &new_passwd,
              const std::string &current_passwd = ""); // prev = 1

  void useradd(const std::string &id, const std::string &password,
               const Level &prev,
               const std::string &name); // prev = 3

  void erase(const std::string id); // prev = 7

  Level telllvl() const; // prev = 0

  Account tellacc() const;

  void print();
};

#endif