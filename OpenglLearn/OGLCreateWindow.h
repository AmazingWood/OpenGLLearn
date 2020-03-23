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

class OGLCreateWindow {
private:
	GLFWwindow* window;
    std::string shaderReader(fs::path shaderPath);
public:
	OGLCreateWindow(int width, int height, std::string windowName);
	void createWindow();
	~OGLCreateWindow();
};