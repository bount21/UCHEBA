cmake_minimum_required(VERSION 4.2)
project(Markov_in_chains)

set(CMAKE_CXX_STANDARD 20)

add_executable(markov cmake-build-debug/
        Markov's_chains.cpp)
