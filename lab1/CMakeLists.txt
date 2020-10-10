cmake_minimum_required(VERSION 3.0.0)
project(lab1 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


include(CTest)
enable_testing()

add_executable(msg_test msg_test.cc message.hh message.cc)

add_executable(lab1 main.cc tcp_socket.hh tcp_socket.cc message.hh message.cc)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
