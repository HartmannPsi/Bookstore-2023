#include <cstdarg>

template <class T> class Expect {
private:
  bool val = true;
  bool is_reversed = false;
  const T *ptr = nullptr;

public:
  Expect<T> &toBe(const T &rhs);

  template <class U> Expect<T> toBe();

  template <typename... Args> Expect<T> &toBeOneOf(Args... args);

  Expect<T> &le(const T &rhs);

  Expect<T> &ge(const T &rhs);

  Expect<T> &Not();

  Expect<T> &And = *this;

  Expect<T> &Or = *this;

  Expect<T> &but = *this;

  Expect(const T &lhs_);

  Expect();

  ~Expect();

  Expect<T> &operator()(const T &lhs_);

  operator bool() const;

  template <class U> friend Expect<U> expect(const U &lhs_);
};

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
