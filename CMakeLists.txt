cmake_minimum_required(VERSION 3.24)

project(Las VERSION 1.0)

set(LAS_FILES_SRC
    src/commands/common/OSProxyUtils.cpp
    src/commands/CommandExecutor.cpp
    src/commands/cut/CutCommand.cpp
    src/commands/diff/DiffCommand.cpp
    src/commands/diff/DiffHeadCommand.cpp
    src/commands/diff/DiffStagedCommand.cpp
    src/commands/restore/RestoreCommand.cpp
    src/commands/common/OSCommandProxy.cpp
    src/commands/common/parsers/FileParser.cpp
    src/commands/common/parsers/HunksParser.cpp
    src/commands/common/writers/CodeWriterExclude.cpp
    src/commands/common/writers/FileWriter.cpp
    src/ui/CmdParser.cpp
    src/executors/Las.cpp)

set(LAS_FILES_INCLUDE
    ${PROJECT_SOURCE_DIR}/src/commands
    ${PROJECT_SOURCE_DIR}/src/commands/common
    ${PROJECT_SOURCE_DIR}/src/commands/common/parsers
    ${PROJECT_SOURCE_DIR}/src/commands/common/writers
    ${PROJECT_SOURCE_DIR}/src/commands/common/osProxy
    ${PROJECT_SOURCE_DIR}/src/commands/cut
    ${PROJECT_SOURCE_DIR}/src/commands/diff
    ${PROJECT_SOURCE_DIR}/src/commands/restore
    ${PROJECT_SOURCE_DIR}/src/ui
    ${PROJECT_SOURCE_DIR}/src/executors
    ${PROJECT_SOURCE_DIR}/src)

add_executable(las src/main.cpp ${LAS_FILES_SRC})

target_include_directories(las 
                           PRIVATE
                           ${LAS_FILES_INCLUDE})

#special target created as library for language requirements
add_library(language_req INTERFACE)
target_compile_features(language_req INTERFACE cxx_std_20)


target_link_libraries(las PUBLIC language_req)
#target_compile_definitions(las PUBLIC LAS_DEBUG)

add_library(las_lib STATIC ${LAS_FILES_INCLUDE} ${LAS_FILES_SRC} )

target_compile_features(las_lib PRIVATE cxx_std_20)

enable_testing()
add_subdirectory(test)
set(WORK_DIRECTORY_FOR_TESTS
    ${PROJECT_SOURCE_DIR})
add_test(NAME LasTestParametrized COMMAND las_test WORKING_DIRECTORY ${WORK_DIRECTORY_FOR_TESTS})

