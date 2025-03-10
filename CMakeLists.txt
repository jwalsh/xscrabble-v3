cmake_minimum_required(VERSION 3.10)
project(XScrabble VERSION 3.0.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find X11 libraries
find_package(X11 REQUIRED)

# Include directories
include_directories(include ${X11_INCLUDE_DIR})

# Add source files
file(GLOB SOURCES "src/*.c")

# Define executable
add_executable(xscrabble ${SOURCES})

# Link libraries
target_link_libraries(xscrabble PRIVATE 
    ${X11_LIBRARIES}
    ${X11_Xt_LIB}
    ${X11_Xaw_LIB}
    ${X11_Xmu_LIB}
)

# Install targets
install(TARGETS xscrabble DESTINATION bin)
install(DIRECTORY resources/ DESTINATION share/xscrabble)

# Testing
enable_testing()
add_subdirectory(tests)
