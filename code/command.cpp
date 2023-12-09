#include "command.hpp"
#include "string_check.hpp"
#include <sstream>

Command::Command(const std::string &str) : text(str) {}

void Command::execute() {

  std::istringstream in(text);
  std::string comtype;
  in >> comtype;

  if (comtype == "su") {

    std::string id, password;
    in >> id >> password;

    if (!in.eof()) {
      throw 0;
      return;
    }

    if (id.size() > 30 || password.size() > 30 ||
        !str_check::num_letter_underscore(id) ||
        !str_check::num_letter_underscore(password)) {
      throw 0;
      return;
    }

    accounts.login(id, password);

  } else if (comtype == "logout") {

    if (!in.eof()) {
      throw 0;
      return;
    }

    accounts.logout();

  } else if (comtype == "register") {

    std::string id, password, name;
    in >> id >> password >> name;

    if (!in.eof()) {
      throw 0;
      return;
    }

    if (id.size() > 30 || password.size() > 30 || name.size() > 30 ||
        !str_check::num_letter_underscore(id) ||
        !str_check::num_letter_underscore(password) ||
        !str_check::non_invisible(name)) {
      throw 0;
      return;
    }

    accounts.regin(id, password, name);

  } else if (comtype == "passwd") {

    std::string id, passwd1, passwd2;
    in >> id >> passwd1 >> passwd2;

    if (!in.eof()) {
      throw 0;
      return;
    }

    if (id.size() > 30 || passwd1.size() > 30 || passwd2.size() > 30 ||
        !str_check::num_letter_underscore(id) ||
        !str_check::num_letter_underscore(passwd1) ||
        !str_check::num_letter_underscore(passwd2)) {
      throw 0;
      return;
    }

    if (passwd2 == "") {
      accounts.passwd(id, passwd1);
    } else {
      accounts.passwd(id, passwd2, passwd1);
    }

  } else if (comtype == "useradd") {

    std::string id, password, name;
    int prev;
    in >> id >> password >> prev >> name;

    if (!in.eof()) {
      throw 0;
      return;
    }

    if (id.size() > 30 || password.size() > 30 || name.size() > 30 ||
        !str_check::num_letter_underscore(id) ||
        !str_check::num_letter_underscore(password) ||
        !str_check::non_invisible(name)) {
      throw 0;
      return;
    }

    if (prev != 0 && prev != 1 && prev != 3 && prev != 7) {
      throw 0;
      return;
    }

    accounts.useradd(id, password, static_cast<Level>(prev), name);

  } else if (comtype == "delete") {

    std::string id;
    in >> id;

    if (!in.eof()) {
      throw 0;
      return;
    }

    if (id.size() > 30 || !str_check::num_letter_underscore(id)) {
      throw 0;
      return;
    }

    accounts.erase(id);

  } else if (comtype == "quit" || comtype == "exit") {

    if (!in.eof()) {
      throw 0;
      return;
    }

    exit(0);

  } else if (comtype == "accounts") {

    accounts.print();

  } else if (comtype == "current_account") {

    std::cout << accounts.tellacc() << '\n';

  } else {
    throw 0;
    return;
  }
}

void CommandSys::read(const std::string text) { latest_command = text; }

void CommandSys::execute() { latest_command.execute(); }