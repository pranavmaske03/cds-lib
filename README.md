# cds-lib — Custom Data Structure Library

A cds (custom data structure) library written in C++ where common STL-like 
data structures are implemented as a learning project to deeply understand 
data structures and algorithms.

---

## The data structures implemented

| Header | Class | Description |
|---|---|---|
| `singly_list.hpp` | `cds::singly_list<T>` | Singly linear linked list |
| `doubly_list.hpp` | `cds::doubly_list<T>` | Doubly linear linked list |
| `singly_circular.hpp` | `cds::singly_circular<T>` | Singly circular linked list |
| `doubly_circular.hpp` | `cds::doubly_circular<T>` | Doubly circular linked list |
| `stack.hpp` | `cds::stack<T>` | LIFO stack |
| `queue.hpp` | `cds::queue<T>` | FIFO queue |
| `bst.hpp` | `cds::bst<T>` | Binary search tree |
| `array.hpp` | `cds::array<T>` | Array with search and sort |

---

## How to use this library

Clone the repo and include the header you need.
```bash
git clone https://github.com/yourname/cds-lib.git
```
```cpp
#include "include/stack.hpp"

int main() {
  cds::stack<int> s;
  s.push(10);
  s.push(20);
  std::cout << s.top();
  s.pop();
}
```

## Compile example

```bash
g++ main.cpp -o myexe
./myexe