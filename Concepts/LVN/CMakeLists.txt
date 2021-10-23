add_library(LVN STATIC
  LVN.cpp
  LVNPass.cpp
)
target_include_directories(LVN PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../../)

set_target_properties(LVN PROPERTIES COMPILE_FLAGS
  "-fno-rtti -std=c++14")


#--------------------------------- driver --------------------------------------

add_executable(lvn-driver lvn-driver.cpp)

target_include_directories(lvn-driver PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../../)

target_link_libraries(lvn-driver LVN Parser IR)

set_target_properties(lvn-driver PROPERTIES COMPILE_FLAGS
  "-fno-rtti -std=c++14")

# Add symlink in top level build directory
add_custom_command(
  TARGET lvn-driver POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink
  ${CMAKE_CURRENT_BINARY_DIR}/lvn-driver
  ${CMAKE_CURRENT_BINARY_DIR}/../../lvn-driver
)
