cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME sref)

project(${PROJECT_NAME} LANGUAGES CXX)

# 生成SourceGroup
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp" "src/*.h")
foreach(SOURCE_FILE ${SOURCES})
    get_filename_component(SOURCE_DIR ${SOURCE_FILE} DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" SOURCE_GROUP ${SOURCE_DIR})
    string(REPLACE "/" "\\" SOURCE_GROUP ${SOURCE_GROUP})
    source_group(${SOURCE_GROUP} FILES ${SOURCE_FILE})
endforeach()

add_executable( ${PROJECT_NAME} main.cpp ${SOURCES})
