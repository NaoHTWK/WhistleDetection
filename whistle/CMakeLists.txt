project(Whistle)

add_library(${PROJECT_NAME}
    algorithm_ext.h
    fftw_allocator.h
    ring_buffer.h
    whistledetection.cpp
    whistledetection.h
)

target_link_libraries(${PROJECT_NAME}
    fftw3f
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    .
)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 14)
