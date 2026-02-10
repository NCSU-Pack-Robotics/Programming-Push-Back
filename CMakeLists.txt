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
# Compilation instructions

# Variables to use in compilation flags
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

# Combine flags
add_compile_options(${CPP_FLAGS} ${MFLAGS} ${GCCFLAGS} ${WARNFLAGS})

# Linker flags and scripts
set(LINKER_SCRIPT_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/Push_Back/firmware/v5.ld)
set(LINKER_SCRIPT_COMMON ${CMAKE_CURRENT_SOURCE_DIR}/Push_Back/firmware/v5-common.ld)
add_link_options(${MFLAGS} ${WARNFLAGS} ${GCCFLAGS}
    -T${LINKER_SCRIPT_MAIN}
    -T${LINKER_SCRIPT_COMMON}
    -Wl,--gc-sections
    -Wl,--no-warn-rwx-segments
    -Wl,--sort-section=alignment
    -Wl,--sort-common
    -nostdlib
)

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
endif()
if(THINK)
    add_definitions(-DTHINK)
endif()
if (BRAIN)
    add_definitions(-DBRAIN)
endif()
if (PI)
    add_definitions(-DPI)
endif()

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

##############################################################s
# Build the executable

add_executable(Programming_Push_Back
    ${SRC_FILES}
    ${TOP_LEVEL_INCLUDE}
)

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

target_link_libraries(Programming_Push_Back PRIVATE ${GROUPED_LIBS})
target_include_directories(Programming_Push_Back PRIVATE
    Push_Back/include
    Push_Back/include/pros
)
