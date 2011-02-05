if(GTEST_FOUND)
    #Set up the exe
    include_directories(${GTEST_INCLUDE_DIRS})
    add_executable(toucan_test test.cpp rules_test.cpp path_test.cpp ../rules.cpp ../path.cpp)
    target_link_libraries(toucan_test ${GTEST_BOTH_LIBRARIES} ${wxWidgets_LIBRARIES})
endif(GTEST_FOUND)
