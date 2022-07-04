cmake_minimum_required(VERSION 3.16)
project(main)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(${PROJECT_NAME}
        ./main.cpp
        )

include_directories(${OPENGL_INCLUDE_DIRS}, ${GLUT_INCLUDE_DIRS})

target_link_libraries(main ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})