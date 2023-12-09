#pragma once
#ifndef STRING_CHECK_HPP
#define STRING_CHECK_HPP

#include <string>

namespace str_check {

bool num_letter_underscore(const std::string &str);

bool non_invisible(const std::string &str);

bool non_invisible_quotes(const std::string &str);
} // namespace str_check

#endif