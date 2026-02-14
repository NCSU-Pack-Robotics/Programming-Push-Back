cmake_minimum_required(VERSION 3.22.1)

# Set C and C++ standards
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)   # this makes it "gnu2x" instead of "c23"
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON) # this makes it "gnu++23"

project(Programming_Push_Back C CXX ASM)

############################################################
# Compilation & Linking flags

# Linker flags and scripts
set(LINKER_SCRIPT_HOT ${CMAKE_CURRENT_SOURCE_DIR}/Push_Back/firmware/v5-hot.ld)
set(LINKER_SCRIPT_COLD ${CMAKE_CURRENT_SOURCE_DIR}/Push_Back/firmware/v5.ld)
set(LINKER_SCRIPT_COMMON ${CMAKE_CURRENT_SOURCE_DIR}/Push_Back/firmware/v5-common.ld)

# All the flags to use later
set(MFLAGS
        -mcpu=cortex-a9
        -mfpu=neon-fp16
        -mfloat-abi=hard
        -Os
        -g
        -mthumb
)
set(CPP_FLAGS
        -D_POSIX_THREADS
        -D_UNIX98_THREAD_MUTEX_ATTRIBUTES
        -D_POSIX_TIMERS
        -D_POSIX_MONOTONIC_CLOCK
)
set(GCCFLAGS
        -ffunction-sections
        -fdata-sections
        -fdiagnostics-color
        -funwind-tables
)
set(WARNFLAGS
        -Wno-psabi
        -Wno-deprecated-declarations  # PROS uses lots of deprecated functions internally
)
set(LDFLAGS_COMMON
        ${MFLAGS}
        ${WARNFLAGS}
        ${GCCFLAGS}
        -T${LINKER_SCRIPT_COMMON}
        -Wl,--gc-sections
        -Wl,--no-warn-rwx-segments
        -Wl,--sort-section=alignment
        -Wl,--sort-common
        -nostdlib
)

# Combine flags
add_compile_options(${CPP_FLAGS} ${MFLAGS} ${GCCFLAGS} ${WARNFLAGS})

# Add extra flags from environment or command line (CMake treats undefined variables as empty)
string(APPEND CMAKE_C_FLAGS " $ENV{EXTRA_CFLAGS}")
string(APPEND CMAKE_CXX_FLAGS " $ENV{EXTRA_CXXFLAGS}")

############################################################
# Find necessary files, libraries, and subdirectories

