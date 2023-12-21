#include "../validator.hpp"
#include <iostream>

int main() {

  for (int i = 0; i != 17; ++i) {
    std::cout << i << ":\t";

    try {
      expect(i).ge(5).And.le(13).but.Not().toBeOneOf(5, 7, 12).Or.toBe(
          11); // i >= 5 && i <= 13 && i != 5 && i != 7 && i != 11 && i != 12

      std::cout << "True\n";
    } catch (...) {

      std::cout << "False\n";
    }
  }

  return 0;
}