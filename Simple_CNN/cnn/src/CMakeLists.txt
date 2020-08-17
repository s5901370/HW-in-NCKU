file(GLOB_RECURSE SRC "*.cpp")
list(REMOVE_ITEM SRC "scnn.cpp")
add_library(cnn ${SRC})

add_executable(scnn scnn.cpp)
target_link_libraries(scnn cnn)