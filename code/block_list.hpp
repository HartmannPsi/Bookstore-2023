#pragma once
#include <ostream>
#ifndef BLOCK_LIST_HPP
#define BLOCK_LIST_HPP
#define DEBUG

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#include <vector>

using ul = unsigned long;

constexpr ul sqN = 450;      // sqrt(N)
constexpr ul MAX = 1l << 16; // buffer size
// constexpr ul SIZE = 4096 * 8; // bigger than sizeof(Node<T>), times of 4096

class Index {
public:
  char index[61] = "";
  char isbn[21] = "";

  Index() = default;

  ~Index() = default;

  Index(const std::string &index_, const std::string &isbn_ = "");

  bool operator>(const Index &rhs) const;

  bool operator<(const Index &rhs) const;

  bool operator>=(const Index &rhs) const;

  bool operator<=(const Index &rhs) const;

  bool operator==(const Index &rhs) const;

  Index operator=(const Index &rhs);

  friend std::ostream &operator<<(std::ostream &op, const Index &rhs);

  friend bool strictly_equal(const Index &lhs, const Index &rhs);
};

class Data {
public:
  char index[65] = {};
  int num = 0;

  Data() = default;

  ~Data() = default;

  Data(const std::string &str, const int &n) {
    num = n;
    strcpy(index, str.c_str());
  }

  bool operator<(const Data &y) const {
    const int res = strcmp(index, y.index);
    return res < 0;
  }

  bool operator>(const Data &y) const {
    const int res = strcmp(index, y.index);
    return res > 0;
  }

  bool operator==(const Data &y) const {
    const int res = strcmp(index, y.index);
    return res == 0;
  }

  bool operator<=(const Data &y) const { return !(*this > y); }

  bool operator>=(const Data &y) const { return !(*this < y); }

  Data operator=(const Data &y) {

    if (this == &y) {
      return *this;
    }

    num = y.num;
    strcpy(index, y.index);

    return *this;
  }

  friend std::istream &operator>>(std::istream &op, Data &rhs);

  friend std::ostream &operator<<(std::ostream &op, const Data &rhs);
};

inline bool cmp(const Data &lhs, const Data &rhs) { return lhs.num < rhs.num; }

template <class T> class Node {
public:
  ul next = 0l;
  ul size = 0l;
  T data[sqN] = {};

  Node() = default;

  Node(const T &val) {
    data[0] = val;
    ++size;
  }

  ~Node() = default;
};

