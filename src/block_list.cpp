#include "block_list.hpp"

void MultiList::erase(const Index &val) { //删除与val完全相同的值

  ul pos = 0;
  Node<Index> block;

  do {
    file.seekg(pos);
    read(block);
    if (block.data[0] <= val && val <= block.data[block.size - 1]) {

      bool is_found = false;
      for (int i = 0; i != block.size; ++i) {
        if (strictly_equal(val, block.data[i])) {
          is_found = true;
          --block.size;
          for (int j = i; j != block.size; ++j) {
            block.data[j] = block.data[j + 1];
          }
          break;
        }
      }

      if (is_found) {
        file.seekp(pos);
        write(block);
        return;
      }
    }

    pos = block.next;
  } while (pos != 0);
}

MultiList::MultiList(const std::string name) : List<Index>(name) {}

bool MultiList::update(const Index &val_old, const Index &val_new) {
  ul pos = 0;
  Node<Index> block;

  do {
    file.seekg(pos);
    read(block);
    if (block.data[0] <= val_old && val_old <= block.data[block.size - 1]) {

      bool is_found = false;
      for (int i = 0; i != block.size; ++i) {
        if (strictly_equal(val_old, block.data[i])) {
          is_found = true;
          block.data[i] = val_new;
          break;
        }
      }

      if (is_found) {
        file.seekp(pos);
        write(block);
        return true;
      }
    }

    pos = block.next;
  } while (pos != 0);

  insert(val_new);
  return false;
}