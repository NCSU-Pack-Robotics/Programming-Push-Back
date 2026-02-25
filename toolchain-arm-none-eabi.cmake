# Cross compilation toolchain setup

# Tell CMake we're cross-compiling for ARM architecture
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Set the toolchain for cross-compilation
set(TOOLCHAIN_PREFIX arm-none-eabi-)

# Compilers
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc)

# Utils
set(CMAKE_AR ${TOOLCHAIN_PREFIX}ar)
set(CMAKE_LINKER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE ${TOOLCHAIN_PREFIX}size)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Skip compiler test for cross-compilation
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
