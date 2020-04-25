#pragma once
//opengl api and the windows lib
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//stl lib
#include <string>
#include <filesystem>
#include <array>
#include <iostream>
#include <functional>
//matrix stuff
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OGLShader.h"

#include "IceCubeModelLoader.h"

namespace fs = std::filesystem;

class IceCubeWindow {
private:
	GLFWwindow* window;
public:
	IceCubeWindow(int windowWidth, int windowHeight, std::string windowName);
	void IceCubeDisplay();
	~IceCubeWindow();

};