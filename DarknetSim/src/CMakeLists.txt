project(darknetsim)

set(UTIL_DIR "${CMAKE_HOME_DIRECTORY}/cmake/util")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Find OMNeT++ config or generate
find_package(OMNeT++ 4 QUIET)
if(NOT OMNeT++_FOUND)
	include(GenerateOMNeT++Config)
endif()

find_package(OMNeT++INET QUIET)
if(NOT OMNeT++INET_FOUND)
	include(GenerateOMNeT++INETConfig)
endif()
set(INET_DESIRED_VERSION "20111118")
if(NOT DEFINED OMNeT++_INET_VERSION)
	message(WARNING "The version of INET could not be determined.")
elseif(NOT ${OMNeT++_INET_VERSION} STREQUAL ${INET_DESIRED_VERSION})
	message(WARNING "The recommended version of INET is ${INET_DESIRED_VERSION}.\nYours is ${OMNeT++_INET_VERSION} and might not build with this project.")
endif()

list(APPEND OMNeT++_DEFINITIONS "${OMNeT++INET_DEFINITIONS}")
list(APPEND OMNeT++_INCLUDE_DIRS "${OMNeT++_INET_INCLUDE_DIRS}")
list(APPEND OMNeT++_LIBRARIES "${OMNeT++_INET_LIBRARIES}")
list(APPEND OMNeT++_LIBRARY_DIRS "${OMNeT++_INET_LIBRARY_DIRS}")

#find_package (Boost REQUIRED)

add_definitions(${OMNeT++_DEFINITIONS})
include_directories(SYSTEM ${OMNeT++_INCLUDE_DIRS})
link_directories(${OMNeT++_LIBRARY_DIRS})

# This should be in OMNeT++, but eh
include(OMNeT++Macros)

convert_message_files(MESSAGE_FILES MESSAGE_INCLUDE_DIRS
	"darknetmessage.msg"
	)
include_directories(${MESSAGE_INCLUDE_DIRS})

add_executable(darknetsim
	${MESSAGE_FILES}
	"DarknetBaseNode.cc"
	"HotpotatoNode.cc"
	)
target_link_libraries(darknetsim oppmain ${OMNeT++_LIBRARIES})
