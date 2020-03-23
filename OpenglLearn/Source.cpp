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
    newWindow.createWindow();
    return 0;
}