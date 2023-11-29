cmake_minimum_required(VERSION 3.24)
project(Las VERSION 1.0)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/b10fad38c4026a29ea6561ab15fc4818170d1c10.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  las_test
  ${PROJECT_SOURCE_DIR}/testcases/example.cc
)

target_link_libraries(
  las_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(las_test)
