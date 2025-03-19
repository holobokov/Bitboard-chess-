cmake_minimum_required(VERSION 3.30)
project(bbc)

set(CMAKE_CXX_STANDARD 20)

add_executable(bbc main.cpp Bitboard.cpp Pieces.cpp Pieces.h BitboardClass.h BitboardClass.cpp)
