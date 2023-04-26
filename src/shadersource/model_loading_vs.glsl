// 顶点着色器源码
#version 330 core

layout (location = 0) in vec3 aPos;			// 顶点坐标
layout (location = 1) in vec3 aNormal;		// 顶点法向量
layout (location = 2) in vec2 aTexCoords;	// 纹理坐标


uniform mat4 model;			// 模型矩阵
uniform mat4 view;			// 观察矩阵
uniform mat4 projection;	// 投影矩阵
uniform mat3 NormalMat;		// 法线矩阵

out vec3 FragPos;			// 片段在世界坐标系下的坐标				
out vec3 FragNormal;		// 片段在世界坐标系下的法向量
out vec2 TexCoords;			// 纹理坐标

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
	FragNormal = NormalMat * aNormal;
	TexCoords = aTexCoords;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
