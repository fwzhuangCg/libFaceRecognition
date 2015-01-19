cmake_minimum_required (VERSION 2.6)

set(PROJECT_NAME libFaceRecognition)
project(${PROJECT_NAME})

# The version number.
set (${PROJECT_NAME}_VERSION_MAJOR 1)
set (${PROJECT_NAME}_VERSION_MINOR 0)


include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-std=c++11 HAS_CXX11_FLAG)
if (HAS_CXX11_FLAG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

#Windows
add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})

include_directories(${PROJECT_SOURCE_DIR})


add_subdirectory(libEnroll)
add_subdirectory(libMatching)
add_subdirectory(libLogging)


