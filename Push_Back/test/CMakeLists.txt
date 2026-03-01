file(GLOB_RECURSE TEST_SOURCES "*.cc")

##################################################################################
# Project Files as Library
##################################################################################

# Build a static library from project sources so tests can link against it
add_library(Programming_Push_Back_lib STATIC
        ${SRC_FILES}
        ${HEADER_FILES}
)

target_link_libraries(Programming_Push_Back_lib PUBLIC
        config
        ${BUILT_LIBS}
)

# Suppress  warnings from PROS headers
target_compile_options(Programming_Push_Back_lib PRIVATE
        -Wno-deprecated-literal-operator  # May not be necessary with some compilers
        -Wno-deprecated-declarations
        -Wno-psabi
        -Wno-cpp
)

##################################################################################
# Tests
##################################################################################

# Get the GTEST framework
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
        DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(googletest)
enable_testing()

set(TestSuite TestSuite_${PROJECT_NAME})

add_executable(${TestSuite}  # Create test exec
        ${TEST_SOURCES}
)

target_link_libraries(${TestSuite}  # Link tests with source files and test framework
        Programming_Push_Back_lib
        GTest::gtest_main
        ${LIBUSB_LIBRARIES}
        config
)
target_include_directories(${TestSuite} PRIVATE
        ${LIBUSB_INCLUDE_DIRS}
        .
        ${CMAKE_SOURCE_DIR}/Push_Back/include
)

include(GoogleTest)
gtest_discover_tests(${TestSuite} DISCOVERY_MODE PRE_TEST)
