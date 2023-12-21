#include "validator.hpp"
#include <cstdarg>
#include <iostream>
#include <memory>
#include <type_traits>

template <class T> Expect<T> &Expect<T>::toBe(const T &rhs) {

  const auto &lhs = *ptr;

  if (is_reversed) {

    if (lhs == rhs) {
      val = false;
    }

  } else {

    if (lhs != rhs) {
      val = false;
    }
  }

  if (!val) {
    throw 0;
  }

  return *this;
}

template <class T> template <class U> Expect<T> Expect<T>::toBe() {

  static_assert(std::is_base_of<T, U>::value);

  if (dynamic_cast<const U *>(ptr) == nullptr) {
    val = false;
  }

  if (!val) {
    throw 0;
  }

  return *this;
}

template <class T>
template <typename... Args>
Expect<T> &Expect<T>::toBeOneOf(Args... args) {

  const auto &lhs = *ptr;

  bool flag = false;

  auto process = [&](T arg) {
    if (lhs == arg) {
      flag = true;
    }
  };

  (..., process(args));

  val = flag;

  if (is_reversed) {
    val = !val;
  }

  if (!val) {
    throw 0;
  }

  return *this;
}

template <class T> Expect<T> &Expect<T>::le(const T &rhs) {

  const auto &lhs = *ptr;

  if (is_reversed) {

    if (lhs <= rhs) {
      val = false;
    }
  } else {

    if (lhs > rhs) {
      val = false;
    }
  }

  if (!val) {
    throw 0;
  }

  return *this;
}

template <class T> Expect<T> &Expect<T>::ge(const T &rhs) {

  const auto &lhs = *ptr;

  if (is_reversed) {

    if (lhs >= rhs) {
      val = false;
    }
  } else {

    if (lhs < rhs) {
      val = false;
    }
  }

  if (!val) {
    throw 0;
  }

  return *this;
}

template <class T> Expect<T> &Expect<T>::Not() {

  is_reversed = !is_reversed;

  return *this;
}

template <class T> Expect<T>::Expect(const T &lhs_) : ptr(&lhs_) {}

template <class T> Expect<T>::~Expect() = default;

template <class T> Expect<T>::Expect() = default;

template <class T> Expect<T>::operator bool() const { return val; }

template <class T> Expect<T> &Expect<T>::operator()(const T &lhs_) {

  ptr = &lhs_;
  val = true;
  is_reversed = false;
  return *this;
}

template <class T> Expect<T> expect(const T &lhs_) { return Expect<T>(lhs_); }

/*
int main() {

  Derived u;
  Base b;
  Base &ref = u;

  std::cout << "b"
            << ":\t";

  try {
    expect(b).toBe<Derived>();

    std::cout << "True\n";
  } catch (...) {

    std::cout << "False\n";
  }

  std::cout << "ref"
            << ":\t";

  try {
    expect(ref).toBe<Derived>();

    std::cout << "True\n";
  } catch (...) {

    std::cout << "False\n";
  }

  std::cout << "u"
            << ":\t";

  try {
    expect(u).toBe<Derived>();

    std::cout << "True\n";
  } catch (...) {

    std::cout << "False\n";
  }

  int a;

  while (std::cin >> a) {
    std::cout << a << ":\t";
    try {
      expect(a).toBeOneOf(1, 3, 5, 7);

      std::cout << "True\n";
    } catch (...) {

      std::cout << "False\n";
    }
  }

  return 0;
}
*/