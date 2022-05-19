cmake_minimum_required(VERSION 3.20)
project(softmax)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out)

include_directories(half_float)

set(SOURCE_FILES half_float/half_float.h
        half_float/umHalf.h
        half_float/umHalf.inl
        half_float/test.cpp
        module/exponent.h
        module/exponent.cpp half_float/half_float.cpp module/logarithm.h module/logarithm.cpp module/type.h module/util.h)

set(APP_FILES main.cpp softmax.h)

add_library(softmax_lib STATIC ${SOURCE_FILES})

add_executable(test ${APP_FILES})
target_link_libraries(test softmax_lib)
