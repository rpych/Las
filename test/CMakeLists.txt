include_directories (${Las_SOURCE_DIR})
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(
  las_test
  ${PROJECT_SOURCE_DIR}/test/testcases/LasTest.cpp
)

target_include_directories(las_test
                           PRIVATE
                           ${PROJECT_SOURCE_DIR}/
                           ${PROJECT_SOURCE_DIR}/testcases)

add_library(language_req_test INTERFACE)
target_compile_features(language_req_test INTERFACE cxx_std_20)

target_link_libraries(las_test PUBLIC language_req_test)
target_link_libraries(las_test PUBLIC las_lib)

target_link_libraries(
  las_test
  PUBLIC
  GTest::gtest_main
)
target_compile_definitions(las_test PUBLIC LAS_DEBUG)

include(GoogleTest)
gtest_discover_tests(las_test)
