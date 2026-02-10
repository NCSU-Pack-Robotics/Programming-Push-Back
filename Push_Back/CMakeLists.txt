cmake_minimum_required(VERSION 3.22.1)

################################################################################
# Toolchain Configuration (must be before project())
################################################################################
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(ARCHTUPLE arm-none-eabi-)
set(CMAKE_C_COMPILER ${ARCHTUPLE}gcc)
set(CMAKE_CXX_COMPILER ${ARCHTUPLE}g++)
set(CMAKE_ASM_COMPILER ${ARCHTUPLE}gcc)
set(CMAKE_AR ${ARCHTUPLE}ar)
set(CMAKE_OBJCOPY ${ARCHTUPLE}objcopy)
set(CMAKE_SIZE ${ARCHTUPLE}size)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Skip compiler test for cross-compilation
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(VEX_V5_Project C CXX ASM)

################################################################################
# Project Configuration
################################################################################
set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(FWDIR ${ROOT}/firmware)
set(BINDIR ${ROOT}/bin)
set(SRCDIR ${ROOT}/src)
set(INCDIR ${ROOT}/include)

# File extensions
set(CEXTS c)
set(ASMEXTS s S)
set(CXXEXTS cpp c++ cc)

# Standards
set(CMAKE_C_STANDARD 2x)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Exclude directories
set(EXCLUDE_SRCDIRS ${SRCDIR}/test ${SRCDIR}/common/test)

# Use package mode (hot/cold linking)
set(USE_PACKAGE 1)

################################################################################
# Compiler Flags
################################################################################
# Use string() to properly handle flags as space-separated values
string(CONCAT MFLAGS 
    "-mcpu=cortex-a9 "
    "-mfpu=neon-fp16 "
    "-mfloat-abi=hard "
    "-Os "
    "-g "
    "-mthumb"
)

string(CONCAT CPPFLAGS
    "-D_POSIX_THREADS "
    "-D_UNIX98_THREAD_MUTEX_ATTRIBUTES "
    "-D_POSIX_TIMERS "
    "-D_POSIX_MONOTONIC_CLOCK"
)

string(CONCAT GCCFLAGS
    "-ffunction-sections "
    "-fdata-sections "
    "-fdiagnostics-color "
    "-funwind-tables"
)

set(WARNFLAGS "-Wno-psabi")

# Check for liblvgl llemu files
if(EXISTS ${INCDIR}/liblvgl/llemu.h)
    string(APPEND CPPFLAGS " -D_PROS_INCLUDE_LIBLVGL_LLEMU_H")
endif()
if(EXISTS ${INCDIR}/liblvgl/llemu.hpp)
    string(APPEND CPPFLAGS " -D_PROS_INCLUDE_LIBLVGL_LLEMU_HPP")
endif()

# Combine flags
set(CMAKE_C_FLAGS "${MFLAGS} ${CPPFLAGS} ${WARNFLAGS} ${GCCFLAGS}")
set(CMAKE_CXX_FLAGS "${MFLAGS} ${CPPFLAGS} ${WARNFLAGS} ${GCCFLAGS}")
set(CMAKE_ASM_FLAGS "${MFLAGS} ${WARNFLAGS}")

# Add extra flags from environment or command line
if(DEFINED ENV{EXTRA_CFLAGS})
    string(APPEND CMAKE_C_FLAGS " $ENV{EXTRA_CFLAGS}")
endif()
if(DEFINED ENV{EXTRA_CXXFLAGS})
    string(APPEND CMAKE_CXX_FLAGS " $ENV{EXTRA_CXXFLAGS}")
endif()

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS "${MFLAGS} ${WARNFLAGS} -nostdlib ${GCCFLAGS}")

# Create list versions for use in custom commands with VERBATIM
separate_arguments(LINKER_FLAGS_LIST UNIX_COMMAND "${CMAKE_EXE_LINKER_FLAGS}")
separate_arguments(C_FLAGS_LIST UNIX_COMMAND "${CMAKE_C_FLAGS}")

################################################################################
# Include Directories
################################################################################
include_directories(${INCDIR})
include_directories(dependencies/common/src)

