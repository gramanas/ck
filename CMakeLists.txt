cmake_minimum_required (VERSION 3.5.6)
project(ck C)

# Set project directories
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
# Set source code locations
set(ckBin_src ${PROJECT_SOURCE_DIR}/ck.c)
set(ckLib_src
  ${PROJECT_SOURCE_DIR}/ck.c
  ${PROJECT_SOURCE_DIR}/actionparser.c
  ${PROJECT_SOURCE_DIR}/actions.c
  ${PROJECT_SOURCE_DIR}/confparser.c
  ${PROJECT_SOURCE_DIR}/checks.c
  ${PROJECT_SOURCE_DIR}/dblayer.c
)
set(ckLib_hdr
  ${PROJECT_SOURCE_DIR}/actionparser.h
  ${PROJECT_SOURCE_DIR}/actions.h
  ${PROJECT_SOURCE_DIR}/confparser.h
  ${PROJECT_SOURCE_DIR}/checks.h
  ${PROJECT_SOURCE_DIR}/dblayer.h
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

find_package(SQLite3)

# Include directories
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})
include_directories(${CMAKE_BINARY_DIR})
include_directories(${SQLITE3_INCLUDE_DIRS})

# Create the shared library
add_library (ckLib SHARED 
    ${ckLib_src}
    ${ckLib_hdr}
)

# Link
add_executable(ck ${ckBin_src})
target_link_libraries (ck ckLib)
target_link_libraries (ck ${SQLITE3_LIBRARIES})

# copy test file
set(PROJECT_TEST_DIR ${PROJECT_SOURCE_DIR}/tests)
set(BIN_TEST_DIR ${CMAKE_BINARY_DIR}/tests)
set(PROJECT_TESTING_GROUNDS ${CMAKE_BINARY_DIR}/test_files)
configure_file(${PROJECT_TEST_DIR}/init ${BIN_TEST_DIR}/init @ONLY)
configure_file(${PROJECT_TEST_DIR}/check_ck check_ck @ONLY)
