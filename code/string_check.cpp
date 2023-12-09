#include "string_check.hpp"

bool str_check::num_letter_underscore(const std::string &str) {

  auto check = [](char ch) {
    return (ch == '_') || (ch >= '0' && ch <= '9') ||
           (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
  };

  for (int i = 0; i != str.size(); ++i) {
    if (!check(str[i])) {
      return false;
    }
  }

  return true;
}

bool str_check::non_invisible(const std::string &str) {

  auto check = [](char ch) { return ch > 31 && ch != 127; };

  for (int i = 0; i != str.size(); ++i) {
    if (!check(str[i])) {
      return false;
    }
  }

  return true;
}

bool str_check::non_invisible_quotes(const std::string &str) {

  auto check = [](char ch) { return ch > 31 && ch != 127 && ch != '\"'; };

  for (int i = 0; i != str.size(); ++i) {
    if (!check(str[i])) {
      return false;
    }
  }

  return true;
}