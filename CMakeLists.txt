cmake_minimum_required(VERSION 3.16)
project(main)
set(CMAKE_CXX_STANDARD 17)

add_executable(main
        ./project/src/main.cpp
        ./project/include/StepanPlot.h
        ./project/include/utilities/StepanPlot_dop_func.h
        ./project/include/utilities/StepanPlot_data_frame.h
        ./project/include/utilities/StepanPlot_exception.h
        )

#openGL
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS}, ${GLUT_INCLUDE_DIRS})
target_link_libraries(main ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
