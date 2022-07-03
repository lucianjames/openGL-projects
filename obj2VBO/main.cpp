#include <iostream>

#include "objVBO.h"

// Read an OBJ file and output a file which can easily be read as a VBO (One line contains all vertex information, another the indices)
// This program needs to:
// 1: Read all data from an obj file
// 2: Create a VBO with all vertex data (expand the indices out to the full number of vertices)
// 3: Prune the VBO to remove duplicate vertices
// 4: Create a VBO to make up for the pruning

int main(int argc, char** argv){
    // Error checking
    if(argc != 3){
        std::cout << "Usage: " << argv[0] << " <input file> <output file>" << std::endl;
        return 1;
    }

    // Get IO variables from command line arg
    std::string input = argv[1]; // argv[0] is the program name.
    std::string output = argv[2]; // argv[1] is the input file.

    objVBO::object obj(input);
    obj.optimiseVBO();
    obj.writeVBO(output);
    obj.writeEBO(output);
    std::cout << "==========================================================" << std::endl;
    return 0;
}