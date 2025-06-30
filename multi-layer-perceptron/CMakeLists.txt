cmake_minimum_required(VERSION 3.22)
project(EX4_updated)

set(CMAKE_CXX_STANDARD 11)

add_executable(EX4_updated main.cpp
        Matrix.h
        Matrix.cpp
        Dense.h
        Dense.cpp
        Activation.h
        Activation.cpp
        MlpNetwork.h
        MlpNetwork.cpp
        autotest_utils.h)
