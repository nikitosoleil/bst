cmake_minimum_required(VERSION 3.10)
project(Clion)

set(CMAKE_CXX_STANDARD 17)

add_executable(Clion Bst.cpp BstAvl.h BstTreap.h BstTwoThree.h BstRbt.h BstSplay.h Bst.h)