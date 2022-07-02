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



objVBO::object::object(const std::string fileName){
    std::cout << "==========================================================" << std::endl;
    std::cout << "Reading obj file: " << fileName << std::endl;
    std::cout << "Reading positions..." << std::endl;
    readObjValues(fileName, "v ", 3, positions);
    std::cout << "Reading normals..." << std::endl;
    readObjValues(fileName, "vn", 3, normals);
    std::cout << "Reading texture coordinates..." << std::endl;
    readObjValues(fileName, "vt", 2, textureCoords);
    std::cout << "Reading indices..." << std::endl;
    readObjIndices(fileName, indices);
    std::cout << "Finished reading obj file." << std::endl;
}



void objVBO::object::debugPrintData(){
    std::cout << "==========================================================" << std::endl;
    std::cout << "Printing debugging data..." << std::endl;
    // Print position values
    std::cout << "Positions: " << std::endl;
    printVector(positions);
    // Print normal values
    std::cout << "Normals: " << std::endl;
    printVector(normals);
    // Print texture coordinate values
    std::cout << "Texture Coords: " << std::endl;
    printVector(textureCoords);
    // Print vertex indices
    std::cout << "Vertex Indices: " << std::endl;
    for(int i = 0; i < indices.size(); i++){
        std::cout << indices[i].positionIndex << ", ";
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



void objVBO::object::assembleVBO(){
    std::cout << "==========================================================" << std::endl;
    std::cout << "Assembling VBO..." << std::endl;
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
    this->VBO.resize(vertSize * this->indices.size()); // Resize the VBO to the correct size.
    this->EBO.clear();
    this->EBO.resize(this->indices.size()); // Resize the EBO to the correct size.
    std::cout << "Creating VBO..." << std::endl;
    for(int i=0; i < this->VBO.size() / vertSize; i++){ // !!!!!! HARD CODED !!!!!!
        // Read position data from the indices.
        this->VBO[(i * vertSize) + 0] = positions[((this->indices[i].positionIndex-1) * 3) + 0]; // The -1 is because the obj file indices start at 1, not 0. (Which is fucking retarded)
        this->VBO[(i * vertSize) + 1] = positions[((this->indices[i].positionIndex-1) * 3) + 1];
        this->VBO[(i * vertSize) + 2] = positions[((this->indices[i].positionIndex-1) * 3) + 2];
        // Read normal data
        this->VBO[(i * vertSize) + 3] = normals[((this->indices[i].normalIndex-1) * 3) + 0];
        this->VBO[(i * vertSize) + 4] = normals[((this->indices[i].normalIndex-1) * 3) + 1];
        this->VBO[(i * vertSize) + 5] = normals[((this->indices[i].normalIndex-1) * 3) + 2];
        // Read texCoord data
        this->VBO[(i * vertSize) + 6] = textureCoords[((this->indices[i].textureIndex-1) * 2) + 0];
        this->VBO[(i * vertSize) + 7] = textureCoords[((this->indices[i].textureIndex-1) * 2) + 1];
    }
    // Create the basic indices (just count up lol)
    std::cout << "Creating EBO..." << std::endl;
    for(int i = 0; i < this->VBO.size() / vertSize; i++){
        this->EBO[i] = i;
    }
    std::cout << "VBO+EBO Assembled." << std::endl;
}



template <typename T>
auto sort_permutation(T cbegin, T cend) {
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



void objVBO::object::optimiseVBO(){
    // First break the data into a vector of vectors with size this->vertSize.
    std::cout << "==========================================================" << std::endl;
    std::cout << "Optimising VBO..." << std::endl;
    std::vector<std::vector<float>> tempVertData;
    tempVertData.resize(this->VBO.size() / this->vertSize);
    for(int i = 0; i < tempVertData.size(); i++){
        tempVertData[i].resize(this->vertSize);
    }
    // Fill the new vector with the data.
    for(int i = 0; i < this->VBO.size(); i++){
        tempVertData[i / this->vertSize][i % this->vertSize] = this->VBO[i];
    }

    // Get the indices of the vertices which are duplicate:
    std::cout << "Searching for duplicate vertices inside the VBO..." << std::endl;
    // Create a sorted copy of tempVertData.
    std::vector<std::vector<float>> sortedTempVertData;
    sortedTempVertData.resize(tempVertData.size());
    for(int i = 0; i < tempVertData.size(); i++){
        sortedTempVertData[i].resize(tempVertData[i].size());
    }
    for(int i = 0; i < tempVertData.size(); i++){
        for(int j = 0; j < tempVertData[i].size(); j++){
            sortedTempVertData[i][j] = tempVertData[i][j];
        }
    }
    // Sort it:
    auto sortPerm = sort_permutation(sortedTempVertData.cbegin(), sortedTempVertData.cend()); // stores an index which lets us track down the original position of the vertex before sorting.
    std::sort(sortedTempVertData.begin(), sortedTempVertData.end()); // Sort the data.

    // Check for duplicates:
    unsigned int duplicates = 0;
    std::vector<unsigned int> unusedVerticeIndexes;
    for(int i = 0; i < sortedTempVertData.size(); i++){
        if(sortedTempVertData[i] == sortedTempVertData[i+1]){
            this->EBO[sortPerm[i]] = this->EBO[sortPerm[i+1]];
            unusedVerticeIndexes.push_back(sortPerm[i]);
            duplicates++;
        }
    }
    
    std::sort(unusedVerticeIndexes.begin(), unusedVerticeIndexes.end(), std::greater<unsigned int>()); // Sort the list of unused vertices in descending order.
    // Remove the duplicates from the unused vertices list by checking if the index of the current unused vertex is the same as the next unused vertex.
    for(int i = 0; i < unusedVerticeIndexes.size(); i++){
        if(unusedVerticeIndexes[i] == unusedVerticeIndexes[i + 1]){
            unusedVerticeIndexes.erase(unusedVerticeIndexes.begin() + i);
            i--;
        }
    }
    std::cout << "Found " << unusedVerticeIndexes.size() << " unused vertices." << std::endl;
    // DEBUG: print the unused vertices.
    for(int i = 0; i < unusedVerticeIndexes.size(); i++){
        std::cout << unusedVerticeIndexes[i] << " ";
    }
    std::cout << std::endl;

    // Remove the unused vertices from tempVertData.
    for(int i = 0; i < unusedVerticeIndexes.size(); i++){
        tempVertData.erase(tempVertData.begin() + unusedVerticeIndexes[i]);
        // Shift the indices of the EBO vector.
        for(int j = unusedVerticeIndexes[i]; j < this->EBO.size(); j++){
            this->EBO[j]--;
        }
        // Percentile progress bar - round to the nearest integer.
        if(i % 10 == 0){
            std::cout << "\rRemoving unused vertices from the VBO... " << std::fixed << std::setprecision(0) << (i / (float)unusedVerticeIndexes.size()) * 100 << "%";
        }
    }
    std::cout << "\rRemoving unused vertices from the VBO... 100%" << std::endl;
    std::cout << "Removed " << unusedVerticeIndexes.size() << " unused vertices from the temporary VBO" << std::endl;
    std::cout << "Moving std::vector<std::vector<float>> tempVertData into std::vector<float> this->VBO" << std::endl;

    // Assemble new VBO from the new vector of vectors.
    unsigned int oldVboSize = this->VBO.size();
    this->VBO.resize(tempVertData.size() * this->vertSize);
    for(int i = 0; i < tempVertData.size(); i++){
        for(int j = 0; j < this->vertSize; j++){
            this->VBO[i * this->vertSize + j] = tempVertData[i][j];
        }
    }
    std::cout << "Old VBO.size(): " << oldVboSize << std::endl;
    std::cout << "New VBO.size(): " << this->VBO.size() << std::endl;
    std::cout << "Optimisation complete." << std::endl;
}



void objVBO::object::writeVBO(const std::string filename){
    std::cout << "==========================================================" << std::endl;
    std::cout << "Writing VBO to file..." << std::endl;
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
    std::cout << "Writing EBO to file..." << std::endl;
    std::ofstream file;
    file.open(filename + ".ebo");
    // Write the entire EBO onto the first line of the file
    for(int i = 0; i < this->EBO.size(); i++){
        file << this->EBO[i] << " ";
    }
    file.close();
    std::cout << "EBO written to file." << std::endl;
}