# stackheap-vector
automatic  stack → heap  promotion

📦 VL Containers (vl_vector & vl_string)

A lightweight, header‑only re‑implementation of std::vector and std::string that automatically toggles between stack and heap storage.  Designed for efficient memory management in modern C++.

## **Table of Contents**  

1. Motivation

2. Key Features

3. How Memory Management Works

4. Complexity Guarantees

5. Building & Testing

6. Quick Start Examples

7. API Overview

8. Contributing


## **1. Motivation**  
Most C++ courses teach std::vector first, then explain its amortised reallocation cost.  This project pushes one step further: it stores the first elements on the stack (default C = 16), only switching to the heap when necessary.  The same trick is applied to a std::string‑like wrapper so that short strings never hit the heap.

This project outline:

* Starting from static capacity on the stack.

* A capacity‐growth rule ⌊3 · (new_size)/2⌋ once on the heap, giving a growth factor ≈ 1.5, which balances runtime vs. memory reuse.

* Strict O(1) (amortised) push_back, pop_back, and element access.

* Full STL‑compatible random‑access iterators.

I have implemented two reusable headers and wrapped them with modern C++ idioms (rule‑of‑zero where possible, copy‑and‑swap for assignment, etc.).

## **2. Key Features**  

| Feature                                            | `vl_vector` | `vl_string` |
|----------------------------------------------------|-------------|-------------|
| Stack ➡️ Heap promotion                            | ✔️          | ✔️          |
| Custom static capacity (`template<size_t C>`)      | ✔️          | ✔️          |
| Random-access & const / reverse iterators          | ✔️          | ✔️          |
| `push_back`, `insert`, `erase`, `clear`, `data`, … | ✔️          | inherits    |
| `operator+=`, implicit cast to `std::string`       | –           | ✔️          |
| Header-only, no STL containers inside              | ✔️          | ✔️          |

## **3. How Memory Management Works** 

*  Stack Phase – static buffer

Each instance owns a fixed array T data_static[C].  As long as size() ≤ C all operations manipulate this array—no heap, zero allocations.

*  Heap Phase – dynamic buffer

When an insertion would make size() > C, we allocate a new buffer whose capacity is

capacity = ⌊ 3·(size + k) / 2 ⌋      // k = number of elements being inserted

This gives a growth factor of ≈1.5, which is a sweet‑spot between doubling (wastes memory) and linear growth (too many reallocations).

We copy existing elements into the new buffer, then free the old one.  When erase() brings size() back to ≤ C, we migrate back to the stack and discard the heap buffer.

Why not shrink heap capacity?  The spec prohibits down‑sizing while on the heap to keep erase() *O(1)`.

## **4. Complexity Guarantees**

Operation

Complexity

operator[], at()

O(1)

push_back, pop_back

Amortised O(1)

insert, erase (single)

O(n)

insert, erase (range)

O(n + m) where m is range length

Iteration

O(n) total, O(1) per step

## **5. Building & Testing**

Prerequisites

Compiler: g++ 10+ or clang++ 12+ with C++20 (-std=c++20).

Tools: cmake (optional), valgrind for leak checking.

Quick build (single header project)

# Clone repository
$ git clone https://github.com/<your‑username>/vl_containers.git
$ cd vl_containers

# Compile sample driver (uses both headers)
$ g++ -std=c++20 -Wall -Wextra -pedantic examples/main.cpp -o demo

# Run & check for leaks
$ valgrind --leak-check=full ./demo

Running the original presubmit tests

The assignment shipped with presubmit.cpp.  If you still have it:

$ g++ -std=c++20 -I./include -Wall -Wextra -pedantic presubmit.cpp -o presubmit
$ ./presubmit   # all tests should pass

## **6. Quick Start Examples**

Using vl_vector

#include "vl_vector.hpp"
#include <iostream>

int main() {
    vl_vector<int, 8> vec;   // 8 ints fit on the stack
    for (int i = 0; i < 20; ++i) vec.push_back(i);
    vec.erase(vec.begin() + 5, vec.begin() + 10);
    for (auto v : vec) std::cout << v << ' ';
}

Using vl_string

#include "vl_string.hpp"
#include <iostream>

int main() {
    vl_string<> s("Hello");          // default static capacity = 16
    s += ", ";                       // char const*
    vl_string<> w("World");
    s += w;                            // vl_string
    s += '!';                          // single char
    std::string stds = s;              // implicit conversion
    std::cout << stds << '\n';
}

## **7. API Overview**

The public interface mirrors std::vector / std::string where sensible.  Below is an abridged list; see the header for full docs.

// vl_vector<T, C>
size_t size()      const;
size_t capacity()  const;
bool   empty()     const;
T&       at(size_t idx);        const T& at(size_t idx) const;
T&       operator[](size_t);    const T  operator[](size_t) const;
void     push_back(const T&);
iterator insert(iterator pos, const T& value);
iterator insert(iterator pos, ForwardIt first, ForwardIt last);
void     pop_back();
iterator erase(iterator pos);
iterator erase(iterator first, iterator last);
void     clear();
T*       data();                const T* data() const;
// + full iterator typedefs & helpers

// vl_string<C> : thin wrapper around vl_vector<char, C>
using size_type = size_t;
size_type size()    const;
bool      empty()   const;
const char* data()  const;      // not null‑terminated
operator std::string() const;   // implicit cast
vl_string& operator+=(const char*);
vl_string& operator+=(const vl_string&);
vl_string& operator+=(char);

## **8. Contributing**

Pull requests are welcome!  If you spot undefined behaviour or want additional STL‑like conveniences (shrink_to_fit, reserve, …) open an issue first so we can discuss the design cost.

Development hints while contributing:  

* Run valgrind regularly—zero leaks is a hard requirement.

* Keep O(1) amortised push_back in mind when proposing changes.

* Follow ISO C++ Core Guidelines for style.


