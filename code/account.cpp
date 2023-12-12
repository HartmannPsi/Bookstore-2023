#include "account.hpp"
#include "string_check.hpp"

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
      books.select_clear();
    } else {

      throw 0;
      return;
    }

  } else {

    const int res = strcmp(password.c_str(), acc.password);
    if (res == 0) {
      log_stack.push(acc);
      books.select_clear();
    } else {

      throw 0;
      return;
    }
  }
} // prev = 0

void AccountSys::logout() {
  if (log_stack.size() == 1 || log_stack.top().prev < 1) {
    throw 0;
    return;
  } else {
    log_stack.pop();
    books.select_clear();
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
  }
} // prev = 0

void AccountSys::passwd(const std::string &id, const std::string &new_passwd,
                        const std::string &current_passwd) {

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

  auto tmp_stack(log_stack);

  while (!tmp_stack.empty()) {
    const auto &log_acc = tmp_stack.top();
    if (log_acc == acc) {
      throw 0;
      return;
    }
    tmp_stack.pop();
  }

  database.erase(acc);
} // prev = 7

Level AccountSys::telllvl() const { return log_stack.top().prev; }

Account AccountSys::tellacc() const { return log_stack.top(); }

void AccountSys::print() { database.print(); }