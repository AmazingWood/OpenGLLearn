#include "ModelReader.h"

void Mesh::setupMesh()
{
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	verticleList = vertices;
	indexList = indices;
	textureList = textures;

	setupMesh();
}