################################################################################
# Find Source Files
################################################################################
file(GLOB_RECURSE C_SOURCES
    ${SRCDIR}/*.c
)
file(GLOB_RECURSE CXX_SOURCES
    ${SRCDIR}/*.cpp
    ${SRCDIR}/*.c++
    ${SRCDIR}/*.cc
)
file(GLOB_RECURSE ASM_SOURCES
    ${SRCDIR}/*.s
    ${SRCDIR}/*.S
)

# Filter out excluded directories
foreach(exclude_dir ${EXCLUDE_SRCDIRS})
    list(FILTER C_SOURCES EXCLUDE REGEX "^${exclude_dir}/.*")
    list(FILTER CXX_SOURCES EXCLUDE REGEX "^${exclude_dir}/.*")
    list(FILTER ASM_SOURCES EXCLUDE REGEX "^${exclude_dir}/.*")
endforeach()

set(ALL_SOURCES ${C_SOURCES} ${CXX_SOURCES} ${ASM_SOURCES})

################################################################################
# Libraries
################################################################################
file(GLOB FIRMWARE_LIBS ${FWDIR}/*.a)
set(LIBRARIES ${FIRMWARE_LIBS})

# Add common library if it exists
set(COMMON_LIB ${ROOT}/dependencies/common/arm-build/libProgramming_Push_Back_Common_lib.a)
if(EXISTS ${COMMON_LIB})
    list(APPEND LIBRARIES ${COMMON_LIB})
endif()

# Exclude libraries from cold image
set(EXCLUDE_COLD_LIBRARIES ${FWDIR}/libc.a ${FWDIR}/libm.a)
set(COLD_LIBRARIES ${LIBRARIES})
list(REMOVE_ITEM COLD_LIBRARIES ${EXCLUDE_COLD_LIBRARIES})

################################################################################
# Linker Script Flags
################################################################################
# LNK_FLAGS for custom commands (includes libraries)
set(LNK_FLAGS
    -Wl,--gc-sections
    -Wl,--start-group
    ${LIBRARIES}
    -lgcc
    -lstdc++
    -Wl,--end-group
    -Wl,-T${FWDIR}/v5-common.ld
    -Wl,--no-warn-rwx-segments
    -Wl,--sort-section=alignment
    -Wl,--sort-common
)

# LNK_FLAGS_STR for add_executable targets (without libraries, they use target_link_libraries)
set(LNK_FLAGS_NO_LIBS
    -Wl,--gc-sections
    -Wl,--start-group
    -lgcc
    -lstdc++
    -Wl,--end-group
    -Wl,-T${FWDIR}/v5-common.ld
    -Wl,--no-warn-rwx-segments
    -Wl,--sort-section=alignment
    -Wl,--sort-common
)

# Convert to space-separated strings
string(REPLACE ";" " " LNK_FLAGS_STR "${LNK_FLAGS_NO_LIBS}")

################################################################################
# Build Targets
################################################################################

# Create timestamp object
add_custom_command(
    OUTPUT ${BINDIR}/_pros_ld_timestamp.o
    COMMAND ${CMAKE_COMMAND} -E make_directory ${BINDIR}
    COMMAND sh -c "echo 'const int _PROS_COMPILE_TIMESTAMP_INT = 0; char const * const _PROS_COMPILE_TIMESTAMP = __DATE__ \" \" __TIME__; char const * const _PROS_COMPILE_DIRECTORY = \"${ROOT}\";' | ${CMAKE_C_COMPILER} -c -x c ${C_FLAGS_LIST} -o ${BINDIR}/_pros_ld_timestamp.o -"
    VERBATIM
)

if(USE_PACKAGE EQUAL 1)
    # Hot/Cold Package Build

    # Cold ELF - use custom command since it doesn't use object files
    add_custom_command(
        OUTPUT ${BINDIR}/cold.package.elf
        COMMAND ${CMAKE_COMMAND} -E make_directory ${BINDIR}
        COMMAND ${CMAKE_CXX_COMPILER} ${LINKER_FLAGS_LIST}
            -Wl,--gc-keep-exported
            -Wl,--whole-archive
            ${COLD_LIBRARIES}
            -lstdc++
            -Wl,--no-whole-archive
            -Wl,-T${FWDIR}/v5.ld
            ${LNK_FLAGS}
            -o ${BINDIR}/cold.package.elf
        COMMAND ${CMAKE_OBJCOPY}
            --strip-symbol=install_hot_table
            --strip-symbol=__libc_init_array
            --strip-symbol=_PROS_COMPILE_DIRECTORY
            --strip-symbol=_PROS_COMPILE_TIMESTAMP
            --strip-symbol=_PROS_COMPILE_TIMESTAMP_INT
            ${BINDIR}/cold.package.elf
            ${BINDIR}/cold.package.elf
        DEPENDS ${COLD_LIBRARIES}
        VERBATIM
    )

    # Cold BIN
    add_custom_command(
        OUTPUT ${BINDIR}/cold.package.bin
        COMMAND ${CMAKE_OBJCOPY} ${BINDIR}/cold.package.elf -O binary -R .hot_init ${BINDIR}/cold.package.bin
        DEPENDS ${BINDIR}/cold.package.elf
        VERBATIM
    )
    
    add_custom_target(cold_package DEPENDS ${BINDIR}/cold.package.elf ${BINDIR}/cold.package.bin)

    # Hot ELF - use add_executable to properly handle TARGET_OBJECTS
    add_executable(hot_package_elf ${BINDIR}/_pros_ld_timestamp.o)
    target_sources(hot_package_elf PRIVATE $<TARGET_OBJECTS:vex_project_objects>)
    set_target_properties(hot_package_elf PROPERTIES
        OUTPUT_NAME hot.package.elf
        RUNTIME_OUTPUT_DIRECTORY ${BINDIR}
        SUFFIX ""
        LINK_FLAGS "-nostartfiles -Wl,-R${BINDIR}/cold.package.elf -Wl,-T${FWDIR}/v5-hot.ld ${LNK_FLAGS_STR}"
        LINK_DEPENDS "${BINDIR}/cold.package.elf"
    )
    target_link_libraries(hot_package_elf ${LIBRARIES})
    add_dependencies(hot_package_elf cold_package vex_project_objects)

    # Hot BIN
    add_custom_command(
        OUTPUT ${BINDIR}/hot.package.bin
        COMMAND ${CMAKE_OBJCOPY} ${BINDIR}/hot.package.elf -O binary ${BINDIR}/hot.package.bin
        DEPENDS hot_package_elf ${BINDIR}/cold.package.bin
        VERBATIM
    )

    set(DEFAULT_BIN ${BINDIR}/hot.package.bin)

else()
    # Monolith Build

    # Monolith ELF - use add_executable to properly handle TARGET_OBJECTS
    add_executable(monolith_elf ${BINDIR}/_pros_ld_timestamp.o)
    target_sources(monolith_elf PRIVATE $<TARGET_OBJECTS:vex_project_objects>)
    set_target_properties(monolith_elf PROPERTIES
        OUTPUT_NAME monolith.elf
        RUNTIME_OUTPUT_DIRECTORY ${BINDIR}
        SUFFIX ""
        LINK_FLAGS "-Wl,-T${FWDIR}/v5.ld ${LNK_FLAGS_STR}"
    )
    target_link_libraries(monolith_elf ${LIBRARIES})
    add_dependencies(monolith_elf vex_project_objects)

    # Monolith BIN (default target)
    add_custom_command(
        OUTPUT ${BINDIR}/monolith.bin
        COMMAND ${CMAKE_OBJCOPY} ${BINDIR}/monolith.elf -O binary -R .hot_init ${BINDIR}/monolith.bin
        DEPENDS monolith_elf
        VERBATIM
    )

    set(DEFAULT_BIN ${BINDIR}/monolith.bin)

endif()

# Object library for source files
add_library(vex_project_objects OBJECT ${ALL_SOURCES})
target_include_directories(vex_project_objects PRIVATE ${INCDIR})
target_include_directories(vex_project_objects PRIVATE dependencies/common/src)

# Main target
add_custom_target(quick ALL DEPENDS ${DEFAULT_BIN})

# Clean target
add_custom_target(clean-vex
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${BINDIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory .d
)
