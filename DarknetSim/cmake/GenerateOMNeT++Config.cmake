set(OMNeT++_DIR "${CMAKE_BINARY_DIR}/cmake")
find_package(OMNeT++ 4 QUIET)
if(NOT OMNeT++_FOUND)
	message("Generating OMNeT++ CMake config...")
	set(CMAKECONF_COMMAND "${UTIL_DIR}/makeinc2cmakeconf.py" "${OMNeTpp_BASE_DIR}")
	execute_process(COMMAND ${CMAKECONF_COMMAND}
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		RESULT_VARIABLE CMAKECONF_SUCC)
	if(NOT ${CMAKECONF_SUCC} EQUAL 0)
		message(FATAL_ERROR "Could not setup OMNeT++ parameters.")
	endif()
	
	set(OMNeT++_DIR "${CMAKE_BINARY_DIR}/cmake")
	find_package(OMNeT++ 4 REQUIRED)
endif()
