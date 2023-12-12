#include "book.hpp"
#include <cstring>
#include <iomanip>

Book::Book(const std::string &isbn_, const std::string &name_,
           const std::string &author_, const std::string &keyword_,
           const int &quantity_, const double &price_,
           const double &total_cost_)
    : quantity(quantity_), price(price_), total_cost(total_cost_) {
  strcpy(isbn, isbn_.c_str());
  strcpy(name, name_.c_str());
  strcpy(author, author_.c_str());
  strcpy(keyword, keyword_.c_str());
}

Book::Book(const std::string &isbn_) { strcpy(isbn, isbn_.c_str()); }

bool Book::operator>(const Book &rhs) const {
  const int res = strcmp(isbn, rhs.isbn);
  return res > 0;
}

bool Book::operator<(const Book &rhs) const {
  const int res = strcmp(isbn, rhs.isbn);
  return res < 0;
}

bool Book::operator>=(const Book &rhs) const {
  const int res = strcmp(isbn, rhs.isbn);
  return res >= 0;
}

bool Book::operator<=(const Book &rhs) const {
  const int res = strcmp(isbn, rhs.isbn);
  return res <= 0;
}

bool Book::operator==(const Book &rhs) const {
  const int res = strcmp(isbn, rhs.isbn);
  return res == 0;
}

Book Book::operator=(const Book &rhs) {
  if (this == &rhs) {
    return *this;
  }

  strcpy(isbn, rhs.isbn);
  strcpy(name, rhs.name);
  strcpy(author, rhs.author);
  strcpy(keyword, rhs.keyword);
  quantity = rhs.quantity;
  price = rhs.price;
  total_cost = rhs.total_cost;

  return *this;
}

std::ostream &operator<<(std::ostream &op, const Book &rhs) {

  if (strlen(rhs.isbn) == 0) {
    return op;
  }
  /*
  op << "ISBN: " << rhs.isbn << " | name: " << rhs.name
     << " | author: " << rhs.author << " | keyword: \"" << rhs.keyword
     << "\" | price: " << rhs.price << " | quantity: " << rhs.quantity
     << " | total_cost: " << rhs.total_cost << '\n';
  */
  op << rhs.isbn << '\t' << rhs.name << '\t' << rhs.author << '\t'
     << rhs.keyword << '\t' << std::fixed << std::setprecision(2) << rhs.price
     << '\t' << rhs.quantity << '\n';
  return op;
}

BookSys::BookSys() : slct_book(), database_isbn("books_ISBN.dat") {}

void BookSys::show(const int &type, const std::string &str) {

  if (accounts.telllvl() < 1) {
    throw 0;
    return;
  }

  if (type == 0) {

    database_isbn.print();
  } else if (type == 1) {

    auto *const ans = database_isbn.get(Book(str));
    if (ans->empty()) {
      std::cout << '\n';
    } else {
      for (auto i = ans->cbegin(); i != ans->cend(); ++i) {
        std::cout << *i;
      }
    }
    delete ans;

  } else if (type == 2) {

  } else if (type == 3) {

  } else if (type == 4) {
  }
}

void BookSys::buy(const std::string &isbn, const int &quantity) {

  if (accounts.telllvl() < 1) {
    throw 0;
    return;
  }

  auto buy_book = database_isbn.find(Book(isbn));

  if (buy_book == Book() || buy_book.quantity < quantity || quantity <= 0) {
    throw 0;
    return;
  }

  database_isbn.erase(buy_book);
  buy_book.quantity -= quantity;
  std::cout << std::fixed << std::setprecision(2) << quantity * buy_book.price
            << '\n';
  database_isbn.insert(buy_book);
}

void BookSys::select(const std::string &isbn) {

  if (accounts.telllvl() < 3) {
    throw 0;
    return;
  }

  slct_book = database_isbn.find(Book(isbn));
  if (strlen(slct_book.isbn) == 0) {
    strcpy(slct_book.isbn, isbn.c_str());
    database_isbn.insert(slct_book);
  }
}

void BookSys::modify(const std::string &isbn, const std::string &name,
                     const std::string &author, const std::string &keyword,
                     double price) {

  if (accounts.telllvl() < 3) {
    throw 0;
    return;
  }

  if (strlen(slct_book.isbn) == 0) {
    throw 0;
    return;
  }

  const int res = strcmp(slct_book.isbn, isbn.c_str());

  if (res == 0) {
    throw 0;
    return;
  }

  if (isbn != "" && database_isbn.count(Book(isbn))) {
    throw 0;
    return;
  }

  database_isbn.erase(slct_book);

  if (isbn != "") {
    strcpy(slct_book.isbn, isbn.c_str());
  }
  if (name != "") {

    strcpy(slct_book.name, name.c_str());
  }
  if (author != "") {
    strcpy(slct_book.author, author.c_str());
  }
  if (keyword != "") {
    strcpy(slct_book.keyword, keyword.c_str());
  }
  if (price >= 0.0) {
    slct_book.price = price;
  }

  database_isbn.insert(slct_book);
}

void BookSys::import(const int &quantity, const double &total_cost) {

  if (accounts.telllvl() < 3) {
    throw 0;
    return;
  }

  database_isbn.erase(slct_book);
  slct_book.quantity += quantity;
  slct_book.total_cost += total_cost;
  database_isbn.insert(slct_book);
}

void BookSys::select_clear() { slct_book = Book(); }

Book BookSys::select_book() { return slct_book; }