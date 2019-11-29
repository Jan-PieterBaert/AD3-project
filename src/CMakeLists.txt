cmake_minimum_required(VERSION 3.15)
project(history C)

set(CMAKE_C_STANDARD 11)

add_executable(history
        main.c
        utils/commandParser.c
        utils/btree.c
        utils/misc.c
        )

add_definitions(
    -DCACHESIZE=8192
    -DCACHEASSOC=8
    -DCACHELINESIZE=64
    -O3
    -g
    )
