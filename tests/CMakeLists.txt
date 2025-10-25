cmake_minimum_required(VERSION 3.16)

# Find Google Test
find_package(GTest REQUIRED)

# Find SFML for tests
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Test executable
add_executable(imageprocessing_test
    test_image.cpp
    test_filters.cpp
    test_pipeline.cpp
    test_integration.cpp
)

target_link_libraries(imageprocessing_test
    imageprocessing
    GTest::GTest
    GTest::Main
    sfml-graphics
    sfml-window
    sfml-system
)

# Enable testing
enable_testing()
add_test(NAME ImageProcessingTest COMMAND imageprocessing_test)

# Add Google Test discovery
include(GoogleTest)
gtest_discover_tests(imageprocessing_test)

# Add coverage support
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    find_program(GCOV_PATH gcov)
    find_program(LCOV_PATH lcov)
    find_program(GENHTML_PATH genhtml)
    
    if(GCOV_PATH AND LCOV_PATH AND GENHTML_PATH)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --coverage -fprofile-arcs -ftest-coverage")
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage")
        
        add_custom_target(coverage
            COMMAND ${LCOV_PATH} --directory . --capture --output-file coverage.info
            COMMAND ${LCOV_PATH} --remove coverage.info '/usr/*' '*/tests/*' --output-file coverage.info.cleaned
            COMMAND ${GENHTML_PATH} -o coverage coverage.info.cleaned
            COMMAND ${CMAKE_COMMAND} -E remove coverage.info coverage.info.cleaned
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating code coverage report"
        )
    endif()
endif()
