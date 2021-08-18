add_library(Dominance STATIC
  DominatorsAnalysis.cpp
)
target_include_directories(Dominance PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../../)

set_target_properties(Dominance PROPERTIES COMPILE_FLAGS
  "-fno-rtti -std=c++14")


#--------------------------------- driver --------------------------------------

add_executable(dom-driver dom-driver.cpp)

target_include_directories(dom-driver PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../../)

target_link_libraries(dom-driver Dominance Parser IR)

set_target_properties(dom-driver PROPERTIES COMPILE_FLAGS
  "-fno-rtti -std=c++14")

# Add symlink in top level build directory
add_custom_command(
  TARGET dom-driver POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink
  ${CMAKE_CURRENT_BINARY_DIR}/dom-driver
  ${CMAKE_CURRENT_BINARY_DIR}/../../dom-driver
)
