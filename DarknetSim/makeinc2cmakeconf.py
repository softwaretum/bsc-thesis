#!/usr/bin/env python3

from io import open
import re
import shlex
from subprocess import Popen, PIPE
from sys import argv, exit
import os

def usage():
    print(
        "%s [path to Makefile.inc]" % argv[0],
        "\tThis tool attempts to (partially, but sufficiently) convert OMNeT++'s Makefile.inc to a CMake config-file.",
        sep = "\n")
    exit(0)

# First find OMNeT++'s Makefile.inc.
makefile_path = None
if len(argv) > 1:
    if "-h" in argv:
        usage()
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
    makefile_path = input("Please provide the path to OMNeT++'s Makefile.inc: ")

print("Using \"%s\"." % makefile_path)

# Now create out CMake config file and start writing some generic stuff.
cmakefile_path = "OMNeT++Config.cmake"

if os.path.exists(cmakefile_path):
    while True:
        answer = input("Warning: %s already exists. Proceed? [y/N] " % cmakefile_path).lower()
        if "no".startswith(answer):
            exit(0)
        elif "yes".startswith(answer):
            break

print("Writing to %s" % cmakefile_path)

cmakefile = open(cmakefile_path, 'w')
cmakefile.write("# This file was auto-generated via %s\n" % argv[0])
cmakefile.write("# This CMake config might not expose the complete OMNeT++ build environment\n\n")

print(
    "if(NOT OMNeT++_BUILD_TYPE)",
    "\tset(OMNeT++_BUILD_TYPE \"release\")",
    "endif()",
    "",
    "set(OMNeT++_DEFINITIONS)",
    "set(OMNeT++_INCLUDE_DIRS)",
    "set(OMNeT++_LIBRARY_DIRS)",
    "set(OMNeT++_LIBRARIES)",
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

# Splits command-line parameters according to filters,
# a { "-switch": list }-dict. All switch-values will be appended to list.
def distribute_parameters(filters, args):
    # put_into is for key, value in two parameters ("-DDEF" vs "-D DEF")
    put_into = None
    for parameter in shlex.split(args):
        if put_into:
            put_into.append(parameter)
            put_into = None
            continue
        for switch, into in filters.items():
            if parameter.startswith(switch):
                rest = parameter[len(switch):]
                if rest == "":
                    put_into = into
                else:
                    into.append(rest)
                continue

# Only consideres -D, -I and -isystem flags, further configuration should be done at project-scope
def convert_cflags(flags):
    defs = []
    includes = []
    filters = {
        "-D": defs,
        # Unlikely, but if there are include-directories defined this way, they'll get added
        "-I": includes,
        # Should we differentiate for CMake?
        "-isystem": includes,
        # There are many more potentially relevant, I've never seen them in the wild before though.
    }
    distribute_parameters(filters, flags)
    return dict(filter(lambda item: item[1] != [], { "DEFINITIONS": map(lambda str: "-D" + str, defs), "INCLUDE_DIRS": includes }.items()))

def convert_ldflags(flags):
    lib_dirs = []
    libs = []
    filters = {
        "-L": lib_dirs,
        "-l": libs,
    }
    distribute_parameters(filters, flags)
    return dict(filter(lambda item: item[1] != [], { "LIB_DIRS": lib_dirs, "LIBRARIES": libs }.items()))

# Converts CFLAGS_$MODE = FLAGS to (roughly) if(BUILD_TYPE == $MODE) OMNeT++_DEFINITIONS = FLAGS
def per_config_cflags(match_key, match_value):
    global newline
    global cmakefile_indentation
    # match_key = "CFLAGS_*", strip CFLAGS_
    configuration = match_key[7:]
    flags = convert_cflags(match_value)
    
    begin_write()
    cmakefile.write("if(${OMNeT++_BUILD_TYPE} STREQUAL \"%s\")\n" % configuration)
    cmakefile_indentation += 1
    for name, values in flags.items():
        cmake_add(name, " ".join(values))
    cmakefile_indentation -= 1
    cmakefile.write("endif()\n")
    newline = True

# These rules match Makefile operations and group the parameters.
regex_assign = re.compile(r"(\w+)\s*=\s*([^#\n]*)",   re.ASCII)
regex_append = re.compile(r"(\w+)\s*\+=\s*([^#\n]*)", re.ASCII)
regex_varref = re.compile(r"\$\((\w+)\)",             re.ASCII)
regex_empty  = re.compile(r"^\s*(?:#.*)?$",           re.ASCII)

linenumber = None

# make evaluates the variables lazily, cmake eagerly.
# This is why right now I don't care about variable references in values.
def transform_varrefs(arg):
    global linenumber
    # re.sub(regex_varref, "${\1}", arg)
    if regex_varref.search(arg):
        print("Sorry, I can't deal with variable references (yet?)\n\tfile %s, line %d: %s" % (makefile_path, linenumber, arg))
    return arg

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
    if regex_empty.match(line):
        newline = True
        continue
    
    match = regex_assign.search(line)
    if match:
        key, value = match.groups()
        # value = transform_varrefs(value)
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
        # Properly done this would parse the HAVE_*-variables and look for its CMake modules.
        # meh
        elif key.startswith("CFLAGS_"):
            newline = True
            per_config_cflags(key, value)
        elif key == "CPPFLAGS":
            begin_write()
            for name, values in convert_cflags(value).items():
                cmake_add(name, " ".join(values))
        elif key == "LDFLAGS":
            begin_write()
            for name, values in convert_ldflags(value).items():
                cmake_add(name, " ".join(values))
            # These needs a bit more hackaroo
        elif key.endswith("ENV_LIBS") or key == "KERNEL_LIBS":
            begin_write()
            libs = filter(lambda flag: flag.startswith("-l"), shlex.split(value))
            # remove -l and $D from the flags
            libs = list(map(lambda libflag: libflag[2:-2], libs))
            if libs == []:
                continue
            cmake_add("LIBRARIES", " ".join(libs))
        continue
    
    match = regex_append.search(line)
    if match:
        key, value = match.groups()
        # value = transform_varrefs(value)
        if key in translate:
            begin_write()
            cmake_add(translate[key], value)
        elif key == "OMNETPP_VERSION":
            print("Sorry, I can't deal with adding to OMNETPP_VERSION (yet?)\n\tfile %s, line %d: %s\n\tIgnoring..." % (makefile_path, linenumber, line.strip()))
        elif key.startswith("CFLAGS_"):
            newline = True
            per_config_cflags(key, value)
        elif key == "CPPFLAGS":
            begin_write()
            for name, values in convert_cflags(value).items():
                cmake_add(name, " ".join(values))
        elif key == "LDFLAGS":
            begin_write()
            for name, values in convert_ldflags(value).items():
                cmake_add(name, " ".join(values))
        continue
