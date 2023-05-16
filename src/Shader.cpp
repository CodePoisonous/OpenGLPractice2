#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

using namespace std;

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	// 通过文件路径获取两个着色器源码字符串

	// 保证ifstream对象可以抛出异常
	ifstream vShaderFile, fShaderFile;
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	try {
		// 打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		// 读取文件的缓冲内容到数据流中
		stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// 关闭文件处理器
		vShaderFile.close();
		fShaderFile.close();

		// 将数据流内容转到字符串中
		string vertexCode = vShaderStream.str();
		string fragmentCode = fShaderStream.str();

		// 编译着色器源码
		compileShader(vertexCode, fragmentCode);

		// 链接着色器程序
		LinkProgram();
	}
	catch (ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
	}
}

Shader::~Shader()
{

}

void Shader::use() const
{
	glUseProgram(m_shaderProgramID);
}

void Shader::deleteProgram() const
{
	glDeleteProgram(m_shaderProgramID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	int location = glGetUniformLocation(m_shaderProgramID, name.c_str());
	//if (-1 == location) cout << "SHADER SETBOOL ERROR: " + name << endl;
	//else 
	glUniform1i(location, (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	int location = glGetUniformLocation(m_shaderProgramID, name.c_str());
	//if (-1 == location) cout << "SHADER SETINT ERROR: " + name << endl;
	//else 
	glUniform1i(location, value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	int location = glGetUniformLocation(m_shaderProgramID, name.c_str());
	//if (-1 == location) cout << "SHADER SETFLOAT ERROR: " + name << endl;
	//else 
	glUniform1f(location, value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	int location = glGetUniformLocation(m_shaderProgramID, name.c_str());
	//if (-1 == location) cout << "SHADER SETVEC3 ERROR: " + name << endl;
	//else 
	glUniform3fv(location, 1, &value[0]);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	int location = glGetUniformLocation(m_shaderProgramID, name.c_str());
	//if (-1 == location) cout << "SHADER SETVEC3 ERROR: " + name << endl;
	//else 
	glUniform3f(location, x, y, z);
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	int location = glGetUniformLocation(m_shaderProgramID, name.c_str());
	//if (-1 == location) cout << "SHADER SETMAT3 ERROR: " + name << endl;
	//else 
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	int location = glGetUniformLocation(m_shaderProgramID, name.c_str());
	//if (-1 == location) cout << "SHADER SETMAT4 ERROR: " + name << endl;
	//else 
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::compileShader(const string& vertexSource, const string& fragmentSource)
{
	// 生成一个顶点着色器对象，并编译顶点着色器源码
	m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	{
		const char* vertexShaderSource = vertexSource.c_str();
		glShaderSource(m_vertexShaderID, 1, &vertexShaderSource, NULL);
		glCompileShader(m_vertexShaderID);
		int success;	// 判断是否编译成功
		glGetShaderiv(m_vertexShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(m_vertexShaderID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}


	// 生成片段着色器对象，并编译片段着色器源码
	m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	{
		const char* fragmentShaderSource = fragmentSource.c_str();
		glShaderSource(m_fragmentShaderID, 1, &fragmentShaderSource, NULL);
		glCompileShader(m_fragmentShaderID);
		int success;	// 判断是否编译成功
		glGetShaderiv(m_fragmentShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(m_fragmentShaderID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT1::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
}

void Shader::LinkProgram()
{
	// 创建一个着色器程序
	m_shaderProgramID = glCreateProgram();
	glAttachShader(m_shaderProgramID, m_vertexShaderID);	// 链接顶点着色器
	glAttachShader(m_shaderProgramID, m_fragmentShaderID);	// 链接片段着色器
	glLinkProgram(m_shaderProgramID);
	int success;	// 判断是否编译成功
	glGetProgramiv(m_shaderProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(m_shaderProgramID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM1::LINK_FAILED\n" << infoLog << std::endl;
	}

	// 链接成功后即可删除
	glDeleteShader(m_vertexShaderID);
	glDeleteShader(m_fragmentShaderID);
}
