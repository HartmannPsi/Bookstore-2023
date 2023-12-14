#include "book.hpp"
#include "block_list.hpp"
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

BookSys::BookSys()
    : database_isbn("books_ISBN.dat"), index_author("index_author.dat"),
      index_name("index_name.dat"), index_keyword("index_keyword.dat") {}

void BookSys::show(const int &type, const std::string &str) {

  if (accounts.telllvl() < 1) {
    throw 0;
    return;
  }

  if (type == 0) { // all

    database_isbn.print();
  } else if (type == 1) { // isbn

    auto *const ans = database_isbn.get(Book(str));

    if (ans->empty()) {
      std::cout << '\n';
    } else {
      for (auto i = ans->cbegin(); i != ans->cend(); ++i) {
        std::cout << *i;
      }
    }
    delete ans;

  } else if (type == 2) { // name

    auto *const indexes = index_name.get(Index(str));

    if (indexes->empty()) {
      std::cout << '\n';
    } else {

      std::vector<Book> ans;
      ans.resize(indexes->size());

      for (auto i = 0; i != indexes->size(); ++i) {

        const std::string isbn((*indexes)[i].isbn);
        ans[i] = database_isbn.find(Book(isbn));
      }

      std::sort(ans.begin(), ans.end());

      for (auto i = ans.cbegin(); i != ans.cend(); ++i) {
        std::cout << *i;
      }
    }

    delete indexes;

  } else if (type == 3) { // author

    auto *const indexes = index_author.get(Index(str));

    if (indexes->empty()) {
      std::cout << '\n';
    } else {

      std::vector<Book> ans;
      ans.resize(indexes->size());

      for (auto i = 0; i != indexes->size(); ++i) {

        const std::string isbn((*indexes)[i].isbn);
        ans[i] = database_isbn.find(Book(isbn));
      }

      std::sort(ans.begin(), ans.end());

      for (auto i = ans.cbegin(); i != ans.cend(); ++i) {
        std::cout << *i;
      }
    }

    delete indexes;

  } else if (type == 4) { // keyword

    auto *const indexes = index_keyword.get(Index(str));

    if (indexes->empty()) {
      std::cout << '\n';
    } else {

      std::vector<Book> ans;
      ans.resize(indexes->size());

      for (auto i = 0; i != indexes->size(); ++i) {

        const std::string isbn((*indexes)[i].isbn);
        ans[i] = database_isbn.find(Book(isbn));
      }

      std::sort(ans.begin(), ans.end());

      for (auto i = ans.cbegin(); i != ans.cend(); ++i) {
        std::cout << *i;
      }
    }

    delete indexes;
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

  buy_book.quantity -= quantity;
  std::cout << std::fixed << std::setprecision(2) << quantity * buy_book.price
            << '\n';
  database_isbn.update(buy_book);
  accounts.modify(buy_book, buy_book);
  finances.write(quantity * buy_book.price);
}

void BookSys::select(const std::string &isbn) {

  if (accounts.telllvl() < 3) {
    throw 0;
    return;
  }

  Book &slct_book = select_book();
  slct_book = database_isbn.find(Book(isbn));
  if (strlen(slct_book.isbn) == 0) {
    strcpy(slct_book.isbn, isbn.c_str());
    database_isbn.insert(slct_book);
  }
}

void BookSys::modify(const std::string &isbn, const std::string &name,
                     const std::string &author, const std::string &keyword,
                     double price) {

  auto &slct_book = select_book();

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

  const auto book_tmp = slct_book;
  bool is_isbn_changed = false;

  if (isbn != "") {

    const Index id_name_old(book_tmp.name, book_tmp.isbn),
        id_author_old(book_tmp.author, book_tmp.isbn);

    strcpy(slct_book.isbn, isbn.c_str());

    const Index id_name_new(book_tmp.name, slct_book.isbn),
        id_author_new(book_tmp.author, slct_book.isbn);

    index_name.update(id_name_old, id_name_new);
    index_author.update(id_author_old, id_author_new);
    is_isbn_changed = true;
  }

  if (name != "") {

    strcpy(slct_book.name, name.c_str());
    Index id_name(book_tmp.name, slct_book.isbn);
    index_name.erase(id_name);
    strcpy(id_name.index, slct_book.name);
    index_name.insert(id_name);
  }

  if (author != "") {

    strcpy(slct_book.author, author.c_str());
    Index id_author(book_tmp.author, slct_book.isbn);
    index_author.erase(id_author);
    strcpy(id_author.index, slct_book.author);
    index_author.insert(id_author);
  }

  if (keyword != "") {
    strcpy(slct_book.keyword, keyword.c_str());
    char keywords_old[61], keywords_new[61];
    const char *delim = "|";
    strcpy(keywords_old, book_tmp.keyword);
    strcpy(keywords_new, slct_book.keyword);

    auto tok = strtok(keywords_old, delim);
    while (tok != nullptr) {
      index_keyword.erase(Index(tok, slct_book.isbn));
      tok = strtok(nullptr, delim);
    }

    tok = strtok(keywords_new, delim);
    while (tok != nullptr) {
      index_keyword.insert(Index(tok, slct_book.isbn));
      tok = strtok(nullptr, delim);
    }
  }

  if (price >= 0.0) {
    slct_book.price = price;
  }

  if (is_isbn_changed) {

    database_isbn.erase(book_tmp);
    database_isbn.insert(slct_book);
  } else {

    database_isbn.update(slct_book);
  }

  accounts.modify(book_tmp, slct_book);
}

void BookSys::import(const int &quantity, const double &total_cost) {

  if (accounts.telllvl() < 3) {
    throw 0;
    return;
  }

  auto &slct_book = select_book();

  if (strlen(slct_book.isbn) == 0) {
    throw 0;
    return;
  }

  slct_book.quantity += quantity;
  slct_book.total_cost += total_cost;
  database_isbn.update(slct_book);

  accounts.modify(slct_book, slct_book);
  finances.write(-total_cost);
}

void BookSys::select_clear() { select_book() = Book(); }

Book &BookSys::select_book() { return accounts.slct_book(); }

void BookSys::print_id_name() { index_name.print(); }

void BookSys::print_id_author() { index_author.print(); }

void BookSys::print_id_keyword() { index_keyword.print(); }

Index::Index(const std::string &index_, const std::string &isbn_) {
  strcpy(index, index_.c_str());
  strcpy(isbn, isbn_.c_str());
}

bool Index::operator>(const Index &rhs) const {
  const int res = strcmp(index, rhs.index);
  return res > 0;
}

bool Index::operator<(const Index &rhs) const {
  const int res = strcmp(index, rhs.index);
  return res < 0;
}

bool Index::operator>=(const Index &rhs) const {
  const int res = strcmp(index, rhs.index);
  return res >= 0;
}

bool Index::operator<=(const Index &rhs) const {
  const int res = strcmp(index, rhs.index);
  return res <= 0;
}

bool Index::operator==(const Index &rhs) const {
  const int res = strcmp(index, rhs.index);
  return res == 0;
}

Index Index::operator=(const Index &rhs) {

  if (this == &rhs) {
    return *this;
  }

  strcpy(index, rhs.index);
  strcpy(isbn, rhs.isbn);

  return *this;
}

std::ostream &operator<<(std::ostream &op, const Index &rhs) {
  op << rhs.index << '\t' << rhs.isbn << '\n';
  return op;
}

bool strictly_equal(const Index &lhs, const Index &rhs) {
  const int res1 = strcmp(lhs.index, rhs.index),
            res2 = strcmp(lhs.isbn, rhs.isbn);
  return res1 == 0 && res2 == 0;
}