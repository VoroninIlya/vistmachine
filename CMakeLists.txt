cmake_minimum_required(VERSION 3.22)

project(tst_vistmachine)

#include(CodeCoverage)
include(CTest)

if(PROJECT_IS_TOP_LEVEL)
# Set as executable
add_subdirectory(${UNITY_ROOT_PATH} ${${EXECUTABLE}_BINARY_DIR}/modules/unity)
endif()

# Add subdir with vistmachine
add_subdirectory(core)

add_executable(tst_vistmachine)
enable_testing()

# Add key executable block
target_sources(tst_vistmachine PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/tests/main/main.c
    ${CMAKE_CURRENT_LIST_DIR}/tests/tst_vistmachine.c
)

# Add key include paths
target_include_directories(tst_vistmachine PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/core/str/inc
    ${UNITY_ROOT_PATH}/src
    ${UNITY_ROOT_PATH}/extras/fixture/src
    ${UNITY_ROOT_PATH}/extras/memory/src
)

# Compilation definition information
target_compile_definitions(tst_vistmachine PUBLIC
    WIN32
    _DEBUG
    CONSOLE
)

# Compiler options
target_compile_options(tst_vistmachine PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

target_link_libraries(
  tst_vistmachine vistmachine unity)

add_test(NAME VISTMACHINE_Init COMMAND tst_vistmachine "--gtest_filter=VISTMACHINE_Init.*")
add_test(NAME VISTMACHINE_AddState COMMAND tst_vistmachine "--gtest_filter=VISTMACHINE_AddState.*")
add_test(NAME VISTMACH_Runtime COMMAND tst_vistmachine "--gtest_filter=VISTMACH_Runtime.*")
