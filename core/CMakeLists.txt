cmake_minimum_required(VERSION 3.22)

# Debug message
message("Entering ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")

# Register core library
add_library(vistmachine INTERFACE)
target_sources(vistmachine PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src/vistmachine.c)
target_include_directories(vistmachine INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/inc)

# Register other modules

# Debug message
message("Exiting ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")