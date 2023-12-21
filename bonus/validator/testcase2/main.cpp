#include "../validator.hpp"
#include <iostream>

class Base {
public:
  Base() = default;

  virtual ~Base() = default;
};

class Derived : public Base {
public:
  Derived() = default;

  ~Derived() = default;
};

class Another {
public:
  Another() = default;

  ~Another() = default;
};

int main() {

  Base base;

  Derived derived;

  Base &ref = derived;

  Another another;

  try {
    expect(base).toBe<Derived>();

    std::cout << "base is an instance of Derived.\n";
  } catch (...) {
    std::cout << "base isn't an instance of Derived.\n";
  }

  try {
    expect(derived).toBe<Derived>();

    std::cout << "derived is an instance of Derived.\n";
  } catch (...) {
    std::cout << "derived isn't an instance of Derived.\n";
  }

  try {
    expect(ref).toBe<Derived>();

    std::cout << "ref is an instance of Derived.\n";
  } catch (...) {
    std::cout << "ref isn't an instance of Derived.\n";
  }

  // expect(another).toBe<Derived>();
  // expect(ref).toBe<Another>();

  return 0;
}
