#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <functional>

//assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "OGLShader.h"

/*
*   texture name rule
*   texture_diffuseN
*   texture_specularN
*   texture_normalN
*   texture_heightN
*/

struct VertexData {
    //each vertex should have a position¡¢normal and a texture
    glm::vec3 posVec;
    glm::vec3 normalVec;
    glm::vec2 textureVec;
};

struct TextureData {
    unsigned int textureId;
    std::string textureType;
    aiString path;
};

class IceCubeMesh {
private:
    //mesh data
    std::vector<VertexData> verticleList;
    std::vector<unsigned int> indexList;
    std::vector<TextureData> textureList;
    //pipeline
    unsigned int VAO, VBO, EBO;
    void setupMesh();
public:
    //funcs
    IceCubeMesh(std::vector<VertexData> verticleList, std::vector<unsigned int> indexList, std::vector<TextureData> textureList);
    void drawMesh(OGLShader shader, std::string programName);
    ~IceCubeMesh();
};