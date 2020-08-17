include_directories(${PROJECT_SOURCE_DIR}/src)
add_executable(test test.cpp)
target_link_libraries(test cnn)