cmake_minimum_required(VERSION 3.10)

project(Las VERSION 1.0)

add_executable(las src/commands/common/OSProxyUtils.cpp
                   src/commands/CommandExecutor.cpp
                   src/commands/CutCommand.cpp
                   src/commands/diff/DiffCommand.cpp
                   src/commands/common/OSCommandProxy.cpp
                   src/ui/CmdParser.cpp
                   src/executors/Las.cpp
                   src/main.cpp)

target_include_directories(las 
                           PRIVATE
                           ${PROJECT_SOURCE_DIR}/src/commands
                           ${PROJECT_SOURCE_DIR}/src/commands/common
                           ${PROJECT_SOURCE_DIR}/src/commands/diff
                           ${PROJECT_SOURCE_DIR}/src/ui
                           ${PROJECT_SOURCE_DIR}/src/executors)

#special target created as library for language requirements
add_library(language_req INTERFACE)
target_compile_features(language_req INTERFACE cxx_std_20)


target_link_libraries(las PUBLIC language_req)
