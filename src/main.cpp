#include "account.hpp"
#include "block_list.hpp"
#include "book.hpp"
#include "command.hpp"
#include "diary.hpp"
#include "string_check.hpp"
#include <stdexcept>
#define TEST

AccountSys accounts;
CommandSys commands;
BookSys books;
Finance finances("finance.dat");
Report<Alteration> financials("report_finance.dat");
Report<WorkerLog> workers("report_worker.dat");
Report<SysLog> logs("system_log.dat");

int main() {
  while (!std::cin.eof()) {

    WorkerLog log;
    const int level = accounts.telllvl();

    try {
      std::string text;
      getline(std::cin, text);
#ifdef TEST
      std::cout << text << ": \n";
#endif
      commands.read(text);

      if (level == 3) {
        strcpy(log.id, accounts.tellacc().id);
        strcpy(log.text, text.c_str());
      }

      commands.execute();

      if (level == 3) {
        workers.write(log);
      }

    } catch (...) {
      std::cout << "Invalid\n";

      if (level == 3) {

        log.status = false;
        workers.write(log);
      }
    }
#ifdef TEST
    std::cout << "-----------------------------------------------------------"
                 "---------\n";
#endif
  }

  return 0;
}