// 顶点着色器源码
#version 330 core

layout (location = 0) in vec3 aPos;		// 顶点坐标 位置值为0
layout (location = 1) in vec3 aColor;	// 颜色信息 位置值为1
layout (location = 2) in vec2 aTexcoord;// 纹理坐标 位置值为2

out vec3 ourColor;						// 向片段着色器输出颜色信息
out vec2 TexCoord;						// 向片段着色器输出纹理坐标

void main()
{
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = aTexcoord;
}
