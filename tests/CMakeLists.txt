# Add test executables
add_executable(test_board test_board.c ../src/board.c)
add_executable(test_game test_game.c ../src/game.c ../src/board.c ../src/dictionary.c)
add_executable(test_dictionary test_dictionary.c ../src/dictionary.c)

# Link libraries
target_link_libraries(test_board PRIVATE ${X11_LIBRARIES})
target_link_libraries(test_game PRIVATE ${X11_LIBRARIES})
target_link_libraries(test_dictionary PRIVATE ${X11_LIBRARIES})

# Add tests
add_test(NAME BoardTest COMMAND test_board)
add_test(NAME GameTest COMMAND test_game)
add_test(NAME DictionaryTest COMMAND test_dictionary)
