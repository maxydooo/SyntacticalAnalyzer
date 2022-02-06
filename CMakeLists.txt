cmake_minimum_required(VERSION 3.17)
project(lankfordP2)

set(CMAKE_CXX_STANDARD 14)

add_executable(lankfordP2 Lexical_Analyzer.h
        Project2.cpp Set_Limits.h Set_Limits.cpp
        Syntactical_Analyzer.h Syntactical_Analyzer.cpp)