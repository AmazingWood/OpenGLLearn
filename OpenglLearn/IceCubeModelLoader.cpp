#include "IceCubeModelLoader.h"

IceCubeModelLoader::IceCubeModelLoader(fs::path modelPath)
{
	loadModel(modelPath);
}

void IceCubeModelLoader::drawMeshes(OGLShader shader, std::string programName)
{
	for (IceCubeMesh mesh : meshList) {
		mesh.drawMesh(shader, programName);
	}
}

void IceCubeModelLoader::loadModel(fs::path modelPath)
{
	Assimp::Importer importer;
	sceneNode = importer.ReadFile(modelPath.string(), aiProcess_Triangulate | aiProcess_FlipUVs);
	
	if (!sceneNode || sceneNode->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !sceneNode->mRootNode)
	{
		std::string errorInfo = "ERROR::ASSIMP::";
		errorInfo = errorInfo + importer.GetErrorString();
		throw std::logic_error(errorInfo);
	}

	modelDirectory = modelPath.parent_path();
	processNode(sceneNode->mRootNode, sceneNode);
}

void IceCubeModelLoader::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshList.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

IceCubeMesh IceCubeModelLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<VertexData> vertexList;
	std::vector<unsigned int> indexList;
	std::vector<TextureData> textureList;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		VertexData vertex;
		// process cood data
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.posVec = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normalVec = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.textureVec = vec;
		}
		else
		{
			vertex.textureVec = glm::vec2(0.0f, 0.0f);
		}

		vertexList.push_back(vertex);
	}
	// process index
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indexList.push_back(face.mIndices[j]);
	}
	// process texture
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<TextureData> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textureList.insert(textureList.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<TextureData> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textureList.insert(textureList.end(), specularMaps.begin(), specularMaps.end());

		//normal texture and height texture
		std::vector<TextureData> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textureList.insert(textureList.end(), normalMaps.begin(), normalMaps.end());
		std::vector<TextureData> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textureList.insert(textureList.end(), heightMaps.begin(), heightMaps.end());
	}

	return IceCubeMesh(vertexList, indexList, textureList);
}

std::vector<TextureData> IceCubeModelLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<TextureData> textureList;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textureListLoaded.size(); j++)
		{
			if (std::strcmp(textureListLoaded[j].path.data, str.C_Str()) == 0)
			{
				textureList.push_back(textureListLoaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			TextureData texture;
			texture.textureId = textureLoader(str.C_Str(), modelDirectory);
			texture.textureType = typeName;
			texture.path = str;
			textureList.push_back(texture);
			textureListLoaded.push_back(texture);
		}
	}
	return textureList;
}

unsigned int IceCubeModelLoader::textureLoader(const fs::path& path, const fs::path& directory, bool gamma)
{
	fs::path filePath = directory;
	filePath /= path;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	std::unique_ptr<unsigned char, std::function<void(unsigned char*)>> data(stbi_load(filePath.string().c_str(), &width, &height, &nrComponents, 0), [](unsigned char* del) {stbi_image_free(del); });
	if (data)
	{
		GLenum format;
		if (nrComponents == 1) {
			format = GL_RED;
		}
		else if (nrComponents == 3) {
			format = GL_RGB;
		}
		else if (nrComponents == 4) {
			format = GL_RGBA;
		}
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data.get());
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		throw std::logic_error("Texture failed to load at path: " + path.string());
	}
	return textureID;
}