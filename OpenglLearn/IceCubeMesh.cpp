#include "IceCubeMesh.h"

IceCubeMesh::IceCubeMesh(std::vector<VertexData> verticleList, std::vector<unsigned int> indexList, std::vector<TextureData> textureList)
{
	this->verticleList = verticleList;
	this->indexList = indexList;
	this->textureList = textureList;

	setupMesh();
}

void IceCubeMesh::setupMesh()
{
	//gernate VAO VBO and EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	/*
	*
	* try verticleList.data() instead of verticleList[0]
	*
	*/

	//setup verticle data
	glBufferData(GL_ARRAY_BUFFER, verticleList.size() * sizeof(VertexData), &verticleList[0], GL_STATIC_DRAW);
	//setup EBO data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexList.size() * sizeof(unsigned int), &indexList[0], GL_STATIC_DRAW);

	//setup data mode
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normalVec));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, textureVec));
	glBindVertexArray(0);
}
//u have to fix this
void IceCubeMesh::drawMesh(OGLShader shader, std::string programName)
{
	unsigned int diffuseNr = 1, specularNr = 1, normalNr = 1, heightNr = 1;
	for (unsigned int i = 0; i < textureList.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); 
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textureList[i].textureType;
		if (name == "texture_diffuse") {
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular") {
			number = std::to_string(specularNr++);
		}
		else if (name == "texture_normal") {
			number = std::to_string(normalNr++);
		}
		else if (name == "texture_height") {
			number = std::to_string(heightNr++);
		}
		shader.setInt(programName, "material." + name + number, i);
		glBindTexture(GL_TEXTURE_2D, textureList[i].textureId);
	}

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexList.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

IceCubeMesh::~IceCubeMesh()
{
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
}
