#include "account.hpp"
#include "block_list.hpp"
#include "command.hpp"
#include "string_check.hpp"

AccountSys accounts;
CommandSys commands;

int main() {
  while (!std::cin.eof()) {

    try {
      std::string text;
      getline(std::cin, text);
      commands.read(text);
      std::cout << text << ": \n";
      commands.execute();
      std::cout << "\n";

    } catch (...) {
      std::cout << "Invalid\n"
                << "\n";
    }
  }

  return 0;
}