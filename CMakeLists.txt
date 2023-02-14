project(WhistleDetectionRelease)

cmake_minimum_required(VERSION 3.16)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_subdirectory(whistledetection)
add_subdirectory(soundcapturing)
add_subdirectory(tfliteexecuter)

add_executable(${PROJECT_NAME}
    example.cpp
)

target_link_libraries(${PROJECT_NAME}
    SoundCapturing
)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
