cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME "Julia")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project("${PROJECT_NAME}" VERSION 1.0)

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
pkg_check_modules(glfw3 REQUIRED IMPORTED_TARGET glfw3)

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${glfw3_INCLUDE_DIRS})
link_directories(${glfw3_LIBRARY_DIRS})

set(SRC_DIR "src")
file(GLOB SRC_FILES "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.c")

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE headers include)
target_link_libraries(${PROJECT_NAME} PkgConfig::glfw3 OpenGL::GL)

if(NOT DEFINED FRAG_PATH)
  target_compile_definitions(${PROJECT_NAME} PRIVATE
    FRAG_PATH="${CMAKE_SOURCE_DIR}/shaders/fragment.glsl"
  )
endif()

if(NOT DEFINED VERT_PATH)
  target_compile_definitions(${PROJECT_NAME} PRIVATE
    VERT_PATH="${CMAKE_SOURCE_DIR}/shaders/vertex.glsl"
  )
endif()

add_custom_target(
  run
  COMMAND ${PROJECT_NAME}
  DEPENDS ${PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  target_compile_options(${PROJECT_NAME} PRIVATE "-Ofast")
endif()
