cmake_minimum_required(VERSION 3.14)
project(MessageBroker VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(broker_lib STATIC
    src/Manager.cpp
    src/Module.cpp
)
target_include_directories(broker_lib PUBLIC include)

add_executable(demo example/main.cpp)
target_link_libraries(demo PRIVATE broker_lib)

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()