cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_CXX_STANDARD 11)

project(LAB3 C CXX)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(include)
file(GLOB SOURCES "*.cpp")
add_executable(main.out ${SOURCES})

# Link threading lib
target_link_libraries(main.out Threads::Threads)
