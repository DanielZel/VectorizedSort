cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 14)
project(NEONSort VERSION 0.1.0)
set(EXPORT_COMPILE_COMMANDS ON)
FIND_PACKAGE(Boost 1.56.0 REQUIRED)
add_executable(NEONSort src/main.cpp)
target_include_directories(NEONSort PRIVATE include)
set(CMAKE_BUILD_TYPE Release)
