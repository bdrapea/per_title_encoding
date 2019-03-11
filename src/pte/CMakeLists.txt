cmake_minimum_required(VERSION 2.8)

#name of the project
project(pte)

#File indexing
set(SRCS "main.cpp" "pte_video_profile.cpp")
set(HDRS "pte_main_functions.h" "pte_video_profile.h")

add_executable(${PROJECT_NAME} ${SRCS} ${HDRS})

