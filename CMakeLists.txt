cmake_minimum_required(VERSION 3.10)

project(Renderer)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 指定源文件目录
file(GLOB_RECURSE SOURCE_FILES 
      "${PROJECT_SOURCE_DIR}/src/*.cpp")

# 指定头文件目录
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/src/graphics")
include_directories("${PROJECT_SOURCE_DIR}/src/math")
include_directories("${PROJECT_SOURCE_DIR}/src/objects")
include_directories("${PROJECT_SOURCE_DIR}/src/scene")

# 生成可执行文件
add_executable(Renderer ${SOURCE_FILES})