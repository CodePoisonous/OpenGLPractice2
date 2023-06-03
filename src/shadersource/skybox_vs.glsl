#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	TexCoords = aPos;
	vec4 pos = projection * view * model * vec4(aPos, 1.0);

	// 将z值设置为w，即可在深度测试时保持最大的深度值（z/w = w/w = 1.0）
	gl_Position = pos.xyww;
}