cmake_minimum_required(VERSION 3.30)
project(coursework)

set(CMAKE_CXX_STANDARD 20)

add_executable(coursework main.cpp
        AlphabetHelper.h
        CodeHelper.h
        StreamGuard.h
        ConsoleHelper.h
        SmartBoolArray.cpp
        SmartBoolArray.h
        smartBoolArrayExceptions.h
)
