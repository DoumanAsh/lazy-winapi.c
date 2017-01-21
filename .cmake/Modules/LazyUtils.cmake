# Set flags if compiler accepts GCC flags
macro(set_gcc_flags_if)
    if (CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_C_COMPILER_ID MATCHES "GNU")
        set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${ARGV0}")
    endif()
endmacro(set_gcc_flags_if)
