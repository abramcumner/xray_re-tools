message("CMAKE_VERSION " ${CMAKE_VERSION})
cmake_minimum_required(VERSION 3.15)
project(xray_re-tools)

message("CMAKE_CXX_COMPILER_ID             : " ${CMAKE_CXX_COMPILER_ID})
message("CMAKE_CXX_COMPILER_ARCHITECTURE_ID: " ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID})
message("CMAKE_BUILD_TYPE                  : " ${CMAKE_BUILD_TYPE})

add_subdirectory("sources/3rd party")
add_subdirectory(sources/xray_re)
add_subdirectory(sources/utils/converter)