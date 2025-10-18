cmake_minimum_required(VERSION 3.22.1)
project(Programming_Push_Back)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find relevent files
file(GLOB_RECURSE SRC_FILES Push_Back/src/*.cpp Push_Back/src/*.hpp Push_Back/src/*.h)
file(GLOB_RECURSE PROS_FILES Push_Back/include/pros/*.hpp Push_Back/include/pros/*.h)
file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS ./Push_Back/src/test/*.cc ./Push_Back/src/common/test/*.cc)

# Exclude build and external directories from source/header globs
list(FILTER SRC_FILES EXCLUDE REGEX ".*/cmake-build-.*|.*/_deps/.*|.*/CMakeFiles/.*")
list(FILTER HEADER_FILES EXCLUDE REGEX ".*/cmake-build-.*|.*/_deps/.*|.*/CMakeFiles/.*")

include_directories(Push_Back/include)
include_directories(Push_Back/include/pros)
include_directories(Push_Back/src)

# Loop to include all src directories
file(GLOB_RECURSE SRC_DIRS LIST_DIRECTORIES true Push_Back/src/*)
foreach(dir ${SRC_DIRS})
    if(IS_DIRECTORY ${dir})
        include_directories(${dir})
    endif()
endforeach()

# Need to: sudo apt-get install pkg-config  libusb-1.0-0-dev
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

if (DO)
    add_definitions(-DDO)
elseif(THINK)
    add_definitions(-DTHINK)
endif()

# For the common module
add_definitions(-DBRAIN)

####################################################

# Build a static library from project sources so tests can link against it
add_library(Programming_Push_Back_lib STATIC
        ${SRC_FILES}
        ${HEADER_FILES}
)

# Get the GTEST framework
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
        DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(TestSuite  # Create test exec
        ${TEST_FILES}
)
target_link_libraries(  # Link tests with source files and test framework
        TestSuite
        Programming_Push_Back_lib
        GTest::gtest_main
        ${LIBUSB_LIBRARIES}
)

target_include_directories(TestSuite PRIVATE ${LIBUSB_INCLUDE_DIRS})

include(GoogleTest)
gtest_discover_tests(TestSuite DISCOVERY_MODE PRE_TEST)

target_compile_options(TestSuite PRIVATE -Wno-deprecated-declarations)
target_compile_options(TestSuite PRIVATE -Wno-psabi)


####################################################

# Removing cross-compilation settings for GTEST to build properly
#set(CMAKE_SYSTEM_NAME Generic)
#set(CMAKE_SYSTEM_PROCESSOR arm)
#
#set(CMAKE_C_COMPILER arm-none-eabi-gcc)
#set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
#set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
#
#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

link_directories(Push_Back/firmware)

add_executable(Programming_Push_Back
        Push_Back/include/api.h
        Push_Back/include/main.h

        ${PROS_FILES}
        ${SRC_FILES}
)
target_link_libraries(Programming_Push_Back ${LIBUSB_LIBRARIES})

target_compile_options(Programming_Push_Back PRIVATE -Wno-deprecated-declarations)
target_compile_options(Programming_Push_Back PRIVATE -Wno-psabi)
