#pragma once
#include <vector>
#include <filesystem>
#include <string>
#include <memory>

//assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"
#include "IceCubeMesh.h"

namespace fs = std::filesystem;

class IceCubeModelLoader {
private:
	//model data
	std::vector<TextureData> textureListLoaded;
	std::vector<IceCubeMesh> meshList;
	fs::path modelDirectory;
	const aiScene* sceneNode;
	//funcs
	void loadModel(fs::path modelPath);
	void processNode(aiNode *node, const aiScene *scene);
	IceCubeMesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<TextureData> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
public:
	IceCubeModelLoader(fs::path modelPath);
	void drawMeshes(OGLShader shader, std::string programName);
	unsigned int textureLoader(const fs::path &path, const fs::path &directory, bool gamma = false);
};