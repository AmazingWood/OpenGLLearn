#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <filesystem>
#include <array>
#include <iostream>
#include "OGLShader.h"

namespace fs = std::filesystem;
namespace OGLCallBack {
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
}


class OGLCreateWindow {
private:
	GLFWwindow* window;
	fs::path vertexShaderPath;
	fs::path fragmentShaderPath;
	unsigned int VAO = 0, VBO = 0;
	std::vector<unsigned int> shaderIDList;
	std::vector<unsigned int> shaderProgramList;
public:
	OGLCreateWindow(int width, int height, std::string windowName);
	void createWindowWithShader(std::vector<fs::path> vertexShaderPath, std::vector<fs::path> fragmentShaderPath);
	~OGLCreateWindow();
};