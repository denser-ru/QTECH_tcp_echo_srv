cmake_minimum_required(VERSION 3.21)
project(tcp_echo_srv)

set(CMAKE_C_FLAGS "-Wall -Wextra -Werror")
include_directories(inc)

add_executable(tcp_echo_srv
        src/main.c
        src/server.c
        src/request.c
        src/respond.c
        src/signal_catch.c
        src/socket_info.c
        src/itoa.c
        src/clients.c
        )
