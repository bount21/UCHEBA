cmake_minimum_required(VERSION 4.2)
project(2_MergeSort)

set(CMAKE_CXX_STANDARD 14)

add_executable(2_MergeSort
        merge.cpp
        merge.exe
        numbers.txt
        numbersfill.cpp
        numbersfill.exe
        output.txt)
