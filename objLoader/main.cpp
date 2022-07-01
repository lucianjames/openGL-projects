// This exists to test an OBJ file reader.
// Main functionality:
// 1 - Read postion, normal, texCoord, and index data from an OBJ file.
// 2 - Arrange the data into a vertex buffer and index buffer.
// 3 - Create a vertex buffer layout object.
// 4 - Be able to integrate this with the pre-existing classes in the other files.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <charconv>
#include "VBO_layout.h"

template<typename T> // This function is templated, but I dont think i actually need it to be lol. Whatever
void readObjValues(const std::string fileName, const std::string startTag, const int dataCount, std::vector<T>& data){
    // Clear the data vector.
    data.clear();
    std::ifstream file(fileName);
    std::string line;
    while(std::getline(file, line)){ // 1: Read a line from the file
        if(line.substr(0,startTag.size()) == startTag){ // 2: Check if it starts with the startTag
            // 3: If so, break it down into <dataCount> ints and add them to the data vector:
            std::stringstream ss(line); 
            std::string token;
            std::getline(ss, token, ' '); // Skip the token at the beginning because it is not a number that we want.
            for(int i = 0; i < dataCount; i++){ // Read the rest of the tokens as ints.
                std::getline(ss, token, ' '); // Read the next token.
                T temp;
                std::from_chars(token.data(), token.data() + token.size(), temp);
                data.push_back(temp); // Push back may be slow, i think.
            }
        }
    }
}

class obj{
public: // Make everything public  /////// !!!ONLY FOR NOW!!! ///////
    // These vectors will have the data read into them:
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> texCoords;
    
    // These will actually be used in openGL:
    std::vector<float> vertices; // Combines the position, normal, and texCoord data into one vector which can be used to create a vertex buffer.
    std::vector<unsigned int> positionIndices; // Indices of the position data.
    VBO_layout layout; // This is the layout of the vertex buffer.

    // Information required to load the file:
    std::string fileName;

    obj(std::string fileName){
        this->fileName = fileName;
        // Check file exists:
        std::ifstream file(fileName);
        if(!file.is_open()){
            // Throw a runtime error
            std::runtime_error("std::ifstream failed: " + fileName);
            return;
        }
    }

    void readPositionData(){
        readObjValues(this->fileName, "v", 3, this->positions);
    }

    void readNormalData(){
        readObjValues(this->fileName, "vn", 3, this->normals);
    }

    void readTexCoordData(){
        readObjValues(this->fileName, "vt", 2, this->texCoords);
    }

};


int main(){
    obj object("../test.obj");

    // Reading the data:
    std::cout << "Reading data..." << std::endl;
    object.readPositionData();
    object.readNormalData();
    object.readTexCoordData();
    std::cout << "Data read." << std::endl;

    // Display the data:
    std::cout << "Positions: " << std::endl;
    for(int i = 0; i < object.positions.size(); i++){
        std::cout << object.positions[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Normals: " << std::endl;
    for(int i = 0; i < object.normals.size(); i++){
        std::cout << object.normals[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "TexCoords: " << std::endl;
    for(int i = 0; i < object.texCoords.size(); i++){
        std::cout << object.texCoords[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "PositionIndices: " << std::endl;
    for(int i = 0; i < object.positionIndices.size(); i++){
        std::cout << object.positionIndices[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}