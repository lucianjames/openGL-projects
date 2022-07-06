#include <iostream>

#include "objVBO.h"

// Read an OBJ file and output a file which can easily be read as a VBO (One line contains all vertex information, another the indices)

int main(int argc, char** argv){
    // Error checking
    if(argc != 3){
        std::cout << "Usage: " << argv[0] << " <input file> <output file>" << std::endl;
        return 1;
    }

    // Get IO variables from command line arg
    std::string input = argv[1]; // argv[0] is the program name.
    std::string output = argv[2]; // argv[1] is the input file.

    // Read the input file, assemble the VBO, and write the output files.
    objVBO::object obj(input);
    obj.writeVBO(output);
    obj.writeEBO(output);
    return 0;
}
