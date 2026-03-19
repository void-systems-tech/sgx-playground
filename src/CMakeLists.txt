cmake_minimum_required(VERSION 3.11)
project(sgx LANGUAGES C CXX)

if (UNIX)
  if (NOT DEFINED ENV{CC} AND NOT DEFINED CMAKE_C_COMPILER)
    find_program(CMAKE_C_COMPILER clang-11 clang-10 clang)
  endif ()
  if (NOT DEFINED ENV{CXX} AND NOT DEFINED CMAKE_CXX_COMPILER)
    find_program(CMAKE_CXX_COMPILER clang++-11 clang++-10 clang++)
  endif ()
endif ()

find_package(OpenEnclave CONFIG REQUIRED)

add_library(project_includes INTERFACE)
target_include_directories(
  project_includes
  INTERFACE
  ${CMAKE_SOURCE_DIR}/../include
)

add_subdirectory(datasealing)
add_subdirectory(enclave)
add_subdirectory(host)


# vim: ts=2 sw=2 et:


