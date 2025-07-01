# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(mlp_network LANGUAGES CXX)

# compiler setup 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# build-mode switch 
option(RUN_CLI "Build the interactive CLI (ON) or the self-test executable (OFF)" ON)

# common sources 
set(COMMON_SRCS
    Matrix.cpp   Matrix.h
    Dense.cpp    Dense.h
    Activation.cpp Activation.h
    MlpNetwork.cpp MlpNetwork.h)

# CLI build
if (RUN_CLI)
    add_executable(mlp
        main.cpp           # entry-point selector
        ${COMMON_SRCS})

    # Tell the compiler to define RUN_CLI only for this target
    target_compile_definitions(mlp PRIVATE RUN_CLI)

# Test build 
else()
    add_executable(mlp_tests
        main.cpp           # same entry point, but RUN_CLI **not** defined
        tests.cpp          # extra test cases / helpers (if you have them)
        autotest_utils.h
        ${COMMON_SRCS})

    # Hook the test executable into CTest (uses its exit code for pass/fail)
    enable_testing()
    add_test(NAME mlp_unit COMMAND mlp_tests)
endif()
