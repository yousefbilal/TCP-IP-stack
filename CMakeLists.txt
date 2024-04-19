cmake_minimum_required(VERSION 3.0.0)
project(TCP-IP VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

add_executable(TCP-IP main.c tun_if.h tun_if.c utils.h utils.c ether.h ether.c) 

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
