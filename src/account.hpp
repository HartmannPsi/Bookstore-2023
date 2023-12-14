#pragma once
#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include "block_list.hpp"
//#include "book.hpp"
#include "diary.hpp"
#include <cstring>
#include <ostream>
#include <stack>
#include <string>
#include <vector>

class BookSys;
class Book;
// class AccountSys;

extern BookSys books;

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

class LogStack {
public:
  std::vector<Account> acc_stack;
  std::vector<Book> book_stack;

  LogStack() = default;

  ~LogStack() = default;

  void push(const Account &acc);

  void select(const Book &book);

  bool check_acc(const Account &acc) const;

  void pop();

  Account top() const;

  int size() const;

  Book &slct_book();

  void modify(const Book &val_old, const Book &val_new);
};

class AccountSys {
private:
  List<Account> database;
  LogStack log_stack;

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

  Book &slct_book();

  void modify(const Book &val_old, const Book &val_new);

  void print();
};

#endif