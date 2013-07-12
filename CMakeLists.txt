# Minimum Version
cmake_minimum_required(VERSION 2.6)

# Print Values of Easymeter Q3XXX
project(easymeter_mon)

# C Std 99
set(CMAKE_C_FLAGS "-std=c99") 


## Example executable
add_executable(easymeter_mon ./src/obis.c ./src/easymeter_mon.c)

# Link sml
target_link_libraries(easymeter_mon sml m)

# Install bin
INSTALL(TARGETS easymeter_mon RUNTIME DESTINATION bin)