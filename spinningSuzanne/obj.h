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
    std::vector<float> VBO; // Vertex buffer info
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

    /* !!! Remove for now, lets just get a simple set of vertices and indices working before moving on to anything else !!!
    void readNormalData(){
        readObjValues(this->fileName, "vn", 3, this->normals);
    }

    void readTexCoordData(){
        readObjValues(this->fileName, "vt", 2, this->texCoords);
    }
    */

    void readPositionIndices(){
        readObjValues(this->fileName, "f", 3, this->positionIndices); // Use of this is a bit hacky, but it works. Ideally i should create a new function which specifically parses "/" separated indices.
        fixPositionIndices();
    }

    void fixPositionIndices(){ // Subtract one from every single index, because OBJ is a retarded file format
        for(auto& i : this->positionIndices){
            i--;
        }
    }

    void createVBO(){
        // We only have position data right now, so we can just use that.
        this->VBO.clear();
        this->VBO = this->positions;
        this->layout.pushFloat(3);
    }

};