cmake_minimum_required (VERSION 3.5.6)
project(ck C)


set(CMAKE_C_COMPILER clang)
# gcc flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3")

option(CK_ASAN "Build with asan")

if(CK_ASAN)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
endif(CK_ASAN)
# Set project directories
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(RES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/res)
set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# Set source code locations
set(ckBin_src ${SRC_DIR}/ck.c)
set(ckLib_src
  ${SRC_DIR}/ck.c
  ${SRC_DIR}/actionparser.c
  ${SRC_DIR}/actions.c
  ${SRC_DIR}/confparser.c
  ${SRC_DIR}/dblayer.c
)
set(ckLib_hdr
  ${SRC_DIR}/actionparser.h
  ${SRC_DIR}/actions.h
  ${SRC_DIR}/confparser.h
  ${SRC_DIR}/dblayer.h
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${RES_DIR}/cmake)

find_package(SQLite3)

# Include directories
include_directories(${SRC_DIR})
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

# copy test files
set(BIN_TESTS_DIR ${CMAKE_BINARY_DIR}/res/tests)
set(PROJECT_TESTING_GROUNDS ${CMAKE_BINARY_DIR}/test_files)
configure_file(${TESTS_DIR}/init ${BIN_TESTS_DIR}/init @ONLY)

# check_ck
configure_file(${RES_DIR}/check_ck check_ck @ONLY)
