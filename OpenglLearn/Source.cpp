#include <iostream>
#include "OGLCreateWindow.h"
#include "IceCubeWindow.h"

#include <string>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <array>

namespace fs = std::filesystem;

int _1stChapter() {
    OGLCreateWindow newWindow(800, 600, "Hello World");
    std::vector<fs::path> VertexShaderList;
    std::vector<fs::path> FragmentShaderList;
    VertexShaderList.push_back("./SimpleVertexShader.glsl");
    FragmentShaderList.push_back("./SimpleFragmentShader.glsl");
    newWindow.createWindowWithShader(VertexShaderList, FragmentShaderList);
    return 0;
}

/*
    fs::path a;
    fs::path b = "a/b/c/b.jpg";
    a = b.parent_path();
    std::cout << a << std::endl;
    std::string c = "a/b/c/b.jpg";
    std::string d;
    d = c.substr(0, c.find_last_of('/'));
    std::cout << d;
*/

int main() {
    IceCubeWindow windowCreator(800, 600, "Test World");
    windowCreator.IceCubeDisplay();

    return 0;
}