#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <charconv>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <utility>

namespace objVBO{

    struct vertexIndices{
        int positionIndex;
        int normalIndex;
        int textureIndex;
    };

    template<typename T> void readObjValues(const std::string fileName, const std::string startTag, const int dataCount, std::vector<T>& data);
    void readObjIndices(const std::string fileName, std::vector<vertexIndices>& data);

    class object{
    private:
        template<typename T> void printVector(const std::vector<T>& data);
        int vertSize;
        void assembleVBO();

    public:
        std::vector<float> positions;
        std::vector<float> normals;
        std::vector<float> textureCoords;
        std::vector<vertexIndices> indices; // Index data
        std::vector<float> VBO; // The actual VBO data derived from the above data.
        std::vector<int> EBO; // The indices of the VBO data derived from the above data.
        object(const std::string fileName);
        void debugPrintData();
        void optimiseVBO();
        void writeVBO(const std::string fileName);
        void writeEBO(const std::string fileName);
    };
}
