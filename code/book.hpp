#pragma once
#ifndef BOOK_HPP
#define BOOK_HPP

#include "account.hpp"
#include "block_list.hpp"
#include <cstring>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>

class AccountSys;
class LogStack;

extern AccountSys accounts;

class Book {
public:
  char isbn[21] = "";
  char name[61] = "";
  char author[61] = "";
  char keyword[61] = "";
  int quantity = 0;
  double price = 0.0;
  double total_cost = 0.0;

  Book(const std::string &isbn_, const std::string &name_,
       const std::string &author_, const std::string &keyword_,
       const int &quantity_ = 0, const double &price_ = 0.0,
       const double &total_cost_ = 0.0);

  Book(const std::string &isbn_);

  Book() = default;

  ~Book() = default;

  bool operator>(const Book &rhs) const;

  bool operator<(const Book &rhs) const;

  bool operator>=(const Book &rhs) const;

  bool operator<=(const Book &rhs) const;

  bool operator==(const Book &rhs) const;

  Book operator=(const Book &rhs);

  friend std::ostream &operator<<(std::ostream &op, const Book &rhs);
};

class BookSys {
private:
  List<Book> database_isbn;
  MultiList index_name;
  MultiList index_author;
  MultiList index_keyword;
  // Book slct_book;

public:
  BookSys();

  ~BookSys() = default;

  void show(const int &type, const std::string &str = "");

  void buy(const std::string &isbn, const int &quantity);

  void select(const std::string &isbn);

  void modify(const std::string &isbn, const std::string &name,
              const std::string &author, const std::string &keyword,
              double price);

  void import(const int &quantity, const double &total_cost);

  void select_clear();

  Book &select_book();

  void print_id_name();

  void print_id_author();

  void print_id_keyword();

  /*
switch (type) {
  case 0: all
  case 1: isbn
  case 2: name
  case 3: author
  case 4: keyword
  default: error
}
*/
};

#endif