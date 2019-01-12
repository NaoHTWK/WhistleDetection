project(WhistleDetection)

add_library(${PROJECT_NAME}
    soundcapturing.cpp
    soundcapturing.h
)

add_subdirectory(whistle)

target_link_libraries(${PROJECT_NAME}
    Whistle
    asound
)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        .
)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 14)
