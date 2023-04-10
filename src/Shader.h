#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

public:	
	void use() const;			// 激活着色器程序
	void deleteProgram() const;	// 删除着色器程序

public:
	// uniform 工具函数
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z);
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;

private:	
	void compileShader(const std::string& vertexSource, const std::string& fragmentSource);	// 编译着色器源码
	void LinkProgram();			// 链接着色器程序

private:
	unsigned int m_shaderProgramID;		// 着色器程序ID
	unsigned int m_vertexShaderID;		// 顶点着色器ID
	unsigned int m_fragmentShaderID;	// 片段着色器ID
};
