#!/bin/sh

if [ -z "$1" ]; then
	echo Please enter the absolute path to the INET installation root
	echo -n '> '
	read INET_PATH
else
	INET_PATH=$1
fi

if [ ! -e "$INET_PATH" ]; then
	echo $INET_PATH does not exist
	exit 1
fi
if [ -z "$(head -1 "$INET_PATH/README" | grep INET)" ]; then
	echo $INET_PATH does not seem correct
	exit 1
fi

mkdir -p cmake
cat > cmake/OMNeT++INETConfig.cmake << EOF
set(OMNeT++_INET_BASE_DIR "$INET_PATH")

exec_program(/usr/bin/sed ARGS "-n '/INET-/{s/INET-//;p;q}'" "\${OMNeT++_INET_BASE_DIR}/WHATSNEW" OUTPUT_VARIABLE OMNeT++_INET_VERSION)
if(\${OMNeT++_INET_VERSION} STREQUAL "")
	unset(OMNeT++_INET_VERSION)
endif()

set(OMNeT++_INET_INCLUDE_DIRS
$(find "$INET_PATH/"src/*/ -name '*.h' -exec dirname {} \; | uniq | sed 's/^\(.*\)$/\t"\1"/')
	)

set(OMNeT++_INET_LIBRARY_DIRS "\${OMNeT++_INET_BASE_DIR}/src")

set(OMNeT++_INET_LIBRARIES "inet")
EOF
