# MLP - A Feed-Forward Neural-Network in C++

## Features
- **Matrix** class with basic linear-algebra ops (`+`, `*`, dot product, RREF, argmax, norm…)
- **Activation** namespace with `relu()` and `softmax()`
- **Dense** layer wrapper (`W · x + b` followed by activation)
- **MlpNetwork** that chains 4 Dense layers and returns the predicted digit + probability
- Exception-safe RAII (copy-&-swap); minimal STL usage (only `<cmath>` / `<iostream>`)

## Folder layout
├── Activation.h // activation::relu / activation::softmax    
├── Dense.h // Dense layer class    
├── Matrix.h // Matrix declaration + error strings/macros    
├── MlpNetwork.h // MLP wrapper    
├── Activation.cpp    
├── Dense.cpp    
├── Matrix.cpp    
├── MlpNetwork.cpp    
└── main.cpp    

## Building

### Prerequisites
* C++17-compatible compiler
* CMake ≥ 3.10

### Usage
```bash
# ---- Test-suite executable (RUN_CLI OFF) ----
# 1. Configure **in a separate build folder** with the switch OFF
cmake -S . -B build-tests -DRUN_CLI=OFF                    

# 2. Compile
cmake --build build-tests -j                              

# 3. Run the self-checks
cd build-tests
./mlp_tests                 # plain execution (prints “All tests passed” …)
ctest --output-on-failure   # same, but through CTest for CI integration

# ---- CLI executable (RUN_CLI ON, the default) ----
# 1. Configure a fresh directory (RUN_CLI defaults to ON)
cmake -S . -B build-cli -DCMAKE_BUILD_TYPE=Release         

# 2. Compile
cmake --build build-cli -j                                 

# 3. Run the interactive digit-classifier
cd build-cli
./mlp w1.bin w2.bin w3.bin w4.bin b1.bin b2.bin b3.bin b4.bin
# …then follow the prompt:
#   Enter image path (or 'q' to quit): digit_7.img




