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
    std::array<float, 18> vertices = {
        //position             //color
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
    };

    //Create a VAO and a VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //Bind VAO and VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //copy the points to the Buffer params desc: 1.buffer type; 2.data size; 3.the data; 4.manage method type;
    //The types of the manage method:
    //  1.GL_STATIC_DRAW : absolutely no change or just a little;
    //  2.GL_DYNAMIC_DRAW: a lot of change;
    //  3.GL_STREAM_DRAW : it will change each times when it draws to the window.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
    
    OGLShader programListAlpha(vertexShaderPath, fragmentShaderPath);
    
    programListAlpha.addProgram("./SimpleVertexShader.glsl","FragmentShaderYellow.glsl","yellowFragment");

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //loop the windows!
    while (!glfwWindowShouldClose(window))
    {

        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);


        


        //float timeValue = glfwGetTime();
        //float GValue = (sin(timeValue) ) + 0.33f;
        //float RValue = (cos(timeValue) ) + 0.66f;
        //float BValue = (cos(timeValue) ) + 0.99f;
        //int vertexColorLocation = glGetUniformLocation(shaderProgramID, "ourColor");
        //glUniform4f(vertexColorLocation, RValue, GValue, BValue, 1.0f);
        
        programListAlpha.activateProgram("yellowFragment");

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

OGLCreateWindow::~OGLCreateWindow()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();
}

void OGLCallBack::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}