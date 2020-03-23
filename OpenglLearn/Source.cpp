#include <iostream>
#include "OGLCreateWindow.h"

#include <string>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <array>

namespace fs = std::filesystem;

int main() {
    OGLCreateWindow newWindow(1280, 720, "Hello World");

    std::vector<fs::path> VertexShaderList;
    std::vector<fs::path> FragmentShaderList;
    VertexShaderList.push_back("./SimpleVertexShader.glsl");
    VertexShaderList.push_back("./EmptyVertexShader.glsl");
    FragmentShaderList.push_back("./SimpleFragmentShader.glsl");
    newWindow.createWindowWithShader(VertexShaderList, FragmentShaderList);
    return 0;
}