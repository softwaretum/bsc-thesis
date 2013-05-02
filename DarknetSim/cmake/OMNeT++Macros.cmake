include(CMakeParseArguments)

# convert_message_files(outfiles header_dirs infiles...)
# Add outfiles to your target and add header_dirs to the include directories
function(CONVERT_MESSAGE_FILES source_files include_dirs)
	set(OPTIONS)
	set(ONE_VALUE_ARGS SOURCE_SUFFIX HEADER_SUFFIX)
	set(MULTIPLE_VALUE_ARGS)
	
	cmake_parse_arguments(MSGC_ARG "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTIPLE_VALUE_ARGS}" ${ARGN})
	
	set(MSGC_ARGS "-h" "-A")
	if(NOT DEFINED MSGC_ARG_SOURCE_SUFFIX)
		set(MSGC_ARG_SOURCE_SUFFIX "_m.cc")
	endif()
	list(APPEND MSGC_ARGS "-s" "${MSGC_ARG_SOURCE_SUFFIX}")
	
	if(NOT DEFINED MSGC_ARG_HEADER_SUFFIX)
		set(MSGC_ARG_HEADER_SUFFIX "_m.h")
	endif()
	list(APPEND MSGC_ARGS "-t" "${MSGC_ARG_HEADER_SUFFIX}")
	
	set(MSG_DIR "${CMAKE_BINARY_DIR}/messages")
	execute_process(COMMAND "${CMAKE_COMMAND}" -E make_directory "${MSG_DIR}")
	
	foreach(MSG_FILE "${MSGC_ARG_UNPARSED_ARGUMENTS}")
		get_filename_component(IN_FILE "${MSG_FILE}" ABSOLUTE)
		get_filename_component(FILE_PREFIX "${MSG_FILE}" NAME_WE)
		set(OUT_PREFIX "${MSG_DIR}/${FILE_PREFIX}")
		
		# It is important that nothing depends on the source-file,
		# else CMake will not create a rule to create the object-file for it
		set(OUT_HEADER_FILE "${OUT_PREFIX}${MSGC_ARG_HEADER_SUFFIX}")
		add_custom_command(OUTPUT "${OUT_HEADER_FILE}"
			COMMAND "${OMNeT++_MSGC}" ARGS ${MSGC_ARGS} "${IN_FILE}"
			MAIN_DEPENDENCY "${IN_FILE}"
			WORKING_DIRECTORY "${MSG_DIR}"
			VERBATIM)
		
		set(OUT_SOURCE_FILE "${OUT_PREFIX}${MSGC_ARG_SOURCE_SUFFIX}")
		add_custom_command(OUTPUT "${OUT_SOURCE_FILE}"
			COMMAND "${CMAKE_COMMAND}" -E touch_nocreate "${OUT_SOURCE_FILE}"
			MAIN_DEPENDENCY "${OUT_HEADER_FILE}"
			WORKING_DIRECTORY "${MSG_DIR}"
			VERBATIM)
		list(APPEND OUT_SOURCE_FILES "${OUT_SOURCE_FILE}")
		
		if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${FILE_PREFIX}_m.h"
		OR EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${FILE_PREFIX}${MSGC_ARG_HEADER_SUFFIX}")
			message(WARNING "You should consider removing ${FILE_PREFIX}_m.{cc,h} from the source-tree: A source could include a potentially old header.\nThe default OMNeT++-created Makefile generates message-sources in place, this CMake-created Makefile generates files in the build-tree and leaves the source-tree untouched.")
		endif()
	endforeach()
	
	# meh
	string(REPLACE ";" " " OUT_SOURCE_FILES ${OUT_SOURCE_FILES})
	set(${source_files} "${OUT_SOURCE_FILES}" PARENT_SCOPE)
	set(${include_dirs} "${MSG_DIR}" PARENT_SCOPE)
endfunction()
