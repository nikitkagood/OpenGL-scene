// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include"Shader.h"
#include"Renderer.h"

Shader::Shader(const std::string& filepath) : m_FilePath(filepath), m_ShaderID(0) //2 shaders in one file: vertex and fragment
{
    ShaderProgamSource source = ParseTwoShaders(filepath);
    m_ShaderID = CreateProgram(source.VertexSource, source.FragmentSource);
}

//Shader::Shader(const std::string& filepath, std::string type) : m_FilePath(filepath), m_ShaderID(0)
//{
//    ShaderProgamSource source = Parse_1_shader(filepath);
//    m_ShaderID = CreateShader(source.VertexSource, source.FragmentSource);
//    std::cout << "Current shader: " << m_FilePath << std::endl;
//}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_ShaderID));
}

ShaderProgamSource Shader::ParseTwoShaders(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    //std::stringstream ss[2]; //0 for vertex shader, 1 for fragment shader
    std::vector<std::stringstream> ss;
    ss.push_back(std::stringstream{});
    ss.push_back(std::stringstream{});

    ShaderType type = ShaderType::NONE;
    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else if (line[0] == '/' && line[1] == '/')
        {
            //nothing, it's a comment and we skip it
            continue;
        }
        else if (line.empty() && type == ShaderType::NONE)
        {
            //nothing, we dont have shader type yet and we skip it
            continue;
        }
        else //copying shader source code from a file
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

//ShaderProgamSource Shader::ParseOneshader(const std::string& filepath)
//{
//    std::ifstream stream(filepath);
//
//    enum class ShaderType
//    {
//        NONE = -1, VERTEX = 0, FRAGMENT = 1
//    };
//
//    std::string line;
//    std::stringstream ss;
//
//    ShaderType type = ShaderType::NONE;
//    while (std::getline(stream, line))
//    {
//        if (line.find("#shader") != std::string::npos)
//        {
//            if (line.find("vertex") != std::string::npos)
//            {
//                type = ShaderType::VERTEX;
//            }
//            else if (line.find("fragment") != std::string::npos)
//            {
//                type = ShaderType::FRAGMENT;
//            }
//        }
//        else if (line[0] == '/' && line[1] == '/')
//        {
//            //nothing, it's a comment and we skip it
//            continue;
//        }
//        else if (line.empty() && type == ShaderType::NONE)
//        {
//            //nothing, we dont have shader type yet and we skip it
//            continue;
//        }
//        else //copying shader source code from a file
//        {
//            ss << line << '\n';
//        }
//    }
//
//    if (type == ShaderType::VERTEX)
//    {
//        return { ss.str(), nullptr };
//    }
//    if (type == ShaderType::FRAGMENT)
//    {
//        return { nullptr, ss.str() };
//    }
//}

unsigned Shader::CompileShader(unsigned type, const std::string& source)
{
    unsigned id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(id, 1024, &log_length, message);

        std::cerr << "Failed to compile a shader " << "type: " << type << std::endl;
        std::cerr << message << std::endl;

        glDeleteShader(id);

        return 0;
    }

    return id;
}

GLuint Shader::CreateProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLuint program = glCreateProgram();

    GLuint vs = 0;
    if (!vertexShader.empty())
    {
        vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
        glAttachShader(program, vs);
    }
    GLuint fs = 0;
    if (!fragmentShader.empty())
    {
        fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
        glAttachShader(program, fs);
    }

    glLinkProgram(program);
    glValidateProgram(program);

    //glDetachShader(); techinaclly it must be called to delete src code from GPU mem
    //but it's not worth it due to debugging reasons

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_ShaderID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

unsigned Shader::GetID() const
{
    return m_ShaderID;
}

unsigned Shader::GetUniformLocation(const std::string& name) //checks uniform location in the cache
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }
    else
    {
        GLCall(int location = glGetUniformLocation(m_ShaderID, name.c_str()));
        if (location == -1)
        {
            std::cerr << "Warning: could not find uniform: " << name << std::endl;
            std::cerr << "Compiler could cut it off if it's defined but not used" << std::endl;
        }
        else
        m_UniformLocationCache[name] = location;

        return location;
    }
}


void Shader::SetUniform1f(const std::string& uniform_name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(uniform_name), value));
}

void Shader::SetUniform3f(const std::string& uniform_name, float v0, float v1, float v2)
{
    GLCall(glUniform3f(GetUniformLocation(uniform_name), v0, v1, v2));
}


void Shader::SetUniform4f(const std::string& uniform_name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(uniform_name), v0, v1, v2, v3));
}

void Shader::SetUniformMatrix4fv(const std::string& uniform_name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(uniform_name), 1, GL_FALSE, glm::value_ptr(matrix)));
}

