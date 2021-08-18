cmake_minimum_required(VERSION 3.20)

add_subdirectory(IR)
add_subdirectory(Parser)
add_subdirectory(Concepts)


# Tool to work with scil CFGs
add_executable(scilt scilt.cpp)

target_include_directories(scilt PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(scilt IR Parser)

set_target_properties(scilt PROPERTIES COMPILE_FLAGS
  "-fno-rtti -std=c++14")
