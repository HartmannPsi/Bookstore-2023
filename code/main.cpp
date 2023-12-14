#include "account.hpp"
#include "block_list.hpp"
#include "command.hpp"
#include "string_check.hpp"
#include <stdexcept>

AccountSys accounts;
CommandSys commands;
BookSys books;

int main() {
  while (!std::cin.eof()) {

    try {
      std::string text;
      getline(std::cin, text);
#ifdef DEBUG
      std::cout << text << ": \n";
#endif
      commands.read(text);
      commands.execute();

    } catch (...) {
      std::cout << "Invalid\n";
      //          << "\n";
    }
#ifdef DEBUG
    std::cout << "-----------------------------------------------------------"
                 "---------\n";
#endif
  }

  return 0;
}