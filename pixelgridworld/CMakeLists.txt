#//// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= ////
#//// Pixelwridworld
#////
#//// MIT Licanse
#////
#//// Copyright (c) 2022 Mikko Romppainen.
#////
#//// Permission is hereby granted, free of charge, to any person obtaining
#//// a copy of this software and associated documentation files (the
#//// "Software"), to deal in the Software without restriction, including
#//// without limitation the rights to use, copy, modify, merge, publish,
#//// distribute, sublicense, and/or sell copies of the Software, and to
#//// permit persons to whom the Software is furnished to do so, subject to
#//// the following conditions:
#////
#//// The above copyright notice and this permission notice shall be included
#//// in all copies or substantial portions of the Software.
#////
#//// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#//// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
#//// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
#//// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
#//// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
#//// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
#//// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#//// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= ////

# Specify minimum supported cmake version
cmake_minimum_required(VERSION 3.5)

##
## Dependencies
##
# add_subdirectorys to be CMaked
option(GLFW_BUILD_EXAMPLES "" OFF)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_BUILD_DOCS "" OFF)
option(GLFW_INSTALL "" OFF)
add_subdirectory("./ext/glfw-3.3.6")
# Specify include directories
include_directories("ext/glfw-3.3.6/include")
include_directories("ext/stb-master")
include_directories("ext/freeglut-3.2.1/include")
include_directories("${GLFW_SOURCE_DIR}/deps")
include_directories("include")
# Define glad source files
set(GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h" "${GLFW_SOURCE_DIR}/deps/glad_gl.c")

## Use C++17, visibility=hidden by default, interprocedural optimization
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)


## Project pixelgridworld
project (pixelgridworld)
option(PIXELGRIDWORLD_BUILD_EXAMPLES "Build pixelgridworld examples" ON )

file(GLOB PIXELGRIDWORLD_INC_FILES "./include/pixelgridworld/*.h")

add_library(pixelgridworld "src/window.cpp" ${PIXELGRIDWORLD_INC_FILES} ${GLAD_GL})

if(PIXELGRIDWORLD_BUILD_EXAMPLES)
    add_executable (demo src/main_demo.cpp include/pixelgridworld/window.h include/pixelgridworld/gridworld.h)
    target_link_libraries(demo PUBLIC pixelgridworld PUBLIC glfw)
endif()
