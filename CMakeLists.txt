cmake_minimum_required(VERSION 2.6)
project(pacman)

# Use our modified FindSDL2* modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${pacman_SOURCE_DIR}/cmake")
set(BIN_DIR ${pacman_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIR})

add_executable(pacman src/main.cpp src/field.cpp src/res_path.cpp)
target_link_libraries(pacman ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})
install(TARGETS pacman RUNTIME DESTINATION ${BIN_DIR})
