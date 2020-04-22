#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
};

class Mesh {
private:
    //mesh data
    std::vector<Vertex> verticleList;
    std::vector<unsigned int> indexList;
    std::vector<Texture> textureList;

    unsigned int VAO, VBO, EBO;
    void setupMesh();
public:
    //function
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

};