project(WhistleDetection)

cmake_minimum_required(VERSION 3.16)

add_library(${PROJECT_NAME}
    fftw_allocator.h
    tflitewhistledetection.h
    tflitewhistledetection.cpp
)

target_link_libraries(${PROJECT_NAME}
    tfliteexecuter
    fftw3f
)

target_include_directories(${PROJECT_NAME}
    PUBLIC .
)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
