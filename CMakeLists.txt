cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project(raymarch)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${LIBIGL_INCLUDE_DIR})

add_library(lodepng STATIC lib/lodepng/lodepng.cpp)

include_directories(${PROJECT_SOURCE_DIR}/lib/lodepng)
link_directories(${PROJECT_SOURCE_DIR}/lib/lodepng)

file(GLOB H_FILES ${PROJECT_SOURCE_DIR}/include/*.h)
file(GLOB CPP_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

find_package (Eigen3 3.3.9 REQUIRED NO_MODULE)

set(CMAKE_CXX_FLAGS -pthread)

add_compile_options(-Wall -Wextra -pedantic -Werror -O3)

add_executable(${PROJECT_NAME} main.cpp ${H_FILES} ${CPP_FILES})

target_link_libraries (${PROJECT_NAME} Eigen3::Eigen lodepng)

