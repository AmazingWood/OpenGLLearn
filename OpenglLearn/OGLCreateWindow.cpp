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

void OGLCreateWindow::createWindow()
{
    //create a triangle's point list
    std::array<float, 9> vertices = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };

    // float squares[]
    std::array<float, 12> squares = {
    0.5f, 0.5f, 0.0f,   // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f   // 左上角
    };

    unsigned int indices[] = { // 注意索引从0开始! 
    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
    };
    //Create a fuckin VAO, how fun it is, mother fucker
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);


    //Create a buffer called VBO and give it a ID , ID is 1
    unsigned int VBO;
    glGenBuffers(1, &VBO);



    //this is a EBO LOL
    //unsigned int EBO;
    //glGenBuffers(1, &EBO);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);


    //bind the buffer type to the ID
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //copy the points to the Buffer params desc: 1.buffer type; 2.data size; 3.the data; 4.manage method type;
    //The types of the manage method:
    //  1.GL_STATIC_DRAW : absolutely no change or just a little;
    //  2.GL_DYNAMIC_DRAW: a lot of change;
    //  3.GL_STREAM_DRAW : it will change each times when it draws to the window.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);




    //read the vertexshader
    fs::path vertexShaderPath = "./SimpleVertexShader.glsl";

    std::string shaderCode = shaderReader(vertexShaderPath).data();
    const char* vertexShaderCodeChar = shaderCode.c_str();
    //create a vertexshader ID 
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //compile the shaderCode
    glShaderSource(vertexShader, 1, &vertexShaderCodeChar, NULL);
    glCompileShader(vertexShader);

    //check the compile status
    int checkCompileStatus;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &checkCompileStatus);
    if (checkCompileStatus != 1) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //read the fragmentshader
    fs::path fragmentShaderPath = "./SimpleFragmentShader.glsl";
    std::string fragmentShaderCode = shaderReader(fragmentShaderPath);
    const char* fragmentShaderCodeChar = fragmentShaderCode.c_str();
    //create a fragmentShader ID 
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    //compile the shaderCode
    glShaderSource(fragmentShader, 1, &fragmentShaderCodeChar, NULL);
    glCompileShader(fragmentShader);

    //create a shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    //link the shaderprogram, veterx and fragment shader to one.
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //check the link status
    int checLinkStatus;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &checLinkStatus);
    if (!checLinkStatus) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::LINK_FAILED\n" << infoLog << std::endl;
    }

    //activate the shader program
    glUseProgram(shaderProgram);

    //now the shader is useless delete them when it is useless.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

OGLCreateWindow::~OGLCreateWindow()
{
    glfwTerminate();
    delete window;
}

void OGLCallBack::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

std::string OGLCreateWindow::shaderReader(fs::path shaderPath)
{
    std::string shaderCode;
    if (!fs::is_empty(shaderPath)) {
        std::ifstream shaderStream(shaderPath);
        std::string temp;
        while (std::getline(shaderStream, temp)) {
            shaderCode += temp;
            shaderCode += "\n";
        }
    }
    else {
        throw std::logic_error("Wrong shader directory!");
    }
    return shaderCode;
}
