cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME "Julia")

project("${PROJECT_NAME}" VERSION 1.0)

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
pkg_check_modules(glfw3 REQUIRED IMPORTED_TARGET glfw3)

include_directories(${glfw3_INCLUDE_DIRS})
link_directories(${glfw3_LIBRARY_DIRS})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR "src")
file(GLOB SRC_FILES "${SRC_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE headers)
target_link_libraries(${PROJECT_NAME} PkgConfig::glfw3 OpenGL::GL)

add_custom_target(
  run
  COMMAND ${PROJECT_NAME}
  DEPENDS ${PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})
