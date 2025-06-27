cmake_minimum_required(VERSION 3.10)
project(Part4Tests LANGUAGES CXX)

# Find GoogleTest
find_package(GTest REQUIRED)

# Add test source
add_executable(test_vector test/test_vector.cpp)

# Link with GoogleTest and pthread (required by GTest)
target_link_libraries(test_vector PRIVATE GTest::gtest GTest::gtest_main pthread)

# Add include directories
target_include_directories(test_vector PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Register test
enable_testing()
add_test(NAME test_vector COMMAND test_vector)