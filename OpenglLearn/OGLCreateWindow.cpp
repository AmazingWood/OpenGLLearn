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
    //load the texture code
    //set the y axis to other side 
    stbi_set_flip_vertically_on_load(true);
   
    unsigned int texture1,texture2;
    //gnerate and bind a texture value, nothing more

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);



    // ATTENTION:there is no such things like glTextureParameterX, only glTexParameter!
    // params:
    // 1.target: which type of texture would u setup?
    // there is GL_TEXTURE_1D,GL_TEXTURE_2D,GL_TEXTURE_3D and even more options.
    // 2.pname: now we know that we have a 2d texture so we shall use S,R axis just like X,Y axis,so we input the GL_TEXTURE_WRAP_S,GL_TEXTURE_WRAP_R
    // 3.params: setup how we deal with the texture, we want the texture repeat, so we input GL_REPEAT, and here's the other optional params:GL_REPEAT,GL_MIRRORED_REPEAT,GL_CLAMP_TO_EDGE and GL_CLAMP_TO_BORDER.

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // params instruction:
    // 2 now we chose the GL_TEXTURE_MIN_FILTER and GL_TEXTURE_MAG_FILTER, to make the opengl know how we deal with the texture when we magnify or minify it.
    // 3 we can chose two way to process the texture when we do those to commands£ºGL_NEAREST£¬like a bit photo£¬GL_LINEAR output the avg color to each pixel when we minify or magnify it.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    //read the pic through stb_image.h
    int textureWidth, textureHeight, textureNrChannels;

    std::unique_ptr<unsigned char, std::function<void(unsigned char*)>> data(stbi_load("./assets/container.jpg", &textureWidth, &textureHeight, &textureNrChannels, 0), [](unsigned char* del) {stbi_image_free(del); });
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data.get());
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        throw std::logic_error("Failed to load texture");
    }


    // texture 2
    // -----------------


    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // ATTENTION:there is no such things like glTextureParameterX, only glTexParameter!
    // params:
    // 1.target: which type of texture would u setup?
    // there is GL_TEXTURE_1D,GL_TEXTURE_2D,GL_TEXTURE_3D and even more options.
    // 2.pname: now we know that we have a 2d texture so we shall use S,R axis just like X,Y axis,so we input the GL_TEXTURE_WRAP_S,GL_TEXTURE_WRAP_R
    // 3.params: setup how we deal with the texture, we want the texture repeat, so we input GL_REPEAT, and here's the other optional params:GL_REPEAT,GL_MIRRORED_REPEAT,GL_CLAMP_TO_EDGE and GL_CLAMP_TO_BORDER.

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // params instruction:
    // 2 now we chose the GL_TEXTURE_MIN_FILTER and GL_TEXTURE_MAG_FILTER, to make the opengl know how we deal with the texture when we magnify or minify it.
    // 3 we can chose two way to process the texture when we do those to commands£ºGL_NEAREST£¬like a bit photo£¬GL_LINEAR output the avg color to each pixel when we minify or magnify it.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    //read the pic through stb_image.h
    int textureWidth2, textureHeight2, textureNrChannels2;
    std::unique_ptr<unsigned char, std::function<void(unsigned char*)>> data2(stbi_load("./assets/awesomeface.png", &textureWidth2, &textureHeight2, &textureNrChannels2, 0), [](unsigned char* del) {stbi_image_free(del); });
    if (data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth2, textureHeight2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2.get());
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        throw std::logic_error("Failed to load texture");
    }



    //create a triangle's point list
    std::array<float, 32> vertices = {
        //position             //color          //texture
         0.4f,  0.4f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         0.4f, -0.4f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        -0.4f, -0.4f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        -0.4f,  0.4f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f
    };

    std::array<unsigned int, 6> indices {
        //position
        0,1,2,
        0,2,3
    };
    unsigned int EBO;
    glGenBuffers(1, &EBO);

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    unsigned int programID = programListAlpha.activateProgram("yellowFragment");
    programListAlpha.setInt("yellowFragment", "texture1", 0);
    programListAlpha.setInt("yellowFragment", "texture2", 1);

    //loop the windows!
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        programListAlpha.setFloat("yellowFragment", "vertexOffset", 0.0f);
        
        //more than one texture will binded the box
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);


        programListAlpha.activateProgram("yellowFragment");
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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


