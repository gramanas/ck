cmake_minimum_required (VERSION 3.5.6)
project(ck C)

# Set project directories
set(PROJECT_SOURCE_DIR ./src)
# Set source code locations
set(ckBin_src ${PROJECT_SOURCE_DIR}/main.c)
set(ckLib_src
  ${PROJECT_SOURCE_DIR}/main.c
)
set(ckLib_hdr
#  ${PROJECT_SOURCE_DIR}/tui.hpp
)

# Include directories
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})
include_directories(${CMAKE_BINARY_DIR})

# Create the shared library
add_library (ckLib SHARED 
    ${ckLib_src}
    ${ckLib_hdr}
)

# Link
add_executable(ck ${ckBin_src})
target_link_libraries (ck ckLib)
