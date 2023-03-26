#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

// ��������
void frambuffer_size_callback(GLFWwindow* window, int width, int height);
void proccessInput(GLFWwindow* window);

// ���ڴ�С
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	// ��ʼ��glfw
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // �������汾��
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // ���ôΰ汾��
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // ʹ�ú���ģʽ
	}

	// ����glfw����
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenglPractice", NULL, NULL);
	{
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);	// ���ڵ�����������Ϊ��ǰ�̵߳���������	
		glfwSetFramebufferSizeCallback(window, frambuffer_size_callback); // �������ڳߴ�ʱͬ�������ӿڳߴ�
	}

	// ��ʼ��glad: ��������OpenGL���ܵ�ָ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ������ɫ������
	Shader shaderProgram("src/VertexShaderSource.glsl", "src/FragmentShaderSource.glsl");

	// ����һ�������������(vertex array object, VAO)
	// һ�����㻺�����(vertex buffer object, VBO)
	// һ��Ԫ��\������������(element\index buffer object, EBO\IBO)
	unsigned int vao, vbo, ebo;
	{
		// ���ɶ���
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		{
			// �󶨶���
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

			// ���ö��������׼���豸����(Normalized Device Coordinates)
			// ����ÿ�������Ӧ����ɫ��Ϣ
			float vertices[] = {
				// λ��				// ��ɫ
				 0.5f,  0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	// ���Ͻ�
				 0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	// ���½�
				-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	// ���½�
				-0.5f,  0.5f, 0.0f,	0.5f, 0.5f, 0.5f,	// ���Ͻ�
			};
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		// ���������ݴ���

			// ���ö�������ָ��
			// λ������
			glVertexAttribPointer(
				0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);	// ���Ͷ�������
			glEnableVertexAttribArray(0);	// ���ö������ԣ���ӦvertexShaderSource�е�location = 0��

			// ��ɫ����
			glVertexAttribPointer(
				1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));	// ���Ͷ�������
			glEnableVertexAttribArray(1);	// ���ö������ԣ���ӦvertexShaderSource�е�location = 1��
		}

		// ������������
		unsigned int indices[] = {
			// �����Ǵ�0��ʼ
			// ����������ֵ�Ƕ�������vertices���±�
			// �����������±��������ϳɾ���
			0, 1, 3,	// ��һ��������
			1, 2, 3,	// �ڶ���������
		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// ���vbo��vao����ֹ�������޸ģ�
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	// ������ɫ��
	shaderProgram.use();
	glBindVertexArray(vao);

	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		// ��ؼ��̰�������
		proccessInput(window);

		// ��Ⱦ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// �������������Ļ����ɫ
		glClear(GL_COLOR_BUFFER_BIT);			// �����ɫ���壬���glClearColor�����õ���ɫ

		// ����ͼԪ
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	// ��ebo�������

		glfwSwapBuffers(window);				// ����ָ�����ڵ�ǰ��˻�����
		glfwPollEvents();						// �������й����¼�
	}

	// �ͷš�ɾ��֮ǰ�����������Դ
	glDeleteVertexArrays(2, &vao);
	glDeleteBuffers(2, &vbo);
	glDeleteBuffers(1, &ebo);
	shaderProgram.deleteProgram();
	glfwTerminate();
    return 0;
}

// �ص����������ݴ��ڴ�С�ı仯�����ӿڵĴ�С
void frambuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// ��ؼ��̰����İ��º��ɿ�
void proccessInput(GLFWwindow* window)
{	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);// �û�����esc�����رմ���
}