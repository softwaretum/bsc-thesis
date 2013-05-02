cmake_minimum_required(VERSION 2.6)
include(CheckCXXCompilerFlag)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
endif()

if(NOT BUILD_EXTRA_WARNINGS)
	set(BUILD_EXTRA_WARNINGS "Warn")
endif()

list(APPEND CMAKE_CXX_FLAGS "-g -Wall -Wextra -pedantic")

set(COLOR_DIAG_FLAG "-fcolor-diagnostics")
check_cxx_compiler_flag (${COLOR_DIAG_FLAG} HAVE_COLOR_DIAG_FLAG)
if(HAVE_COLOR_DIAG_FLAG)
	message ("forcing colored compiler output")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COLOR_DIAG_FLAG}")
endif()

set(WARN_EVERYTHING_FLAG "-Weverything")
# Some warnings that are of absolutely no interest
set(WARN_NOT_FLAGS "-Wno-c++98-compat;-Wno-c++98-compat-pedantic;-Wno-exit-time-destructors")
if(${BUILD_EXTRA_WARNINGS} STREQUAL "Warn-more")
	set(BUILD_EXTRA_WARNINGS "Warn")
else()
	# These warnings are often meaningless
	list(APPEND WARN_NOT_FLAGS "-Wno-padded")
endif()

check_cxx_compiler_flag (${WARN_EVERYTHING_FLAG} HAVE_WARN_EVERYTHING_FLAG)
if ((${BUILD_EXTRA_WARNINGS} STREQUAL "Warn") AND (HAVE_WARN_EVERYTHING_FLAG))
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARN_EVERYTHING_FLAG}")
	foreach(NOT_FLAG ${WARN_NOT_FLAGS})
		check_cxx_compiler_flag (${NOT_FLAG} NOT_HAVE_FLAG)
		if(NOT_HAVE_FLAG)
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${NOT_FLAG}")
		endif()
	endforeach()
endif()

add_subdirectory(src)
