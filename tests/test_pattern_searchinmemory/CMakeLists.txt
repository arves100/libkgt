add_executable(test_pattern_searchinmemory test_pattern_searchinmemory.c)
set_target_properties(test_pattern_searchinmemory PROPERTIES FOLDER "tests")
target_link_libraries(test_pattern_searchinmemory PRIVATE kgt)
