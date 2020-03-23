#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <array>
#include <iostream>

namespace fs = std::filesystem;
namespace OGLCallBack {
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
}

enum shaderType {
	Vertex,
	Fragment
};

class OGLCreateWindow {
private:
	GLFWwindow* window;
	fs::path vertexShaderPath;
	fs::path fragmentShaderPath;
	unsigned int VAO, VBO;
	//return shaderCode list
	int shaderCompiler(shaderType typeName, fs::path vertexShaderPath);
	std::vector<std::string> shaderReader(std::vector<fs::path> shaderPath);
public:
	OGLCreateWindow(int width, int height, std::string windowName);
	void createWindowWithShader(std::vector<fs::path> vertexShaderPath, std::vector<fs::path> fragmentShaderPath);
	~OGLCreateWindow();
};