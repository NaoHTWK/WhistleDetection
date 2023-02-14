project(SoundCapturing)

cmake_minimum_required(VERSION 3.16)

add_library(${PROJECT_NAME}
    soundcapturing.cpp
    soundcapturing.h
)

target_link_libraries(${PROJECT_NAME}
    asound
    WhistleDetection
)
target_include_directories(${PROJECT_NAME} PUBLIC .)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
