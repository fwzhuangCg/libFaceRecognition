cmake_minimum_required (VERSION 2.6)

set(SUBPROJECT_NAME Utilities)
project(${SUBPROJECT_NAME})

# The version number.
set (${SUBPROJECT_NAME}_VERSION_MAJOR 1)
set (${SUBPROJECT_NAME}_VERSION_MINOR 0)


include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-std=c++11 HAS_CXX11_FLAG)
if (HAS_CXX11_FLAG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

if(WITH_FV_SDK)

# Add the FaceVACS SDK as a dependency:
message(STATUS "FVSDK library at: ${FVSDK_LIB_DIR}")
message(STATUS "FVSDK debug library at (empty for linux): ${FVSDK_DBG_LIB_DIR}")
if(WIN32) # I think this doesn't include Cygwin/MingW, but we would want it too
	set(FVSDKLIB ${FVSDK_LIB_DIR})
	set(FVSDKLIB_DBG ${FVSDK_DBG_LIB_DIR})
	add_library(fvsdk SHARED IMPORTED)
	set_property(TARGET fvsdk PROPERTY IMPORTED_LOCATION ${FVSDKLIB}.dll)
	set_property(TARGET fvsdk PROPERTY IMPORTED_IMPLIB ${FVSDKLIB}.lib)
	set_property(TARGET fvsdk PROPERTY IMPORTED_LOCATION_DEBUG ${FVSDKLIB_DBG}.dll)
	set_property(TARGET fvsdk PROPERTY IMPORTED_IMPLIB_DEBUG ${FVSDKLIB_DBG}.lib)
else(WIN32)
	set(FVSDKLIB ${FVSDK_LIB_DIR})
	add_library(fvsdk SHARED IMPORTED)
	set_property(TARGET fvsdk PROPERTY IMPORTED_LOCATION ${FVSDKLIB}/libfrsdk-8.9.5.so)

endif()
message(STATUS "Including FVSDK at ${FVSDK_INCLUDE_DIR}")
include_directories(${FVSDK_INCLUDE_DIR})

endif()

# find dependencies:

#openCV
find_package(OpenCV 2.4.3 REQUIRED core highgui video)

find_package(Boost 1.50.0 COMPONENTS program_options system filesystem REQUIRED)
if(Boost_FOUND)
  message(STATUS "Boost found at ${Boost_INCLUDE_DIRS}")
else(Boost_FOUND)
  message(FATAL_ERROR "Boost not found")
endif()


# Source and header files:
set(SOURCE
	src/libImageConversion.cpp

)

set(HEADERS
	include/libImageConversion.hpp
)


include_directories("include")
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

add_library(${SUBPROJECT_NAME} ${SOURCE} ${HEADERS})

target_link_libraries(${SUBPROJECT_NAME} ${Boost_LIBRARIES} ${OpenCV_LIBS} fvsdk )


