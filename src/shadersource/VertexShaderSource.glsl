// 顶点着色器源码
#version 330 core

layout (location = 0) in vec3 aPos;		// 顶点坐标
layout (location = 1) in vec3 aNormal;	// 顶点法向量

uniform mat4 model;						// 模型矩阵
uniform mat4 view;						// 观察矩阵
uniform mat4 projection;				// 投影矩阵
uniform mat3 NormalMat;					// 法线矩阵

out vec3 FragPos;
out vec3 FragNormal;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0)); // 片段着色器需要的顶点在世界坐标系下的坐标
	FragNormal = NormalMat * aNormal;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
