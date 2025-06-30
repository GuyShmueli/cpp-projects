cmake_minimum_required(VERSION 3.22)
project(recommendation_system)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(recommendation_system
        Movie.cpp
        Movie.h
        RecommendationSystem.cpp
        RecommendationSystem.h
        RecommendationSystemLoader.cpp
        RecommendationSystemLoader.h
        User.cpp
        User.h
        UsersLoader.cpp
        UsersLoader.h
        testing.cpp
        )
