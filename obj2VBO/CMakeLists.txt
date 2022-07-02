cmake_minimum_required(VERSION 3.23.1) # Set based on the current version im using, will probably work on lots of other versions too
project(OBJ2VBO)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3") # Optimisation!!!

set(SOURCE
    ${SOURCE}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/objVBO.cpp
)

add_executable(main ${SOURCE}) # Add the source files to the executable

target_link_libraries(main)

# A warning to anyone reading this cmake file:
#   I am fucking retarded and I don't know how to use cmake properly. :)