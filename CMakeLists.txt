cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 17)

project(metal-cpp-bug)

find_package(glfw3 CONFIG REQUIRED)
find_library(METAL_LIBRARY Metal)
find_library(QUARTZCORE_LIBRARY QuartzCore)

add_executable(metal-test main.cpp glfw_bridge.mm)
target_include_directories(metal-test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp/)
target_link_libraries(metal-test PRIVATE glfw ${METAL_LIBRARY} ${QUARTZCORE_LIBRARY})
