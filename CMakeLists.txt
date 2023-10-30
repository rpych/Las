cmake_minimum_required(VERSION 3.10)

project(Las VERSION 1.0)

add_executable(las src/main.cpp
                   src/commands/CommandExecutor.cpp
                   src/commands/CutCommand.cpp)

target_include_directories(las 
                           PRIVATE
                           ${PROJECT_SOURCE_DIR}/src/commands)

#special target created as library for language requirements
add_library(language_req INTERFACE)
target_compile_features(language_req INTERFACE cxx_std_20)


target_link_libraries(las PUBLIC language_req)