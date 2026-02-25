add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/common)
list(APPEND BUILT_LIBS Programming_Push_Back_Common_lib)

set(BUILT_LIBS ${BUILT_LIBS} PARENT_SCOPE)  # Must be after everything for some reason
