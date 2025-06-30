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
└── mnist_demo.cpp // example main() – load weights & predict

## Building
