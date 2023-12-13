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
      std::cout << text << ": \n";
      commands.read(text);
      commands.execute();
      std::cout << "-----------------------------------------------------------"
                   "---------\n";

    } catch (int) {
      std::cout << "Invalid\n"
                << "\n";
    } catch (std::invalid_argument &e) {
      std::cout << e.what() << '\n';
    } catch (std::out_of_range &e) {
      std::cout << e.what() << '\n';
    }
  }

  return 0;
}