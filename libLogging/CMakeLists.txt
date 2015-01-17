set(SUBPROJECT_NAME libLogging)
project(${SUBPROJECT_NAME})
cmake_minimum_required(VERSION 2.6)
set(${SUBPROJECT_NAME}_VERSION_MAJOR 1)
set(${SUBPROJECT_NAME}_VERSION_MINOR 1)



#Add boost
find_package(Boost 1.50.0 COMPONENTS program_options system filesystem REQUIRED)
if(Boost_FOUND)
  message(STATUS "Boost found at ${Boost_INCLUDE_DIRS}")
else(Boost_FOUND)
  message(FATAL_ERROR "Boost not found")
endif()


# Source and header files:
set(SOURCE
	src/libLogging/Logger.cpp
	src/libLogging/LoggerFactory.cpp
	src/libLogging/ConsoleAppender.cpp
	src/libLogging/FileAppender.cpp
)

set(HEADERS
	include/libLogging/Logger.hpp
	include/libLogging/LoggerFactory.hpp
	include/libLogging/Appender.hpp
	include/libLogging/ConsoleAppender.hpp
	include/libLogging/FileAppender.hpp
	include/libLogging/LogLevels.hpp
)

include_directories("include")


include_directories(${Boost_INCLUDE_DIRS})


add_library(${SUBPROJECT_NAME} ${SOURCE} ${HEADERS})
#target_include_directories(${SUBPROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${SUBPROJECT_NAME} ${Boost_LIBRARIES}) #fvsdk ${OpenCV_LIBS} ImageIO

