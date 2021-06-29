cmake_minimum_required(VERSION 3.20)

add_subdirectory(IR)
add_subdirectory(Parser)


# Tool to work with SIL CFGs
add_executable(silt silt.cpp)

target_include_directories(silt PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(silt IR Parser)

set_target_properties(silt PROPERTIES COMPILE_FLAGS
  "-fno-rtti -std=c++14")
