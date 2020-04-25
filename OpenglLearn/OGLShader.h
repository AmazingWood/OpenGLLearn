#pragma once
#include <glad/glad.h>

#include <vector>
#include <string>
#include <filesystem>
#include <map>

#include <fstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace fs = std::filesystem;

enum class shaderType {
	Vertex,
	Fragment
};

class OGLShader {

public:
	OGLShader(const std::vector<fs::path>& vertexPath, std::vector<fs::path>& fragmentPath, std::string programName = "default");
	OGLShader(const fs::path& vertexPath, const fs::path& fragmentPath,std::string programName = "default");
	OGLShader(const fs::path& vertexPath, std::vector<fs::path>& fragmentPath, std::string programName = "default");
	OGLShader(const std::vector<fs::path>& vertexPath, const fs::path& fragmentPath, std::string programName = "default");

	void addProgram(const std::vector<fs::path>& vertexPath, std::vector<fs::path>& fragmentPath, std::string programName);
	void addProgram(const fs::path& vertexPath, const fs::path& fragmentPath, std::string programName);
	void addProgram(const fs::path& vertexPath, std::vector<fs::path>& fragmentPath, std::string programName);
	void addProgram(const std::vector<fs::path>& vertexPath, const fs::path& fragmentPath, std::string programName);

	//activate program
	unsigned int activateProgram(const std::string &programName = "default");
	unsigned int getProgramID(const std::string &programName) const;	//should be a const. but I cannot

	//uniform tools
	void setBool(const std::string& programName, const std::string& name, bool value) const;
	void setInt(const std::string& programName, const std::string& name, int value) const;
	void setFloat(const std::string& programName, const std::string& name, float value) const;
	void setMat4(const std::string& programName, const std::string& name, glm::mat4 value) const;

	~OGLShader();
private:
	std::map<std::string, unsigned int> shaderProgramList;
	std::vector<unsigned int> shaderIDList;
	std::string shaderProgramTitle;
	std::vector<std::string> shaderReader(const std::vector<fs::path>& shaderPath);
	int shaderCompiler(const shaderType& typeName, const std::vector<fs::path>& shaderPath);
	int shaderProgramCreator(int vertexID, int fragmentID, std::string programName);

};