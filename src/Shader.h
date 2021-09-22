#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<vector>
#include<unordered_map>

struct ShaderProgamSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned m_ShaderID;

	//ShaderProgamSource ParseOneshader(const std::string& filepath);
	ShaderProgamSource ParseTwoShaders(const std::string& filepath); //2 shaders in one file: vertex and fragment; must be marked with #shader vertex/fragment
	unsigned CompileShader(unsigned type, const std::string& source);
	unsigned CreateProgram(const std::string& vertexShader, const std::string& fragmentShader);

	unsigned GetUniformLocation(const std::string& name); //checks uniform location in the cache
	unsigned GetUniformLocation(const std::string&& name); //checks uniform location in the cache
	std::unordered_map<std::string, int> m_UniformLocationCache;
	
public:
	Shader(const std::string& filepath); //2 shaders in one file: vertex and fragment
	//Shader(const std::string& filepath, std::string type);
	~Shader();
	
	void Bind() const;
	void Unbind() const;
	
	unsigned GetID() const;

	void SetUniform1f(const std::string& uniform_name, float value);
	void SetUniform3f(const std::string& uniform_name, float v0, float v1, float v2);
	void SetUniform4f(const std::string& uniform_name, float v0, float v1, float v2, float v3);
	void SetUniformMatrix4fv(const std::string& uniform_name, const glm::mat4& matrix);

	//string argument move semantics
	//floats
	void SetUniform1f(const std::string&& uniform_name, float value);
	void SetUniform3f(const std::string&& uniform_name, float v0, float v1, float v2);
	void SetUniform4f(const std::string&& uniform_name, float v0, float v1, float v2, float v3);
	void SetUniformMatrix4fv(const std::string&& uniform_name, const glm::mat4& matrix);
	//ints
	void SetUniform1i(const std::string&& uniform_name, int value);

	
};
