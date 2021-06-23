cmake_minimum_required(VERSION 3.0.0)
project(minisql VERSION 0.1.0)

include(CTest)
enable_testing()

aux_source_directory(./src sourcelist)
add_executable(minisql ${sourcelist})
SET(EXECUTABLE_OUTPUT_PATH ../) 
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
