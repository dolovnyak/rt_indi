cmake_minimum_required(VERSION 3.0)

cmake_policy(SET CMP0076 NEW)

target_sources(cl PUBLIC
        cl_setup.c
        cl_utilits.c
        cl_create_program.c
        cl_init.c
        cl_kernels.c
        )