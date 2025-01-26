cmake_minimum_required(VERSION 3.10)
project(latex_scanner)

set(CMAKE_CXX_STANDARD 17)
find_package(SQLite3 REQUIRED)

add_executable(latex_scanner latex_scanner.cpp)

target_link_libraries(latex_scanner PRIVATE SQLite::SQLite3) 