cmake_minimum_required(VERSION 3.26.4)
project(Project1)

set(CMAKE_CXX_STANDARD 14)

include_directories(main)
include_directories(main/include)


add_executable(Project1
        main/include/Branch.h
        main/include/Node.cpp
        main/include/Node.h
        main/include/PathORAM.cpp
        main/include/PathORAM.h
        main/include/Pdefault.cpp
        main/include/Pdefault.h
        main/include/Ptree.h
        main/test.cpp)
target_link_libraries(Project1 cryptopp.a)