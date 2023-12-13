#include "command.hpp"
#include "string_check.hpp"
#include <sstream>
#include <string>

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

  } else if (comtype == "show") {
    std::string str = "";
    std::string tok;
    int type = 0;

    getline(in, tok);
    tok = str_check::trim(tok);

    if (!in.eof()) {
      throw 0;
      return;
    }

    if (tok.substr(0, 6) == "-ISBN=") {

      str = tok.substr(6, tok.size() - 6);
      type = 1;
      if (str == "" || str.size() > 20 || !str_check::non_invisible(str)) {
        throw 0;
        return;
      }

    } else if (tok.substr(0, 7) == "-name=\"" && tok.back() == '\"') {

      str = tok.substr(7, tok.size() - 8);
      type = 2;
      if (str == "" || str.size() > 60 ||
          !str_check::non_invisible_quotes(str)) {
        throw 0;
        return;
      }

    } else if (tok.substr(0, 9) == "-author=\"" && tok.back() == '\"') {

      str = tok.substr(9, tok.size() - 10);
      type = 3;
      if (str == "" || str.size() > 60 ||
          !str_check::non_invisible_quotes(str)) {
        throw 0;
        return;
      }

    } else if (tok.substr(0, 10) == "-keyword=\"" && tok.back() == '\"') {

      str = tok.substr(10, tok.size() - 11);
      type = 4;
      if (str == "" || str.size() > 60 ||
          !str_check::check_keyword_non_repetition(str)) {
        throw 0;
        return;
      }

    } else if (tok == "") {

      type = 0;

    } else {
      throw 0;
      return;
    }

    books.show(type, str);

  } else if (comtype == "buy") {

    std::string isbn, quantity;
    in >> isbn >> quantity;

    if (!in.eof()) {
      throw 0;
      return;
    }

    if (isbn.size() > 20 || !str_check::non_invisible(isbn) ||
        quantity.size() > 10 || !str_check::check_int(quantity)) {
      throw 0;
      return;
    }

    books.buy(isbn, stoi(quantity));

  } else if (comtype == "select") {

    std::string isbn;
    in >> isbn;

    if (!in.eof()) {
      throw 0;
      return;
    }

    if (isbn == "" || isbn.size() > 20 || !str_check::non_invisible(isbn)) {
      throw 0;
      return;
    }

    books.select(isbn);

  } else if (comtype == "modify") {

    bool mdfy_isbn = false, mdfy_name = false, mdfy_author = false,
         mdfy_keyword = false, mdfy_price = false;
    std::string tok, isbn = "", name = "", author = "", keyword = "",
                     price = "-1";

    if (in.eof()) {
      throw 0;
      return;
    }

    while (in >> tok) {
      if (tok.substr(0, 6) == "-ISBN=") {

        if (mdfy_isbn) {
          throw 0;
          return;
        }

        isbn = tok.substr(6, tok.size() - 6);
        if (isbn == "" || isbn.size() > 20 || !str_check::non_invisible(isbn)) {
          throw 0;
          return;
        }
        mdfy_isbn = true;

      } else if (tok.substr(0, 7) == "-name=\"" && tok.back() == '\"') {

        if (mdfy_name) {
          throw 0;
          return;
        }

        name = tok.substr(7, tok.size() - 8);
        if (name == "" || name.size() > 60 ||
            !str_check::non_invisible_quotes(name)) {
          throw 0;
          return;
        }
        mdfy_name = true;

      } else if (tok.substr(0, 9) == "-author=\"" && tok.back() == '\"') {

        if (mdfy_author) {
          throw 0;
          return;
        }

        author = tok.substr(9, tok.size() - 10);
        if (author == "" || author.size() > 60 ||
            !str_check::non_invisible_quotes(author)) {
          throw 0;
          return;
        }
        mdfy_author = true;

      } else if (tok.substr(0, 10) == "-keyword=\"" && tok.back() == '\"') {

        if (mdfy_keyword) {
          throw 0;
          return;
        }

        keyword = tok.substr(10, tok.size() - 11);
        if (keyword == "" || keyword.size() > 60 ||
            !str_check::check_keyword_non_repetition(keyword)) {
          throw 0;
          return;
        }
        mdfy_keyword = true;

      } else if (tok.substr(0, 7) == "-price=") {

        if (mdfy_price) {
          throw 0;
          return;
        }

        price = tok.substr(7, tok.size() - 7);
        if (price == "" || price.size() > 13 ||
            !str_check::check_float(price)) {
          throw 0;
          return;
        }
        mdfy_price = true;

      } else {
        throw 0;
        return;
      }
    }

    books.modify(isbn, name, author, keyword, std::stod(price));

  } else if (comtype == "import") {

    std::string quantity, total_cost;
    in >> quantity >> total_cost;

    if (!in.eof()) {
      throw 0;
      return;
    }

    if (quantity.size() > 10 || total_cost.size() > 13 ||
        !str_check::check_int(quantity) ||
        !str_check::check_float(total_cost)) {
      throw 0;
      return;
    }

    books.import(stoi(quantity), std::stod(total_cost));

  } else if (comtype == "accounts") {

    accounts.print();

  } else if (comtype == "current_account") {

    std::cout << accounts.tellacc() << '\n';

  } else if (comtype == "current_book") {
    std::cout << books.select_book();
  } else if (comtype == "show_index_name") {
    books.print_id_name();
  } else if (comtype == "show_index_author") {
    books.print_id_author();
  } else if (comtype == "show_index_keyword") {
    books.print_id_keyword();
  } else {
    throw 0;
    return;
  }
}

void CommandSys::read(const std::string text) { latest_command = text; }

void CommandSys::execute() { latest_command.execute(); }