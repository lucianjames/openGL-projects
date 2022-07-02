#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <charconv>

namespace objVBO{

    struct vertexIndices{
        int vertexIndex;
        int normalIndex;
        int textureIndex;
    };

    template<typename T>
    void readObjValues(const std::string fileName, const std::string startTag, const int dataCount, std::vector<T>& data);
    void readObjIndices(const std::string fileName, std::vector<vertexIndices>& data);

    class object{
    private:
        template<typename T>
        void printVector(const std::vector<T>& data);

    public:
        std::vector<float> positions;
        std::vector<float> normals;
        std::vector<float> textureCoords;
        std::vector<vertexIndices> indices; // Index data
        std::vector<float> VBO; // The actual VBO data derived from the above data.
        std::vector<int> VBO_indices; // The indices of the VBO data derived from the above data.
        object(const std::string fileName);
        void debugPrintData();
        void assembleVBO();
    };
}
