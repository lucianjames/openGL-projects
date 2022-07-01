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


struct vertexIndices{
    int vertexIndex;
    int normalIndex;
    int textureIndex;
};

// Read specifically the indices from the obj file. Use the above struct to make it easier to use them later on.
void readObjIndices(const std::string fileName, std::vector<vertexIndices>& data){
    data.clear();
    std::ifstream file(fileName);
    std::string line;
    while(std::getline(file, line)){ // 1: Read a line from the file
        if(line.substr(0,2) == "f "){ // 2: Check if it starts with the startTag
            // 3: If so, break it down into <dataCount> ints and add them to the data vector:
            std::stringstream ss(line); 
            std::string token;
            std::getline(ss, token, ' '); // Skip the token at the beginning because it is not a number that we want.
            for(int i = 0; i < 3; i++){ // Read the rest of the tokens as ints.
                std::getline(ss, token, ' '); // Read the next token.
                vertexIndices temp;
                std::stringstream ss2(token);
                std::getline(ss2, token, '/'); // Get the first token.
                std::from_chars(token.data(), token.data() + token.size(), temp.vertexIndex);
                std::getline(ss2, token, '/'); // Get the second token.
                std::from_chars(token.data(), token.data() + token.size(), temp.textureIndex);
                std::getline(ss2, token, '/'); // Get the third token.
                std::from_chars(token.data(), token.data() + token.size(), temp.normalIndex);
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
    std::vector<vertexIndices> indices;

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

    void readNormalData(){
        readObjValues(this->fileName, "vn", 3, this->normals);
    }

    void readIndexData(){
        readObjIndices(this->fileName, this->indices);
    }

    void createVBO(){
        // First lets set the index data up
        for(int i = 0; i < this->indices.size(); i++){
            const auto& index = this->indices[i];
            this->positionIndices.push_back(index.vertexIndex - 1);
        }

        // We only have position data right now, so we can just use that.
        this->VBO.clear();

        // Figure out how many vertices need to be made:
        int n_verts = this->positions.size()/3; // Lets assume this is fine.

        // Debug print size of data
        std::cout << "n_verts: " << n_verts << std::endl;
        std::cout << "positions.size(): " << this->positions.size() << std::endl;
        std::cout << "positionIndices.size(): " << this->positionIndices.size() << std::endl;
        std::cout << "normals.size(): " << this->normals.size() << std::endl;
        std::cout << "this->indices.size(): " << this->indices.size() << std::endl;

        // For every vert requried, do some shit :)
        for(int v=0; v < n_verts; v++){
            // Add position data
            this->VBO.push_back(this->positions[(v*3)]);
            this->VBO.push_back(this->positions[(v*3)+1]);
            this->VBO.push_back(this->positions[(v*3)+2]);
            // Add normal data
            this->VBO.push_back(this->normals[(v*3)]);
            this->VBO.push_back(this->normals[(v*3)+1]);
            this->VBO.push_back(this->normals[(v*3)+2]);
        }

        this->layout.pushFloat(3);
        this->layout.pushFloat(3);

    }

};