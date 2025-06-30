# MLP - A Feed-Forward Neural-Network in C++

## Features
- **Matrix** class with basic linear-algebra ops (`+`, `*`, dot product, RREF, argmax, norm…)
- **Activation** namespace with `relu()` and `softmax()`
- **Dense** layer wrapper (`W · x + b` followed by activation)
- **MlpNetwork** that chains 4 Dense layers and returns the predicted digit + probability
- Exception-safe RAII (copy-&-swap); minimal STL usage (only `<cmath>` / `<iostream>`)

## Folder layout
include/
├── Activation.h // activation::relu / activation::softmax    
├── Dense.h // Dense layer class    
├── Matrix.h // Matrix declaration + error strings/macros    
├── MlpNetwork.h // MLP wrapper    
src/
├── Activation.cpp    
├── Dense.cpp    
├── Matrix.cpp    
├── MlpNetwork.cpp    
tests/
└── mnist_demo.cpp    

## Building

### Prerequisites
* C++17-compatible compiler (GCC ≥ 7, Clang ≥ 5, MSVC ≥ 19.14)
* CMake ≥ 3.10 (optional, recommended)

### Quick one-liner (G++)
```bash
# release build
g++ -std=c++17 -O2 -Wall -Wextra \
    src/*.cpp  main.cpp \
    -o mlp            # CLI build

# test-suite build
sed -i 's/^#define RUN_CLI/\/\/#define RUN_CLI/' main.cpp
g++ -std=c++17 -O2 src/*.cpp  main.cpp -o mlp_tests

mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release   # CLI build
cmake --build . -j

# Tests:
cmake .. -DRUN_CLI=OFF
cmake --build . -j

./mlp w1.bin w2.bin w3.bin w4.bin b1.bin b2.bin b3.bin b4.bin
Enter image path (or 'q' to quit):  two_sample_images/3.bin

Prediction: 3  (p = 0.987)
