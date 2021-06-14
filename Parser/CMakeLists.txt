add_library(Parser STATIC
  Parser.cpp
)

target_include_directories(Parser PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../)

set_target_properties(Parser PROPERTIES COMPILE_FLAGS
  "-fno-rtti -std=c++14")

