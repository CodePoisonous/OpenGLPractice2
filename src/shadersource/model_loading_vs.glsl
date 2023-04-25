// ������ɫ��Դ��
#version 330 core

layout (location = 0) in vec3 aPos;			// ��������
layout (location = 1) in vec3 aNormal;		// ���㷨����
layout (location = 2) in vec2 aTexCoords;	// ��������


uniform mat4 model;			// ģ�;���
uniform mat4 view;			// �۲����
uniform mat4 projection;	// ͶӰ����
uniform mat3 NormalMat;		// ���߾���

out vec3 FragPos;			// Ƭ������������ϵ�µ�����				
out vec3 FragNormal;		// Ƭ������������ϵ�µķ�����
out vec2 TexCoords;			// ��������

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
	FragNormal = NormalMat * aNormal;
	TexCoords = aTexCoords;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
