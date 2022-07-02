#include <iostream>

#include "objVBO.h"

// Read an OBJ file and output a file which can easily be read as a VBO (One line contains all vertex information, another the indices)
// This program needs to:
// 1: Read all data from an obj file
// 2: Create a VBO with all vertex data (expand the indices out to the full number of vertices)
// 3: Prune the VBO to remove duplicate vertices
// 4: Create a VBO to make up for the pruning

int main(){
    objVBO::object obj("../cube.obj");
    //obj.debugPrintData();
    obj.assembleVBO();
    obj.writeVBO("../cube");
    obj.writeEBO("../cube");
    return 0;
}