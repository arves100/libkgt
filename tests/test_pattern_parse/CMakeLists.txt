add_executable(test_pattern_parse test_pattern_parse.c)
set_target_properties(test_pattern_parse PROPERTIES FOLDER "tests")
target_link_libraries(test_pattern_parse PRIVATE kgt)
