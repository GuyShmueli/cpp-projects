# Movie Recommendation System

A C++ 20 project illustrating two classic recommender-system techniques:

1. **Content-Based Filtering**  
   Builds a personalised preference vector from a user’s ratings and recommends  
   the unseen movie whose feature vector has highest cosine similarity.
2. **Item-Based Collaborative Filtering (k-NN)**  
   Predicts a score for each unseen movie by weighting the user’s own ratings  
   with cosine similarities between movies.

It comes with:
- **Loaders** for reading movie-feature and user-rating files.
- A **debug harness** (`print_main.cpp`) you can edit at will.
- A **presubmit harness** (`presubmit.cpp`) that the autograder uses (do **not** modify).

---

## Repository Layout
CMakeLists.txt
Movie.h
Movie.cpp
RecommendationSystem.h
RecommendationSystem.cpp
RecommendationSystemLoader.h
RecommendationSystemLoader.cpp
User.h
User.cpp
UsersLoader.h
UsersLoader.cpp
print_main.cpp # your editable debug/test driver
presubmit.cpp # official grading driver (keep pristine)
io/
---

## Prerequisites

- C++20-capable compiler (g++ ≥ 11, Clang ≥ 13, MSVC ≥ 19.30)  
- CMake ≥ 3.22

---

## Build & Run

```bash
git clone https://github.com/GuyShmueli/cpp-projects/recommendation-system.git
cd recommendation-system

# out-of-source build:
mkdir build && cd build
cmake ..
cmake --build .
# run your debug harness:
./recommendation-system
