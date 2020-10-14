cmake_minimum_required(VERSION 3.10)
project(Lab2)

set (CMAKE_CXX_STANDARD 17)
# add_definitions(-std=c++17)
set(CMAKE_C_COMPTLER GCC)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# set(CMAKE_CXX FLAGS -fexec-charset=GBK)

include_directories(${PROJECT_SOURCE_DIR}/include)
find_library(NETSIM_LIB libnetsim.a ${PROJECT_SOURCE_DIR}/lib)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)
add_executable(stop_wait ${SRC_LIST})
target_link_libraries(stop_wait ${NETSIM_LIB})