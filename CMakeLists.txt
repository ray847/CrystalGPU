cmake_minimum_required(VERSION 3.15)

# Project Options
project(CrystalGPU CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# External Libraries
add_subdirectory(lib/WebGPU-distribution)
add_subdirectory(lib/CrystalBase)

# Internal Library
add_library(CrystalGPU)
set_target_properties(CrystalGPU PROPERTIES COMPILE_WARNING_AS_ERROR ON)
target_sources(CrystalGPU
  PRIVATE
    src/gpu.cpp
    src/impl/webgpu.cpp
)
target_include_directories(CrystalGPU
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(CrystalGPU
  PUBLIC
    webgpu
    CrystalBase
)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  message(STATUS "Building CrystalGPU Standalone (with tests/main)")
  # Main
  add_executable(main main.cpp)
  target_include_directories(main
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
  target_link_libraries(main
  PRIVATE
    CrystalGPU
  )
  target_copy_webgpu_binaries(main)

  # Testing
  add_subdirectory(test)
endif()
