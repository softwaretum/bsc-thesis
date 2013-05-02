set(OMNeT++INET_DIR "${CMAKE_BINARY_DIR}/cmake")
find_package(OMNeT++INET QUIET)
if(NOT OMNeT++INET_FOUND)
	message("Generating OMNeT++ INET CMake config...")
	set(CMAKECONF_COMMAND "${UTIL_DIR}/geninetconf.sh" "${OMNeTpp_INET_BASE_DIR}")
	execute_process(COMMAND ${CMAKECONF_COMMAND}
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		RESULT_VARIABLE CMAKECONF_SUCC)
	if(NOT ${CMAKECONF_SUCC} EQUAL 0)
		message(FATAL_ERROR "Could not setup OMNeT++ INET parameters.")
	endif()
	
	set(OMNeT++INET_DIR "${CMAKE_BINARY_DIR}/cmake")
	find_package(OMNeT++INET REQUIRED)
endif()
