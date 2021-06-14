add_library(IR STATIC
  Value.cpp
  Instruction.cpp
)

target_include_directories(IR PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../)

set_target_properties(IR PROPERTIES COMPILE_FLAGS
  "-fno-rtti -std=c++14")
