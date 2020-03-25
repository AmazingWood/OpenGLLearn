#include "OGLShader.h"

OGLShader::OGLShader(const fs::path& vertexPath, const fs::path& fragmentPath, std::string programName)
{
    addProgram(vertexPath, fragmentPath, programName);
}

OGLShader::OGLShader(const std::vector<fs::path>& vertexPath, std::vector<fs::path>& fragmentPath, std::string programName)
{
    addProgram(vertexPath, fragmentPath, programName);
}

void OGLShader::addProgram(const fs::path& vertexPath, const fs::path& fragmentPath, std::string programName)
{
    auto vertexPathList{ std::vector<fs::path>() };
    vertexPathList.push_back(vertexPath);
    auto fragmentPathList{ std::vector<fs::path>() };
    fragmentPathList.push_back(fragmentPath);

    addProgram(vertexPathList, fragmentPathList, programName);
}

void OGLShader::addProgram(const std::vector<fs::path>& vertexPath, std::vector<fs::path>& fragmentPath, std::string programName)
{
    //read the shader
    unsigned int vertexShaderID = shaderCompiler(shaderType::Vertex, vertexPath);
    unsigned int fragmentShaderID = shaderCompiler(shaderType::Fragment, fragmentPath);
    shaderProgramCreator(vertexShaderID, fragmentShaderID, programName);
}


void OGLShader::activateProgram(std::string progtamName)
{
    glUseProgram(shaderProgramList[progtamName]);
}


OGLShader::~OGLShader()
{
    for (auto v : shaderIDList) {
        glDeleteShader(v);
    }
    for (auto p : shaderProgramList) {
        glDeleteProgram(std::get<1>(p));
    }
}

std::vector<std::string> OGLShader::shaderReader(const std::vector<fs::path> &shaderPath)
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

int OGLShader::shaderCompiler(const shaderType &typeName, const std::vector<fs::path> &shaderPath)
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

int OGLShader::shaderProgramCreator(int vertexID, int fragmentID, std::string programName)
{
    //create a shader program
    unsigned int shaderProgram = glCreateProgram();
    shaderProgramList.insert({ programName,shaderProgram });
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
