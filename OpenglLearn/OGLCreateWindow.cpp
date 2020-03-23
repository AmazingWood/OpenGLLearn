#include "OGLCreateWindow.h"

OGLCreateWindow::OGLCreateWindow(int windowWidth, int windowHeight, std::string windowName)
{
    //initializtion the openGl with glfw
    glfwInit();
    //setup the openGl Version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //use the core mod
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //use glfw to create the window which the width is 1366 and the height is 768 and the title is LearnOpenGL
    window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), NULL, NULL);

    //create failed and delete the src
    if (window == NULL)
    {
        throw std::logic_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);

    //glad to manage the point
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::logic_error("Failed to initialize GLAD");
    }

    glViewport(0, 0, windowWidth, windowHeight);

    //reg the callback to resize the window
    glfwSetFramebufferSizeCallback(window, OGLCallBack::framebuffer_size_callback);
}

void OGLCreateWindow::createWindowWithShader(std::vector<fs::path> vertexShaderPath, std::vector<fs::path> fragmentShaderPath)
{
    //create a triangle's point list
    std::array<float, 9> vertices = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.25f, 0.0f,
    };
    std::array<float, 9> secondTriangle = {
    0.0f, 0.25f, 0.0f,
    -0.5f, 1.0f, 0.0f,
    0.5f, 1.0f, 0.0f,
    };

    std::unique_ptr<unsigned int[]> VAOs = std::make_unique<unsigned int[]>(2);
    std::unique_ptr<unsigned int[]> VBOs = std::make_unique<unsigned int[]>(2);
    //Create a VAO and a VBO
    glGenVertexArrays(2, &VAOs[0]);
    glGenBuffers(2, &VBOs[0]);

    //Bind VAO and VBO
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

    //copy the points to the Buffer params desc: 1.buffer type; 2.data size; 3.the data; 4.manage method type;
    //The types of the manage method:
    //  1.GL_STATIC_DRAW : absolutely no change or just a little;
    //  2.GL_DYNAMIC_DRAW: a lot of change;
    //  3.GL_STREAM_DRAW : it will change each times when it draws to the window.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Bind VAO and VBO
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle.data(), GL_STATIC_DRAW);


    unsigned int vertexShaderID = shaderCompiler(shaderType::Vertex, vertexShaderPath);
    unsigned int fragmentShaderID = shaderCompiler(shaderType::Fragment, fragmentShaderPath);
    std::vector<fs::path> fragmentYellow;
    fragmentYellow.push_back("./FragmentShaderYellow.glsl");
    unsigned int fragmentYellowShaderID = shaderCompiler(shaderType::Fragment, fragmentYellow);

    unsigned int shaderProgramID = shaderProgramCreator(vertexShaderID, fragmentShaderID);
    unsigned int shaderProgramID2 = shaderProgramCreator(vertexShaderID, fragmentYellowShaderID);
    //tell how to desc the vetex to the device
    /*
        glVertexAttribPointer params instuctrion:
        1.pointer location
        2.the size of the data, this is a triangle, so it is 3
        3.data type, for now it is always the GL_FLOAT.
        4.is it normalize? no, GL_FALSE, if it is true it will be transfer the coordinate 0-1
        5.the stride, we need to read the each data with the correct size, we send the 3 float item, so we put it to 3*size of float,
          and if it is tighty packed, u can put it 0 to let opengl deside it.
        6.I really do not know what the fuck it is, the learnopengl-cn didn't told us yet.
    */
    //use the vetex info to activate the vetex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //loop the windows!
    while (!glfwWindowShouldClose(window))
    {
        glUseProgram(shaderProgramID);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //shaderProgram2
        glUseProgram(shaderProgramID2);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int OGLCreateWindow::shaderCompiler(shaderType typeName, std::vector<fs::path> shaderPath)
{
    //read the shader
    std::vector<std::string> ShaderCode = shaderReader(shaderPath);
    std::unique_ptr<const char* []> ShaderCodeCharPointerList = std::make_unique<const char* []>(ShaderCode.size());

    for (int i = 0; i < ShaderCode.size(); i++) {
        ShaderCodeCharPointerList[i] = ShaderCode[i].c_str();
    }

    auto ShaderCodeCPoint = ShaderCodeCharPointerList.get();

    //create a shader ID 
    unsigned int shaderID;
    if (typeName == shaderType::Vertex) {
        shaderID = glCreateShader(GL_VERTEX_SHADER);
        shaderIDList.push_back(shaderID);
    }
    else if (typeName == shaderType::Fragment) {
        shaderID = glCreateShader(GL_FRAGMENT_SHADER);
        shaderIDList.push_back(shaderID);
    }
    else {
        throw std::logic_error("Wrong Shader Type!");
    }

    //compile the shaderCode
    glShaderSource(shaderID, ShaderCode.size(), ShaderCodeCPoint, NULL);
    glCompileShader(shaderID);

    //check the compile status
    int checkCompileStatus;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &checkCompileStatus);
    if (checkCompileStatus != 1) {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::string errLog = infoLog;
        throw std::logic_error("ERROR::SHADER::COMPILATION_FAILED" + errLog);
    }
    return shaderID;
}

std::vector<std::string> OGLCreateWindow::shaderReader(std::vector<fs::path> shaderPath)
{
    std::vector<std::string> shaderCodeList;
    for (auto s : shaderPath) {
        std::string shaderCode;
        if (!fs::is_empty(s)) {
            std::ifstream shaderStream(s);
            std::string temp;
            while (std::getline(shaderStream, temp)) {
                shaderCode += temp;
                shaderCode += "\n";
            }
            shaderCodeList.push_back(shaderCode);
        }
        else {
            throw std::logic_error("Wrong shader directory!");
        }
    }
    return shaderCodeList;
}

int OGLCreateWindow::shaderProgramCreator(int vertexID, int fragmentID)
{
    //create a shader program
    unsigned int shaderProgram = glCreateProgram();
    shaderProgramList.push_back(shaderProgram);
    //link the shaderprogram, veterx and fragment shader to one.
    glAttachShader(shaderProgram, vertexID);
    glAttachShader(shaderProgram, fragmentID);
    glLinkProgram(shaderProgram);

    //check the link status
    int checLinkStatus;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &checLinkStatus);
    char infoLog[512];
    if (!checLinkStatus) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::string errLog = infoLog;
        throw std::logic_error("ERROR::SHADER::COMPILATION_FAILED" + errLog);
    }

    //activate the shader program
    glUseProgram(shaderProgram);

    //now the shader is useless delete them when it is useless.
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);

    return shaderProgram;
}

OGLCreateWindow::~OGLCreateWindow()
{
    for (auto v : shaderIDList) {
        glDeleteShader(v);
    }
    for (auto p : shaderProgramList) {
        glDeleteProgram(p);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();
}

void OGLCallBack::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}