template <class T> class List {
protected:
  std::string file_name = "data";
  std::fstream file;
  ul end = 0;    // sqrt(N)
  const ul SIZE; // bigger than sizeof(Node<T>), times of 4096

  void read(Node<T> &block) {
    file.read(reinterpret_cast<char *>(&block), sizeof(block));
  }

  void write(const Node<T> &block) {
    file.write(reinterpret_cast<const char *>(&block), sizeof(block));
  }

public:
  List(const std::string &name)
      : file_name(name), SIZE(((sizeof(Node<T>) + 4095) / 4096) * 4096) {

    file.open(file_name);
    if (!file.is_open()) {

      file.open(file_name, std::ios::out);

      Node<T> empty_block;
      const T val;
      // empty_block.data[0] = val;
      // empty_block.size = 1;
      write(empty_block);
      end += SIZE;

      file.close();
      file.open(file_name);
    } else {

      file.seekp(0, std::ios::end);
      const ul up = file.tellp();
      while (end < up) {
        end += SIZE;
      }
    }

    char buffer[MAX];
    file.rdbuf()->pubsetbuf(buffer, sizeof(buffer));
  }

  void insert(const T &val) {

    ul pos = 0;
    ul pre = pos;
    Node<T> block;

    do {
      file.seekg(pos);
      read(block);
      if (block.data[0] <= val && val <= block.data[block.size - 1]) {
        break;
      }
      pre = pos;
      pos = block.next;
    } while (pos != 0);

    if (pos == 0) {
      file.seekg(0);
      read(block);
      if (val >= block.data[0]) {
        file.seekg(pre);
        read(block);
        pos = pre;
      }
    }

    if (block.size < sqN) {

      T *const ptr = std::upper_bound(block.data, block.data + block.size, val);
      T *p = block.data + block.size - 1;
      while (p >= ptr) {
        *(p + 1) = *(p);
        --p;
      }
      *ptr = val;
      ++block.size;

      file.seekp(pos);
      write(block);
    } else {

      T *const ptrtmp =
          std::upper_bound(block.data, block.data + block.size, val);
      Node<T> breaker;
      breaker.size = sqN / 2;
      block.size = sqN / 2;
      for (int i = 0; i < sqN / 2; ++i) {
        breaker.data[i] = block.data[i + sqN / 2];
      }
      breaker.next = block.next;
      block.next = end;

      if (ptrtmp < block.data + sqN / 2) {

        T *const ptr =
            std::upper_bound(block.data, block.data + block.size, val);
        T *p = block.data + block.size - 1;
        while (p >= ptr) {
          *(p + 1) = *(p);
          --p;
        }
        *ptr = val;
        ++block.size;
      } else {

        T *const ptr =
            std::upper_bound(breaker.data, breaker.data + breaker.size, val);
        T *p = breaker.data + breaker.size - 1;
        while (p >= ptr) {
          *(p + 1) = *(p);
          --p;
        }
        *ptr = val;
        ++breaker.size;
      }

      file.seekp(pos);
      write(block);
      file.seekp(end);
      write(breaker);
      end += SIZE;
    }
  }

  virtual void
  erase(const T &val) { //删除字典序与val相同的值，如果有多个则全部删除

    ul pos = 0;
    Node<T> block;

    do {
      file.seekg(pos);
      read(block);
      if (block.data[0] <= val && val <= block.data[block.size - 1]) {

        bool is_found = false;
        T *const lt =
            std::lower_bound(block.data, block.data + block.size, val);
        T *const rt =
            std::upper_bound(block.data, block.data + block.size, val);

        if (lt != rt) {
          is_found = true;
          const int count = rt - lt;
          block.size -= count;
          for (int j = lt - block.data; j != block.size; ++j) {
            block.data[j] = block.data[j + count];
          }
        }

        if (is_found) {
          file.seekp(pos);
          write(block);
        }
      }

      pos = block.next;
    } while (pos != 0);
  }

  virtual bool update(
      const T &
          val) { // 将字典序与val相同的元素修改为val并返回true，若不存在则无操作并返回false
                 // 仅适用于无重复元素的情况

    ul pos = 0;
    Node<T> block;

    do {
      file.seekg(pos);
      read(block);
      if (block.data[0] <= val && val <= block.data[block.size - 1]) {

        bool is_found = false;
        T *const lt =
            std::lower_bound(block.data, block.data + block.size, val);

        if (*lt == val) {
          is_found = true;
          *lt = val;
        }

        if (is_found) {
          file.seekp(pos);
          write(block);
          return true;
        }
      }

      pos = block.next;
    } while (pos != 0);

    return false;
  }

  std::vector<T> *get(const T &val) {

    ul pos = 0;
    std::vector<T> *ans = new std::vector<T>;
    Node<T> block;

    ans->reserve(sqN);

    while (true) {

      file.seekg(pos);
      read(block);

      if (block.data[0] <= val && block.data[block.size - 1] >= val) {
        T *const lt =
            std::lower_bound(block.data, block.data + block.size, val);
        T *const rt =
            std::upper_bound(block.data, block.data + block.size, val);
        for (auto p = lt; p != rt; ++p) {
          ans->push_back(*p);
        }

        if (ans->size() > sqN) {
          ans->reserve(sqN);
        }
      }

      if (block.next == 0) {
        break;
      }

      pos = block.next;
    }

    // sort(ans->begin(), ans->end());
    return ans;
  }

  ul count(const T &val) {

    ul pos = 0;
    Node<T> block;
    ul count = 0;

    while (true) {

      file.seekg(pos);
      read(block);

      if (block.data[0] <= val && block.data[block.size - 1] >= val) {
        T *const lt =
            std::lower_bound(block.data, block.data + block.size, val);
        T *const rt =
            std::upper_bound(block.data, block.data + block.size, val);

        count += rt - lt;
      }

      if (block.next == 0) {
        break;
      }

      pos = block.next;
    }

    return count;
  }

  T find(const T &val) {

    ul pos = 0;
    Node<T> block;

    while (true) {

      file.seekg(pos);
      read(block);

      if (block.data[0] <= val && block.data[block.size - 1] >= val) {
        T *const lt =
            std::lower_bound(block.data, block.data + block.size, val);
        T *const rt =
            std::upper_bound(block.data, block.data + block.size, val);

        if (lt != rt) {
          return *lt;
        }
      }

      if (block.next == 0) {
        break;
      }

      pos = block.next;
    }

    return T();
  }

  void print() {

    ul pos = 0;
    Node<T> block;
    int i = 1;
    bool is_found = false;

    while (true) {

      file.seekg(pos);
      read(block);

      // std::cout << "block " << i << " size " << block.size << " next "
      //           << block.next << " :" << std::endl;
      if (block.size) {
        is_found = true;
      }
      for (int i = 0; i != block.size; ++i) {
        std::cout << block.data[i];
      }

      if (block.next == 0) {
        break;
      }

      pos = block.next;
      ++i;
    }

    if (!is_found) {
      std::cout << '\n';
    }
  }

  void clear() {
    file.close();
    remove(file_name.c_str());
  }

  virtual ~List() {
    file.close();
#ifdef DEBUG
    std::remove(file_name.c_str());
#endif
  }
};

class MultiList : public List<Index> {
public:
  void erase(const Index &val); //删除与val完全相同的值

  bool update(const Index &val_old,
              const Index &val_new); //更新与val_old完全相同的值，若不存在则插入

  MultiList(const std::string name);

  ~MultiList() = default;
};

#endif