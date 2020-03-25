#pragma once
#include <glad/glad.h>

#include <vector>
#include <string>
#include <filesystem>
#include <map>

#include <fstream>
#include <iostream>

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
	void activateProgram(std::string progtamName = "default");

	// uniform tools
	//void setBool(const std::string& name, bool value) const;
	//void setInt(const std::string& name, int value) const;
	//void setFloat(const std::string& name, float value) const;
	~OGLShader();
private:
	std::map<std::string, unsigned int> shaderProgramList;
	std::vector<unsigned int> shaderIDList;
	std::string shaderProgramTitle;
	std::vector<std::string> shaderReader(const std::vector<fs::path>& shaderPath);
	int shaderCompiler(const shaderType& typeName, const std::vector<fs::path>& shaderPath);
	int shaderProgramCreator(int vertexID, int fragmentID, std::string programName);

};