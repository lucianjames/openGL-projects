#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <charconv>

// Read an OBJ file and output a file which can easily be read as a VBO (One line contains all vertex information, another the indices)
// This program needs to:
// 1: Read all data from an obj file
// 2: Create a VBO with all vertex data (expand the indices out to the full number of vertices)
// 3: Prune the VBO to remove duplicate vertices
// 4: Create a VBO to make up for the pruning

struct vertexIndices{
    int vertexIndex;
    int normalIndex;
    int textureIndex;
};

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


class objVBO{
private:
    template<typename T>
    void printVector(const std::vector<T>& data){
        for(int i = 0; i < data.size(); i++){
            std::cout << data[i] << ", ";
        }
        std::cout << std::endl;
    }

public:
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> textureCoords;
    std::vector<vertexIndices> indices; // Index data

    std::vector<float> VBO; // The actual VBO data derived from the above data.
    std::vector<int> VBO_indices; // The indices of the VBO data derived from the above data.

    objVBO(const std::string fileName){
        readObjValues(fileName, "v ", 3, positions);
        readObjValues(fileName, "vn", 3, normals);
        readObjValues(fileName, "vt", 2, textureCoords);
        readObjIndices(fileName, indices);
    }

    void debugPrintData(){
        // Print position values
        std::cout << "Positions: " << std::endl;
        printVector(positions);
        // Print normal values
        std::cout << "Normals: " << std::endl;
        printVector(normals);
        // Print texture coordinate values
        std::cout << "Texture Coords: " << std::endl;
        printVector(textureCoords);
        std::cout << "==========================================================" << std::endl;
        // Print vertex indices
        std::cout << "Vertex Indices: " << std::endl;
        for(int i = 0; i < indices.size(); i++){
            std::cout << indices[i].vertexIndex << ", ";
            // New line every 3 indices.
            if(i % 3 == 2){
                std::cout << std::endl;
            }
        }
        std::cout << "Normal Indices: " << std::endl;
        for(int i = 0; i < indices.size(); i++){
            std::cout << indices[i].normalIndex << ", ";
            // New line every 3 indices.
            if(i % 3 == 2){
                std::cout << std::endl;
            }
        }
        std::cout << "Texture Indices: " << std::endl;
        for(int i = 0; i < indices.size(); i++){
            std::cout << indices[i].textureIndex << ", ";
            // New line every 3 indices.
            if(i % 3 == 2){
                std::cout << std::endl;
            }
        }
    }

    void assembleVBO(){
        // 1: Create a VBO with all vertex data (expand the indices out to the full number of vertices)
        // Set indices to count up from 0 to the number of vertices. (Will be optimised later on)

        this->VBO.clear();
        int vertSize = 3 + 3 + 2; // 3 floats for position, 3 floats for normal, 2 floats for texture coordinate. !!!!!! HARD CODE !!!!!!
        this->VBO.resize(vertSize * indices.size()); // Resize the VBO to the correct size.
        this->VBO_indices.clear();
        this->VBO_indices.resize(positions.size() / 3); // The number of vertices is the number of positions divided by 3 because each vertex has 3 float values.

        // !!!!!! HARD CODED - IDEALLY CAN ADAPT IF ANY ATTRIBUTE IS MISSING !!!!!!
        for(int i=0; i < this->VBO.size() / vertSize; i++){
            // Read position data from the indices.
            this->VBO[(i * vertSize) + 0] = positions[((this->indices[i].vertexIndex-1) * 3) + 0]; // The -1 is because the obj file indices start at 1, not 0. (Which is fucking retarded)
            this->VBO[(i * vertSize) + 1] = positions[((this->indices[i].vertexIndex-1) * 3) + 1];
            this->VBO[(i * vertSize) + 2] = positions[((this->indices[i].vertexIndex-1) * 3) + 2];
            // Read normal data
            this->VBO[(i * vertSize) + 3] = normals[((this->indices[i].normalIndex-1) * 3) + 0];
            this->VBO[(i * vertSize) + 4] = normals[((this->indices[i].normalIndex-1) * 3) + 1];
            this->VBO[(i * vertSize) + 5] = normals[((this->indices[i].normalIndex-1) * 3) + 2];
            // Read texCoord data
            this->VBO[(i * vertSize) + 6] = textureCoords[((this->indices[i].textureIndex-1) * 2) + 0];
            this->VBO[(i * vertSize) + 7] = textureCoords[((this->indices[i].textureIndex-1) * 2) + 1];
        }

        // Create the basic indices (just count up lol)
        for(int i = 0; i < this->VBO.size() / vertSize; i++){
            this->VBO_indices[i] = i;
        }

        // Print the entire VBO
        std::cout << "VBO: " << std::endl;
        for(int i = 0; i < this->VBO.size(); i++){
            std::cout << this->VBO[i] << ", ";
            // New line every vertSize indices.
            if(i % vertSize == vertSize - 1){
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;

        // Print the indices
        std::cout << "VBO Indices: " << std::endl;
        for(int i = 0; i < this->VBO_indices.size(); i++){
            std::cout << this->VBO_indices[i] << ", ";
        }
        std::cout << std::endl;
    }

};


int main(){
    objVBO obj("../triangle.obj");
    //obj.debugPrintData();
    obj.assembleVBO();
    return 0;
}