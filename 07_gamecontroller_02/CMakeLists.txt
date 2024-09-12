cmake_minimum_required(VERSION 3.10)

project(07_gamecontroller_02 VERSION 1.0.0 LANGUAGES C CXX)

add_executable(main main.cpp)

include_directories(/home/y/桌面/SDL/include)

link_directories(/home/y/桌面/SDL/lib)

target_link_libraries(main /home/y/桌面/SDL/lib/libSDL2.so)
