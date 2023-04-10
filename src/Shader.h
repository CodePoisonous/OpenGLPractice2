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
	void use() const;			// ������ɫ������
	void deleteProgram() const;	// ɾ����ɫ������

public:
	// uniform ���ߺ���
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z);
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;

private:	
	void compileShader(const std::string& vertexSource, const std::string& fragmentSource);	// ������ɫ��Դ��
	void LinkProgram();			// ������ɫ������

private:
	unsigned int m_shaderProgramID;		// ��ɫ������ID
	unsigned int m_vertexShaderID;		// ������ɫ��ID
	unsigned int m_fragmentShaderID;	// Ƭ����ɫ��ID
};
