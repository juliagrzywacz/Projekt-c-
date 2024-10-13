cmake_minimum_required(VERSION 3.28)
project(App)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH "C:/Qt/6.7.1/mingw_64/lib/cmake")

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(App main.cpp)

target_link_libraries(App Qt6::Widgets)