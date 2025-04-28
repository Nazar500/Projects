cmake_minimum_required(VERSION 3.22.1)

project("cpptest")

set(CMAKE_CXX_STANDARD 17)

# Ensure the NDK path is correct
set(CMAKE_ANDROID_NDK "${CMAKE_ANDROID_NDK}")

# Include directories for native_app_glue and OpenGL ES
include_directories(${CMAKE_ANDROID_NDK}/sources/android/native_app_glue)
#include_directories(${CMAKE_ANDROID_NDK}/platforms/android-31/arch-arm/usr/include)

# Create and name a shared library
add_library(${CMAKE_PROJECT_NAME} SHARED
        main.cpp
        ${CMAKE_ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)

# Find the log, android, EGL, and GLESv1_CM libraries
find_library(log-lib log)
find_library(android-lib android)

# Link libraries to the target library
target_link_libraries(${CMAKE_PROJECT_NAME}
        ${log-lib}
        ${android-lib}
        EGL
        GLESv2
)

# Additional CMake settings for Android
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DANDROID")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
