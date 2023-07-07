cmake_minimum_required(VERSION 3.16.3)

project(bt_manipulator_demo)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(ThirdParty/BehaviorTree.CPP/include
        include
        src)
link_directories(ThirdParty/BehaviorTree.CPP/build
        lib

)

set(SRC_FILES src/movebase_node.cpp src/movebase_node.h
        src/manipulator_node.cpp
        src/manipulator_node.h)

#add_library(dummy_nodes_dyn     SHARED src/manipulator_node.cpp  src/manipulator_node.h)
add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})


# Link against BehaviorTree.CPP library
target_link_libraries(${PROJECT_NAME} libbehaviortree_cpp.so libmyCobotCpp.so)
