#include "diary.hpp"
#include <iomanip>

extern Report<SysLog> logs;

Finance::~Finance() {
  file.close();
#ifdef DEBUG
  remove(file_name.c_str());
#endif
}

Finance::Finance(const std::string &name) : file_name(name) {

  file.open(file_name);

  if (!file.is_open()) {

    file.open(file_name, std::ios::out);

    file.close();
    file.open(file_name);
  }

  char buffer[MAX];
  file.rdbuf()->pubsetbuf(buffer, sizeof(buffer));
}

void Finance::write(const double &val) {
  file.seekp(0, std::ios::end);
  file.write(reinterpret_cast<const char *>(&val), sizeof(double));
}

void Finance::finance(const int &count) {

  if (accounts.telllvl() < 7) {
    throw 0;
    return;
  }

  if (count == 0) {
    std::cout << '\n';
    return;
  }

  double plus = 0, minus = 0;
  file.seekg(0, std::ios::end);
  const ul end = file.tellg();

  if (count > static_cast<int>(end / sizeof(double))) {
    throw 0;
    return;
  }

  if (count == -1) { // all

    ul pos = 0;
    while (pos < end) {
      double num;
      file.seekg(pos);
      file.read(reinterpret_cast<char *>(&num), sizeof(double));
      if (num > 0.0) {
        plus += num;
      } else {
        minus -= num;
      }
      pos += sizeof(double);
    }
  } else { // count

    for (int i = 1; i <= count; ++i) {
      file.seekg(-i * sizeof(double), std::ios::end);
      double num;
      file.read(reinterpret_cast<char *>(&num), sizeof(double));
      if (num > 0.0) {
        plus += num;
      } else {
        minus -= num;
      }
    }
  }

  std::cout << "+ " << std::fixed << std::setprecision(2) << plus << " - "
            << std::fixed << std::setprecision(2) << minus << '\n';

  logs.write(SysLog("Finance showed."));
}

Alteration::Alteration(const std::string &id_, const std::string &isbn_,
                       bool type_, int count_, double sum_)
    : type(type_), count(count_), sum(sum_) {
  strcpy(id, id_.c_str());
  strcpy(isbn, isbn_.c_str());
}

std::ostream &operator<<(std::ostream &op, const Alteration &val) {
  op << (val.type ? "+ Book\tISBN: " : "- Book\tISBN: ") << val.isbn
     << "\tCount: " << val.count
     << (val.type ? "\tTotal Cost: " : "\tTotal Price: ") << std::fixed
     << std::setprecision(2) << val.sum
     << (val.type ? "\tBy Worker: " : "\tBy Customer: ") << val.id << '\n';

  return op;
}

WorkerLog::WorkerLog(const std::string &id_, std::string &text_, bool status_)
    : status(status_) {
  strcpy(id, id_.c_str());
  strcpy(text, text_.c_str());
}

std::ostream &operator<<(std::ostream &op, const WorkerLog &rhs) {
  op << "Worker Id: " << rhs.id << "\tOperation: " << rhs.text
     << "\tStatus: " << (rhs.status ? "Succeeded" : "Failed") << '\n';
  return op;
}

template <> void Report<Alteration>::execute() {
  if (accounts.telllvl() < 7) {
    throw 0;
    return;
  }

  file.seekp(0, std::ios::end);
  const ul end = file.tellp();

  std::cout << "==============================================================="
               "==========================================\n"
            << "* FINANCIAL REPORT *\n\n";
  std::cout << (end / sizeof(Alteration)) << " Records:\n";

  ul pos = 0, i = 1;
  while (pos < end) {
    Alteration val;
    file.seekg(pos);
    file.read(reinterpret_cast<char *>(&val), sizeof(Alteration));
    std::cout << "Record " << i << ":\t" << val;

    pos += sizeof(Alteration);
    ++i;
  }

  std::cout << "\n* END *\n"
            << "==============================================================="
               "==========================================\n";
  logs.write(SysLog("Finance reported."));
}

template <> void Report<WorkerLog>::execute() {
  if (accounts.telllvl() < 7) {
    throw 0;
    return;
  }

  file.seekp(0, std::ios::end);
  const ul end = file.tellp();

  std::cout << "==============================================================="
               "==========================================\n"
            << "* WORKER REPORT *\n\n";
  std::cout << (end / sizeof(WorkerLog)) << " Records:\n";

  ul pos = 0, i = 1;
  while (pos < end) {
    WorkerLog val;
    file.seekg(pos);
    file.read(reinterpret_cast<char *>(&val), sizeof(WorkerLog));
    std::cout << "Record " << i << ":\t" << val;

    pos += sizeof(WorkerLog);
    ++i;
  }

  std::cout << "\n* END *\n"
            << "==============================================================="
               "==========================================\n";
  logs.write(SysLog("Worker logs reported."));
}

template <> void Report<SysLog>::execute() {
  if (accounts.telllvl() < 7) {
    throw 0;
    return;
  }

  file.seekp(0, std::ios::end);
  const ul end = file.tellp();

  std::cout << "==============================================================="
               "==========================================\n"
            << "* SYSTEM LOG *\n\n";
  std::cout << (end / sizeof(SysLog)) << " Records:\n";

  ul pos = 0, i = 1;
  while (pos < end) {
    SysLog val;
    file.seekg(pos);
    file.read(reinterpret_cast<char *>(&val), sizeof(SysLog));
    std::cout << "Record " << i << ":\t" << val;

    pos += sizeof(SysLog);
    ++i;
  }

  std::cout << "\n* END *\n"
            << "==============================================================="
               "==========================================\n";

  logs.write(SysLog("System Log showed."));
}

SysLog::SysLog(const std::string &str) { strcpy(content, str.c_str()); }

std::ostream &operator<<(std::ostream &op, const SysLog &rhs) {
  op << rhs.content << '\n';
  return op;
}