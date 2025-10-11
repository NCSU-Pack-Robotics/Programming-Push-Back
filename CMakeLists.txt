cmake_minimum_required(VERSION 3.22.1)
project(Programming_Push_Back)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

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



if (DO)
    add_definitions(-DDO)
elseif(THINK)
    add_definitions(-DTHINK)
endif()

# For the common module
add_definitions(-DBRAIN)

file(GLOB_RECURSE SRC_FILES Push_Back/src/*.cpp Push_Back/src/*.hpp Push_Back/src/*.h)
file(GLOB_RECURSE PROS_FILES Push_Back/include/pros/*.hpp Push_Back/include/pros/*.h)

link_directories(Push_Back/firmware)

add_executable(Programming_Push_Back
        Push_Back/include/api.h
        Push_Back/include/main.h

        ${PROS_FILES}
        ${SRC_FILES}
)

target_compile_options(Programming_Push_Back PRIVATE -Wno-deprecated-declarations)
target_compile_options(Programming_Push_Back PRIVATE -Wno-psabi)

