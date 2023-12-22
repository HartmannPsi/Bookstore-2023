#include "account.hpp"
#include "diary.hpp"
#include "string_check.hpp"
#include <cassert>

extern Report<SysLog> logs;

Account::Account(const std::string &name_, const std::string &id_,
                 const std::string &password_, const Level &prev_)
    : prev(prev_) {

  strcpy(name, name_.c_str());
  strcpy(id, id_.c_str());
  strcpy(password, password_.c_str());
}

Account::Account(const std::string &id_, const Level &prev_) : prev(prev_) {
  strcpy(id, id_.c_str());
}

bool Account::operator>(const Account &rhs) const {
  const int res = strcmp(id, rhs.id);
  return res > 0;
}

bool Account::operator<(const Account &rhs) const {
  const int res = strcmp(id, rhs.id);
  return res < 0;
}

bool Account::operator>=(const Account &rhs) const {
  const int res = strcmp(id, rhs.id);
  return res >= 0;
}

bool Account::operator<=(const Account &rhs) const {
  const int res = strcmp(id, rhs.id);
  return res <= 0;
}

bool Account::operator==(const Account &rhs) const {
  const int res = strcmp(id, rhs.id);
  return res == 0;
}

Account Account::operator=(const Account &rhs) {
  if (this == &rhs) {
    return *this;
  }

  strcpy(name, rhs.name);
  strcpy(id, rhs.id);
  strcpy(password, rhs.password);
  prev = rhs.prev;

  return *this;
}

std::ostream &operator<<(std::ostream &op, const Account &rhs) {
  op << "id: " << rhs.id << "\nname: " << rhs.name
     << "\npassword: " << rhs.password << "\nLevel: " << rhs.prev << '\n';
  return op;
}

AccountSys::AccountSys() : database("accounts.dat") {
  const auto count = database.count(Account("root"));
  if (count == 0) {
    database.insert(Account("root", "root", "sjtu", admin));
  }

  log_stack.push(Account(""));
}

void AccountSys::login(const std::string &id, const std::string &password) {

  const auto acc = database.find(Account(id));
  const auto &current_acc = log_stack.top();
  if (strlen(acc.id) == 0) {
    throw 0;
    return;
  }

  if (password == "") {
    if (current_acc.prev > acc.prev) {

      log_stack.push(acc);
    } else {
      throw 0;
      return;
    }
  } else {

    const int res = strcmp(password.c_str(), acc.password);
    if (res == 0) {
      log_stack.push(acc);
    } else {
      throw 0;
      return;
    }
  }

  std::string str = "Logged in account " + id;

  logs.write(SysLog(str));
} // prev = 0

void AccountSys::logout() {

  if (log_stack.size() == 1) {
    throw 0;
    return;
  } else {
    std::string str = "Logged out account " + std::string(tellacc().id);

    logs.write(SysLog(str));
    log_stack.pop();
  }
} // prev = 1

void AccountSys::regin(const std::string &id, const std::string &password,
                       const std::string &name) {

  const auto count = database.count(Account(id));

  if (count) {
    throw 0;
    return;
  } else {
    const Account new_acc(name, id, password, customer);
    database.insert(new_acc);

    std::string str = "Registered account " + id + " with password " + password;

    logs.write(SysLog(str));
  }
} // prev = 0

void AccountSys::passwd(const std::string &id, const std::string &new_passwd,
                        const std::string &current_passwd) {

  assert(false);

  if (log_stack.top().prev < 1) {
    throw 0;
    return;
  }

  auto acc = database.find(Account(id));

  if (strlen(acc.id) == 0) {
    throw 0;
    return;
  }

  if (current_passwd == "") {
    if (log_stack.top().prev == admin) {

      strcpy(acc.password, new_passwd.c_str());
      database.update(acc);
    } else {
      throw 0;
      return;
    }
  } else {

    const int res = strcmp(current_passwd.c_str(), acc.password);
    if (res == 0) {

      strcpy(acc.password, new_passwd.c_str());
      database.update(acc);
    } else {
      throw 0;
      return;
    }
  }

  std::string str = "Changed password of account " + id + " from " +
                    current_passwd + " to " + new_passwd;

  logs.write(SysLog(str));
} // prev = 1

void AccountSys::useradd(const std::string &id, const std::string &password,
                         const Level &prev, const std::string &name) {

  if (log_stack.top().prev < 3 || log_stack.top().prev <= prev) {
    throw 0;
    return;
  }

  const auto count = database.count(Account(id));

  if (count) {
    throw 0;
    return;
  } else {
    const Account new_acc(name, id, password, prev);
    database.insert(new_acc);
  }

  std::string str = "Added account " + id + " of level " +
                    std::to_string(static_cast<int>(prev));

  logs.write(SysLog(str));
} // prev = 3

void AccountSys::erase(const std::string id) {
  if (log_stack.top().prev < 7) {
    throw 0;
    return;
  }

  const auto acc = database.find(Account(id));
  if (strlen(acc.id) == 0) {
    throw 0;
    return;
  }

  if (log_stack.check_acc(acc)) {
    throw 0;
    return;
  }

  database.erase(acc);

  std::string str = "Deleted account " + id;

  logs.write(SysLog(str));
} // prev = 7

Level AccountSys::telllvl() const { return log_stack.top().prev; }

Account AccountSys::tellacc() const { return log_stack.top(); }

void AccountSys::print() { database.print(); }

Book &AccountSys::slct_book() { return log_stack.slct_book(); }

void AccountSys::modify(const Book &val_old, const Book &val_new) {
  log_stack.modify(val_old, val_new);
}

LogStack::LogStack() {
  acc_stack.clear();
  book_stack.clear();
  // acc_stack.push_back(Account("", "", "", visitor));
  // book_stack.push_back(Book());
}

void LogStack::push(const Account &acc) {

  acc_stack.push_back(acc);
  book_stack.push_back(Book());
}

void LogStack::select(const Book &book) { book_stack.back() = book; }

bool LogStack::check_acc(const Account &acc) const {
  for (int i = 0; i != acc_stack.size(); ++i) {
    if (acc_stack[i] == acc) {
      return true;
    }
  }

  return false;
}

void LogStack::pop() {
  acc_stack.pop_back();
  book_stack.pop_back();
}

Account LogStack::top() const { return acc_stack.back(); }

int LogStack::size() const { return acc_stack.size(); }

Book &LogStack::slct_book() { return book_stack.back(); }

void LogStack::modify(const Book &val_old, const Book &val_new) {
  for (int i = 0; i != book_stack.size(); ++i) {
    if (book_stack[i] == val_old) {
      book_stack[i] = val_new;
    }
  }
}