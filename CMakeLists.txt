cmake_minimum_required(VERSION 3.9)
project(timetable C)

set(CMAKE_C_STANDARD 11)

add_executable(timetable main.c
        parseFiles.c
        timetableInfo.c
        buildTimetable.c
        functions.h)