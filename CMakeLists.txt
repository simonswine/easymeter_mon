# Minimum Version
cmake_minimum_required(VERSION 2.6)

# Print Values of Easymeter Q3XXX
project(easymeter_mon)

# Get git hash
execute_process(
    COMMAND git describe --abbrev=8 --dirty --always
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Set version
set(MAIN_VERSION "0.0.1")

# export versions
configure_file( version_config.h.in ${CMAKE_BINARY_DIR}/generated/version_config.h )
include_directories( ${CMAKE_BINARY_DIR}/generated/ ) 

# C Std 99
set(CMAKE_C_FLAGS "-std=gnu99") 

## Example executable
add_executable(easymeter_mon ./src/obis.c ./src/easymeter_mon.c)

# Link sml
target_link_libraries(easymeter_mon sml m)

# Install bin
INSTALL(TARGETS easymeter_mon RUNTIME DESTINATION bin)
