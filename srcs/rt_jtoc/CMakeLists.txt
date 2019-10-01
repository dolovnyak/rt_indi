cmake_minimum_required(VERSION 3.0)

cmake_policy(SET CMP0076 NEW)

target_sources(rt PUBLIC
        rt_jtoc_get_textures.c
        rt_jtoc_utilits.c
        rt_jtoc_get_camera.c
        rt_jtoc_scene_setup.c
        rt_jtoc_get_effects.c.
        )
