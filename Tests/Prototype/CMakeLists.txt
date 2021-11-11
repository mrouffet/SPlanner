# Copyright (c) 2021 Sapphire's Suite. All Rights Reserved.



# Projects

project(SPlanner_Proto)



# Inputs

### Add executable target.
add_executable(SPlanner_Proto "main.cpp")



# Dependencies

### Add library dependencies.
target_link_libraries(SPlanner_Proto PRIVATE SPlanner)



# Tests

add_test(NAME CSPlanner_Proto COMMAND SPlanner_Proto)
