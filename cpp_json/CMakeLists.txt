cmake_minimum_required(VERSION 3.19)
project(cpp_json)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp_json main.cpp JsonAPI/Json.h JsonAPI/Json.cpp JsonAPI/Jsonparser.h
        JsonAPI/Jsonparser.cpp JsonAPI/Jsonstringifier.h JsonAPI/Jsonstringifier.cpp
        JsonAPI/stack.h JsonAPI/stack.cpp
        )