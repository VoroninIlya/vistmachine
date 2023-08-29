cmake_minimum_required(VERSION 3.22)

# Setup project
project(tst_vistmachine)

if(NOT PROJECT_IS_TOP_LEVEL)

    option(INCLUDE_TEST_SOURCES "Include test sources to sources_list." OFF)

    if(${INCLUDE_TEST_SOURCES})
        get_property(tmp GLOBAL PROPERTY tst_sources_list)
        set(tmp "${tmp}${CMAKE_CURRENT_SOURCE_DIR}/tests/tst_vistmachine.c;")
        set_property(GLOBAL PROPERTY tst_sources_list "${tmp}")

        get_property(tmp GLOBAL PROPERTY tst_include_paths_list)
        set(tmp "${tmp}<$<BOOL:${TESTS_ENABLED}>:core/src/inc>;")
        set_property(GLOBAL PROPERTY tst_include_paths_list "${tmp}")
    endif()

    add_subdirectory(core)

else()
    # Set as executable
    add_executable(${PROJECT_NAME})

    # Add key executable block
    target_sources(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/tests/main/main.c
        ${CMAKE_CURRENT_LIST_DIR}/tests/tst_vistmachine.c
    )

    # Add key include paths
    target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/core/str/inc
        ${UNITY_ROOT_PATH}/src
        ${UNITY_ROOT_PATH}/extras/fixture/src
        ${UNITY_ROOT_PATH}/extras/memory/src
    )

    # Compilation definition information
    target_compile_definitions(${PROJECT_NAME} PUBLIC
        WIN32
        _DEBUG
        CONSOLE
    )

    # Compiler options
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )

    # Add subproject unity
    add_subdirectory(${UNITY_ROOT_PATH} ${PROJECT_NAME}/build)
    # Add subdir with vistmachine and link to project
    add_subdirectory(core)

    target_link_libraries(${PROJECT_NAME} vistmachine)
    target_link_libraries(${PROJECT_NAME} unity)
endif()