cmake_minimum_required(VERSION 3.0.0)
project(TCP-IP VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

add_executable(${PROJECT_NAME} main.c tun_if.c utils.c ether.c
        arp.h
        arp.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
