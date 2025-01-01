cmake_minimum_required(VERSION 3.10)
project(TCP-IP VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

add_executable(${PROJECT_NAME}
        src/main.c
        src/tun_if.c
        src/utils.c
        src/ether.c
        src/arp.c
        include/ip.h
        src/ip.c
        include/icmp.h
        src/icmp.c
)
include_directories(./include)

add_subdirectory(Google_tests)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
