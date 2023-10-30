#pragma once

#include <string>
#include <unordered_map>


struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_Filepath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
	
	unsigned int getUniformLocation(const std::string& name);

	ShaderProgramSource parseShader(const std::string& filepath);
	unsigned int compileShader(unsigned int type, const std::string & source);
	unsigned int createShader(const std::string & vertexShader, const std::string & fragmentShader);

public:
	Shader(const std::string & filepath);
	~Shader();

	void bind() const;
	void unbind() const;

	//set uniforms
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
};

