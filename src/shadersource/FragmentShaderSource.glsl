// 片段着色器源码
#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform float MixValue;

void main()
{
	// 返回80%第一个输入，20%第二个输入
	FragColor = mix(texture(Texture1, TexCoord), texture(Texture2, TexCoord), MixValue);
}