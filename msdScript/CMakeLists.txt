cmake_minimum_required(VERSION 3.27)
project(CS6015Project)

set(CMAKE_CXX_STANDARD 17)

add_executable(CS6015Project main.cpp
        cmdline.cpp
        cmdline.h
        Expr.cpp
        Expr.h
        catch.cpp
        catch.h)
