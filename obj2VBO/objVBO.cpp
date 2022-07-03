#include "objVBO.h"



template<typename T> // This function is templated, but I dont think i actually need it to be lol. Whatever
void objVBO::readObjValues(const std::string fileName, const std::string startTag, const int dataCount, std::vector<T>& data){
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



// Read specifically the indices from the obj file. Use a struct to make it easier to use them later on.
void objVBO::readObjIndices(const std::string fileName, std::vector<vertexIndices>& data){
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
                std::from_chars(token.data(), token.data() + token.size(), temp.positionIndex);
                std::getline(ss2, token, '/'); // Get the second token.
                std::from_chars(token.data(), token.data() + token.size(), temp.textureIndex);
                std::getline(ss2, token, '/'); // Get the third token.
                std::from_chars(token.data(), token.data() + token.size(), temp.normalIndex);
                data.push_back(temp); // Push back may be slow, i think.
            }
        }
    }
}



template <typename T> // This function provides indices which can be used to "undo" a sort. (Basically used to keep track of where everything has been moved to when std::sort is used)
auto objVBO::sort_permutation(T cbegin, T cend) {
    auto len = std::distance(cbegin, cend);
    std::vector<size_t> perm(len);
    std::iota(perm.begin(), perm.end(), 0U);
    std::sort(
        perm.begin(), 
        perm.end(), 
        [&](const size_t& a, const size_t& b){return *(cbegin+a) < *(cbegin+b);}
    );
    return perm;
}






// =========================================================
//            OBJECT CLASS IMPLEMENTATION BELOW
// =========================================================



template<typename T>
void objVBO::object::printVector(const std::vector<T>& data){
    for(int i = 0; i < data.size(); i++){
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}



void objVBO::object::assembleVBO(){
    std::cout << "==========================================================" << std::endl;
    std::cout << "Assembling VBO:" << std::endl;
    // Check for the presence of position, normal, and texture coordinates:
    std::cout << "Checking for presence of position, normal, and texture coordinates..." << std::endl;
    if(positions.size() == 0 || normals.size() == 0 || textureCoords.size() == 0){ // !!!!!! HARD CODED !!!!!!
        std::cout << "Error: Object does not have all required data:" << std::endl;
        // Figure out which data is missing.
        if(positions.size() == 0){
            std::cout << "Missing position data" << std::endl;
        }
        if(normals.size() == 0){
            std::cout << "Missing normal data" << std::endl;
        }
        if(textureCoords.size() == 0){
            std::cout << "Missing texture coordinate data" << std::endl;
        }
        // Throw runtime error
        throw std::runtime_error("Error: Object does not have all required data.");
    }


    // Create a VBO with all vertex data (expand the indices out to the full number of vertices)
    // Set indices to count up from 0 to the number of vertices. (This can be optimised later on)
    this->VBO.clear();
    this->vertSize = 3 + 3 + 2; // 3 floats for position, 3 floats for normal, 2 floats for texture coordinate. !!!!!! HARD CODED !!!!!!
    this->EBO.clear();
    this->EBO.resize(this->indices.size()); // Resize the EBO to the correct size.

    // 1: Assemble a vertex based on this->indices[i], Store the vertex data in the tempVert vector.
    // 2: Check if the vertex data is a duplicate of a vertex which already exists in the VBO. Just push the index of the existing vertex onto the EBO.
    // 3: If the vertex is a new vertex, push the vertex data onto the VBO and push the index of the new vertex onto the EBO.
    std::vector<std::vector<float>> tempVBO; // its easier to work with a vector of vectors, then turn it into a flat vector later on.
    std::vector<float> tempVert;
    tempVert.resize(this->vertSize);
    for(int i=0; i<this->indices.size(); i++){
        // 1: Assemble a vertex based on this->indices[i], Store the vertex data in the tempVert vector.
        tempVert[0] = this->positions[((this->indices[i].positionIndex-1)*3) + 0];
        tempVert[1] = this->positions[((this->indices[i].positionIndex-1)*3) + 1];
        tempVert[2] = this->positions[((this->indices[i].positionIndex-1)*3) + 2];
        tempVert[3] = this->normals[((this->indices[i].normalIndex-1)*3) + 0];
        tempVert[4] = this->normals[((this->indices[i].normalIndex-1)*3) + 1];
        tempVert[5] = this->normals[((this->indices[i].normalIndex-1)*3) + 2];
        tempVert[6] = this->textureCoords[((this->indices[i].textureIndex-1)*2) + 0];
        tempVert[7] = this->textureCoords[((this->indices[i].textureIndex-1)*2) + 1];
        // 2: Check if the vertex data is a duplicate of a vertex which already exists in the VBO. Just push the index of the existing vertex onto the EBO.
        bool isDuplicate = false;
        unsigned int duplicateIndex = 0;
        for(int j=0; j<tempVBO.size(); j++){
            if(tempVBO[j] == tempVert){
                this->EBO[i] = j;
                isDuplicate = true;
                duplicateIndex = j;
                break;
            }
        }
        // 3: If the vertex is a new vertex, push the vertex data onto the VBO and push the index of the new vertex onto the EBO.
        if(!isDuplicate){
            tempVBO.push_back(tempVert);
            this->EBO[i] = tempVBO.size()-1;
        }
        // 4: If the vertex is a duplicate, push the index of the existing vertex onto the EBO.
        else{
            this->EBO[i] = duplicateIndex;
        }
        
        // Progress bar using \r
        if(i%1000 == 0){
            std::cout << "\rBuilding VBO/EBO..." << std::fixed << std::setprecision(0) << (i / (float)this->indices.size()) * 100 << "%";
        }
    }
    std::cout << "\r Complete." << std::endl;

    // Assemble the VBO from the tempVBO vector.
    this->VBO.resize(tempVBO.size()*vertSize);
    for(int i=0; i<tempVBO.size(); i++){
        for(int j=0; j<vertSize; j++){
            this->VBO[i*vertSize + j] = tempVBO[i][j];
        }
    }
    std::cout << "VBO assembled." << std::endl;
}



objVBO::object::object(const std::string fileName){
    std::cout << "==========================================================" << std::endl;
    std::cout << "Reading obj file:" << fileName << std::endl;
    std::cout << "Reading positions..." << std::endl;
    readObjValues(fileName, "v ", 3, positions);
    std::cout << "Reading normals..." << std::endl;
    readObjValues(fileName, "vn", 3, normals);
    std::cout << "Reading texture coordinates..." << std::endl;
    readObjValues(fileName, "vt", 2, textureCoords);
    std::cout << "Reading indices..." << std::endl;
    readObjIndices(fileName, indices);
    std::cout << "Finished reading obj file." << std::endl;
    assembleVBO();
}


void objVBO::object::writeVBO(const std::string filename){
    std::cout << "==========================================================" << std::endl;
    std::cout << "Writing VBO to file:" << std::endl;
    std::ofstream file;
    file.open(filename + ".vbo");
    // Write the entire VBO onto the first line of the file
    for(int i = 0; i < this->VBO.size(); i++){
        file << this->VBO[i] << " ";
    }
    file.close();
    std::cout << "VBO written to file." << std::endl;
}



void objVBO::object::writeEBO(const std::string filename){
    std::cout << "==========================================================" << std::endl;
    std::cout << "Writing EBO to file:" << std::endl;
    std::ofstream file;
    file.open(filename + ".ebo");
    // Write the entire EBO onto the first line of the file
    for(int i = 0; i < this->EBO.size(); i++){
        file << this->EBO[i] << " ";
    }
    file.close();
    std::cout << "EBO written to file." << std::endl;
}