#!/usr/bin/env python3

from io import open
import re
from subprocess import Popen, PIPE
from sys import argv, exit
import os

# First find OMNeT++'s Makefile.inc.
makefile_path = None
if len(argv) > 1:
    makefile_path = argv[1]
else:
    print("Attempting to locate Makefile.inc.")
    
    try:
        opp_configfilepath = Popen(["opp_configfilepath"], stdout=PIPE)
    except OSError as err:
        print("Unable to spawn \"opp_configfilepath\":", err)
        
        # I don't know where to look on non-FHS systems.
        search_paths = {
            'posix': ["/usr/share/omnetpp", "/usr/local/share/omnetpp", "/opt/omnetpp"],
        }
        for path in search_paths.get(os.name, []):
            print("Probing %s ..." % path)
            path = os.path.join(path, "Makefile.inc")
            if os.path.isfile(path):
                print("%s it is!" % path);
                makefile_path = path
                break
    else:
        makefile_path = opp_configfilepath.stdout.read().decode().strip()

if makefile_path is None:
    print("Could not locate Makefile.inc.")
    # TODO: ask user to supply path via stdin
    exit(1)

print("Using \"%s\"." % makefile_path)

# Now create out CMake config file and start writing some generic stuff.
cmakefile = open("OMNeT++Config.cmake", 'w')
cmakefile.write("# This file was auto-generated via %s\n" % argv[0])
cmakefile.write("# This CMake config might not expose the complete OMNeT++ environment\n\n")

print(
    "if(NOT OMNeT++_BUILD_TYPE)",
    "\tset(OMNeT++_BUILD_TYPE \"release\")",
    "endif()",
    sep = "\n", file = cmakefile)

# This controls indentation-level for cmake_set and cmake_add.
cmakefile_indentation = 0

# Assign value to key
def cmake_set(key, value):
    cmakefile.write("%sset(OMNeT++_%s \"%s\")\n" % ("\t" * cmakefile_indentation, key, value))

# Append value to key
def cmake_add(key, value):
    cmakefile.write("%sset(OMNeT++_%s \"${OMNeT++_%s} %s\")\n" % ("\t" * cmakefile_indentation, key, key, value))

# We will try to cluster variables (without empty lines) like Makefile.inc.
# Since we don't convert everything, we can't just copy every new line.
# We also use this to determine printing our own empty lines,
# so since we've already written something, start with True.
newline = True
def begin_write():
    global newline
    if newline:
        cmakefile.write("\n")
    newline = False

# Converts CFLAGS_$MODE to (roughly) if(BUILD_TYPE == $MODE) OMNeT++_DEFINITIONS = CFLAGS
# Only consideres -D flags, further configuration should be done at project-scope
# TODO: parse -I, -isystem, -L and -l
def convert_cflags(match_key, match_value, cmake_op):
    global newline
    global cmakefile_indentation
    # Properly done this would parse the HAVE_*-variables and look for its CMake modules.
    # meh
    configuration = key[7:].lower()
    defs = filter(lambda flag: flag.startswith("-D"), value.split())
    begin_write()
    cmakefile.write("if(${OMNeT++_BUILD_TYPE} STREQUAL \"%s\")\n" % configuration)
    cmakefile_indentation += 1
    cmake_op("DEFINITIONS", " ".join(defs))
    cmakefile_indentation -= 1
    cmakefile.write("endif()\n")
    newline = True

# These rules match Makefile operations and group the parameters.
regex_assign = re.compile(r"(\w+)\s*=\s*([^#\n]*)",   re.ASCII)
regex_append = re.compile(r"(\w+)\s*\+=\s*([^#\n]*)", re.ASCII)
regex_varref = re.compile(r"\$\((\w+)\)",             re.ASCII)

# Makefile operations with these variables will simply get translated into CMake syntax.
# Key is Makefile-variable, value is CMake name.
# OMNeT++_ is automatically prepended.
translate = {
    "OMNETPP_BIN_DIR": "PROGRAM_PATH",
    "OMNETPP_INCL_DIR": "INCLUDE_DIRS",
    "OMNETPP_LIB_DIR": "LIBRARY_DIRS",
    "OMNETPP_IMAGE_PATH": "IMAGE_PATH",
    # find_program instead?
    "MSGC": "MSGC",
}

# Now we start converting the Makefile.inc
for linenumber, line in enumerate(open(makefile_path, 'r')):
    if line == "\n" or line[0] == "#":
        newline = True
        continue
    
    match = regex_assign.search(line)
    if match:
        key, value = match.groups()
        if key in translate:
            begin_write()
            cmake_set(translate[key], value)
        elif key == "OMNETPP_VERSION":
            begin_write()
            version_parts = ["MAJOR", "MINOR", "PATCH"]
            for index, version in enumerate(value.split(".")[:len(version_parts)]):
                cmake_set("VERSION_" + version_parts[index], version)
            cmake_set("VERSION_STRING", value)
            newline = True
        elif key.startswith("CFLAGS_"):
            convert_cflags(key, value, cmake_set)
    
    match = regex_append.search(line)
    if match:
        key, value = match.groups()
        if key in translate:
            begin_write()
            cmake_add(translate[key], value)
        elif key == "OMNETPP_VERSION":
            print("Sorry, I can't deal with adding to OMNETPP_VERSION (yet?)\n\tfile %s, line %d: %s\n\tIgnoring..." % (makefile_path, linenumber, line.strip()))
        elif key.startswith("CFLAGS_"):
            convert_cflags(key, value, cmake_add)
    
    # TODO: deal with varrefs
    # TODO: and _LIBS