# Files  TODO: This glob method of finding source files is actively discouraged by CMake
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
        Push_Back/src/*.cpp
        Push_Back/src/*.hpp
        Push_Back/src/*.c
        Push_Back/src/*.h)
list(FILTER SRC_FILES EXCLUDE REGEX ".*/test/.*")
file(GLOB TOP_LEVEL_INCLUDE CONFIGURE_DEPENDS
        Push_Back/include/*.h)
file(GLOB_RECURSE PROS_FILES CONFIGURE_DEPENDS
        Push_Back/include/pros/*.cpp
        Push_Back/include/pros/*.hpp
        Push_Back/include/pros/*.c
        Push_Back/include/pros/*.h)
file(GLOB_RECURSE COMMON_FILES CONFIGURE_DEPENDS
        Push_Back/dependencies/common/*.cpp
        Push_Back/dependencies/common/*.hpp
        Push_Back/dependencies/common/*.c
        Push_Back/dependencies/common/*.h)
file(GLOB FIRMWARE_LIBS
        ${CMAKE_CURRENT_SOURCE_DIR}/Push_Back/firmware/*.a
)
file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS
        *\.cc)

# Exclude build and external directories from source/header globs
list(FILTER SRC_FILES EXCLUDE REGEX ".*/cmake-build-.*|.*/_deps/.*|.*/CMakeFiles/.*")

# Directories to include in compilation
include_directories(
        Push_Back/src
        Push_Back/include
        Push_Back/include/pros
)

##############################################################
# Definitions

if (DO)
    add_definitions(-DDO)
endif ()
if (THINK)
    add_definitions(-DTHINK)
endif ()
if (BRAIN)
    add_definitions(-DBRAIN)
endif ()
if (PI)
    add_definitions(-DPI)
endif ()

# Check for liblvgl llemu files and define macros if they exist
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Push_Back/include/liblvgl/llemu.h")
    add_definitions(-D_PROS_INCLUDE_LIBLVGL_LLEMU_H)
endif ()
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Push_Back/include/liblvgl/llemu.hpp")
    add_definitions(-D_PROS_INCLUDE_LIBLVGL_LLEMU_HPP)
endif ()

##############################################################
# Build dependency libraries

list(APPEND BUILT_LIBS)

# Common library—shared between multiple projects
add_library(common STATIC
        ${COMMON_FILES}
)
target_include_directories(common PUBLIC
        Push_Back/dependencies/common/src
)
add_subdirectory(Push_Back/dependencies/common)
list(APPEND BUILT_LIBS common)

# Group all archives and standard libs to resolve circular deps like in Makefile
# Put start/end group directly in link libraries so they surround archives
set(GROUPED_LIBS
        -Wl,--start-group
        ${FIRMWARE_LIBS}
        ${BUILT_LIBS}
        gcc
        stdc++
        c
        m
        nosys
        -Wl,--end-group
)

##############################################################s
# Create timestamp

# File paths
set(TIMESTAMP_SRC ${CMAKE_BINARY_DIR}/_pros_ld_timestamp.c)
set(TIMESTAMP_TEMPLATE ${TIMESTAMP_SRC}.in)

# Create template file
file(WRITE ${TIMESTAMP_TEMPLATE}
"const int _PROS_COMPILE_TIMESTAMP_INT = @TIMESTAMP_INT@;
const char * const _PROS_COMPILE_TIMESTAMP = \"@TIMESTAMP_STR@\";
const char * const _PROS_COMPILE_DIRECTORY = \"@COMPILE_DIR@\";"
)

# Get Unix timestamp
execute_process(
        COMMAND date +%s
        OUTPUT_VARIABLE UNIX_TIME
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get timezone offset in hours (e.g., -0500 → -5)
execute_process(
        COMMAND date +%-z
        OUTPUT_VARIABLE TZ_OFFSET
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Adjust timestamp by timezone offset and convert to int
math(EXPR TIMESTAMP_INT "${UNIX_TIME} + (${TZ_OFFSET} / 100 * 3600)")

# Create formatted timestamp like __DATE__ " " __TIME__
string(TIMESTAMP TIMESTAMP_STR "%b %d %Y %H:%M:%S")

# Get last 23 characters of working directory
get_filename_component(PWD "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)
string(LENGTH "${PWD}" PWD_LEN)

# Truncate to last 23 characters if too long
if (PWD_LEN GREATER 23)
    math(EXPR START "${PWD_LEN} - 23")
    string(SUBSTRING "${PWD}" ${START} 23 COMPILE_DIR)
else ()
    set(COMPILE_DIR "${PWD}")
endif ()

# Generate the timestamp source file from the template
configure_file(
        ${TIMESTAMP_SRC}.in
        ${TIMESTAMP_SRC}
        @ONLY
)

##############################################################s
# Find project.pros file for moving bin files to expected location.

# Find the directory containing pros.project file
file(GLOB_RECURSE PROS_PROJECT_FILE "${CMAKE_SOURCE_DIR}/*/project.pros")
if (PROS_PROJECT_FILE)
    list(GET PROS_PROJECT_FILE 0 PROS_PROJECT_PATH)
    get_filename_component(PROS_PROJECT_DIR "${PROS_PROJECT_PATH}" DIRECTORY)
    message(STATUS "Found pros.project at: ${PROS_PROJECT_DIR}")
    file(MAKE_DIRECTORY ${PROS_PROJECT_DIR}/bin)
else ()
    message(WARNING "Could not find project.pros file")
    set(PROS_PROJECT_DIR "${CMAKE_SOURCE_DIR}")
endif ()

##############################################################s
# Build Cold Package

# Libraries to link
set(COLD_FIRMWARE_LIBS ${FIRMWARE_LIBS})
# Cannot include newlib and libc because not all of the req'd stubs are implemented
list(FILTER COLD_FIRMWARE_LIBS EXCLUDE REGEX "libc\.a$")
list(FILTER COLD_FIRMWARE_LIBS EXCLUDE REGEX "libm\.a$")

set(EMPTY_SOURCE ${CMAKE_BINARY_DIR}/_cold_dummy.c)  # Need a dummy file for to make an executable
file(WRITE ${EMPTY_SOURCE} "")  # Empty bc libpros.a provides main()

add_executable(cold.package.elf ${EMPTY_SOURCE})

target_link_libraries(cold.package.elf PRIVATE
        -Wl,--whole-archive
        ${COLD_FIRMWARE_LIBS}
        -lstdc++
        -Wl,--no-whole-archive
        -Wl,--start-group
        -nostdlib
        gcc
        c
        m
        nosys
        -Wl,--end-group
)

target_link_options(cold.package.elf PRIVATE
        -T${LINKER_SCRIPT_COLD}
        -Wl,--gc-keep-exported
        ${LDFLAGS_COMMON}  # Needs to be last for --gc-keep-exported to work correctly
)

# Create binary
add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/cold.package.bin ${PROS_PROJECT_DIR}/bin/cold.package.bin
        COMMAND ${CMAKE_OBJCOPY}
            --strip-symbol=install_hot_table
            --strip-symbol=__libc_init_array
            --strip-symbol=_PROS_COMPILE_DIRECTORY
            --strip-symbol=_PROS_COMPILE_TIMESTAMP
            --strip-symbol=_PROS_COMPILE_TIMESTAMP_INT
            $<TARGET_FILE:cold.package.elf>
            $<TARGET_FILE:cold.package.elf>
        COMMAND ${CMAKE_OBJCOPY}
            -O binary
            -R .hot_init
            $<TARGET_FILE:cold.package.elf>
            ${CMAKE_BINARY_DIR}/cold.package.bin
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/cold.package.bin
            ${PROS_PROJECT_DIR}/bin/cold.package.bin
        DEPENDS cold.package.elf
        COMMENT "Creating cold.package.bin"
)

##############################################################s
# Build Hot Package

# Hot Package
add_executable(hot.package.elf
        ${SRC_FILES}
        ${TOP_LEVEL_INCLUDE}
        ${TIMESTAMP_SRC}
)

add_dependencies(hot.package.elf cold.package.elf)

target_link_libraries(hot.package.elf PRIVATE
        ${GROUPED_LIBS}
)

target_link_options(hot.package.elf PRIVATE
        -T${LINKER_SCRIPT_HOT}
        ${LDFLAGS_COMMON}
        -nostartfiles
        -Wl,-R,${CMAKE_BINARY_DIR}/cold.package.elf
)

# Create binary
add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/hot.package.bin ${PROS_PROJECT_DIR}/bin/hot.package.bin
        COMMAND ${CMAKE_OBJCOPY}
            ${CMAKE_BINARY_DIR}/hot.package.elf
            -O binary ${CMAKE_BINARY_DIR}/hot.package.bin
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/hot.package.bin
            ${PROS_PROJECT_DIR}/bin/hot.package.bin
        DEPENDS hot.package.elf
        COMMENT "Creating hot.package.bin"
)

##############################################################
# Main Targets

set(FINAL_OUTPUTS
        ${CMAKE_BINARY_DIR}/cold.package.bin
        ${PROS_PROJECT_DIR}/bin/cold.package.bin
        ${CMAKE_BINARY_DIR}/hot.package.bin
        ${PROS_PROJECT_DIR}/bin/hot.package.bin
)

# Hot and Cold package target
add_custom_target(Build ALL
        DEPENDS ${FINAL_OUTPUTS}
)

add_custom_target(Build_and_Upload
        COMMAND cd ${PROS_PROJECT_DIR} && pros upload
        DEPENDS Build
        COMMENT "Uploading to V5 Brain"
)
