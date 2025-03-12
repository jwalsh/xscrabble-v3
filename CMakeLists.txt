cmake_minimum_required(VERSION 3.10)
project(XScrabble VERSION 3.0.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find X11 libraries
find_package(X11 REQUIRED)

# Include directories
include_directories(include ${X11_INCLUDE_DIR})

# Add source files for main application
file(GLOB SOURCES "src/board.c" "src/dictionary.c" "src/game.c" "src/main.c" "src/ui.c")

# Define main executable
add_executable(xscrabble ${SOURCES})

# Link libraries
target_link_libraries(xscrabble PRIVATE 
    ${X11_LIBRARIES}
    ${X11_Xt_LIB}
    ${X11_Xaw_LIB}
    ${X11_Xmu_LIB}
)

# Dictionary demos
add_executable(dictionary_demo src/dictionary_demo.c src/dictionary_enhanced.c)
add_executable(al_dictionary_demo src/al_dictionary_demo.c)

# Install targets
install(TARGETS xscrabble DESTINATION bin)
install(TARGETS dictionary_demo DESTINATION bin)
install(TARGETS al_dictionary_demo DESTINATION bin)
install(DIRECTORY resources/ DESTINATION share/xscrabble)
install(DIRECTORY data/dictionaries/ DESTINATION share/xscrabble/dictionaries)

# Testing
enable_testing()
add_subdirectory(tests)
