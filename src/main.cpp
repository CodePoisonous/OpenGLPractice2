#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define  STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"


// ��������
void frambuffer_size_callback(GLFWwindow* window, int width, int height);
void proccessInput(GLFWwindow* window);

// ���ڴ�С
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// ���ųߴ�
float scale_x = 1.0;
float scale_y = 1.0;

// ��ת�Ƕ�
float angle = 0.0;

// �ƶ���Χ
float move_x = 0.0;
float move_y = 0.0;

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
	Shader shaderProgram("src/shadersource/VertexShaderSource.glsl", "src/shadersource/FragmentShaderSource.glsl");

	// ����һ�������������(vertex array object, VAO)
	// һ�����㻺�����(vertex buffer object, VBO)
	// һ��Ԫ��\������������(element\index buffer object, EBO\IBO)
	unsigned int vao, vbo, ebo;
	{
		// ���ɶ���
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		// �󶨶���
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		// ���ö��������׼���豸����(Normalized Device Coordinates)
		// ����ÿ�������Ӧ����ɫ��Ϣ
		float vertices[] = {
			 // λ��				 // ��ɫ			    // ��������
			 0.5f,  0.5f, 0.0f,	 1.0f, 0.0f, 0.0f,  1.0f, 1.0f,	// ���Ͻ�
			 0.5f, -0.5f, 0.0f,	 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,	// ���½�
			-0.5f, -0.5f, 0.0f,	 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,	// ���½�
			-0.5f,  0.5f, 0.0f,	 0.5f, 0.5f, 0.5f,  0.0f, 1.0f,	// ���Ͻ�
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		// ���������ݴ���

		// ���ö�������ָ��
		{
			// λ������
			glVertexAttribPointer(
				0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);	// ���Ͷ�������
			glEnableVertexAttribArray(0);	// ���ö������ԣ���ӦvertexShaderSource�е�location = 0��

			// ��ɫ����
			glVertexAttribPointer(
				1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));	// ���Ͷ�������
			glEnableVertexAttribArray(1);	// ���ö������ԣ���ӦvertexShaderSource�е�location = 1��

			// ��������
			glVertexAttribPointer(
				2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));	// ���Ͷ�������
			glEnableVertexAttribArray(2);	// ���ö������ԣ���ӦvertexShaderSource�е�location = 2��
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

	// ����һ������(texture)
	unsigned int texture1;
	{
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		
		// ���û��Ʒ�ʽ�͹��˷�ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	  // X,Y�������Ʒ�ʽΪ�ظ�
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // �Ŵ���Сʱ������˷�ʽΪ���Բ�ֵ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// ���ز���������
		int width, height, nrchannels;
		unsigned char* data = stbi_load("src/texture/wall.jpg", &width, &height, &nrchannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else 
		{
			std::cout << "Failed to load texture1" << std::endl;
		}

		stbi_image_free(data);
	}
	unsigned int texture2;
	{
		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// ���û��Ʒ�ʽ�͹��˷�ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	  // X,Y�������Ʒ�ʽΪ�ظ�
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // �Ŵ���Сʱ������˷�ʽΪ���Բ�ֵ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// ���ز���������
		int width, height, nrchannels;
		stbi_set_flip_vertically_on_load(true);	// ��תy�᷽��
		unsigned char* data = stbi_load("src/texture/awesomeface.png", &width, &height, &nrchannels, 0);
		if (data)
		{
			// pngͼƬ��alphaֵ�������ó�GL_RGBA
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture2" << std::endl;
		}

		stbi_image_free(data);
	}


	// ������ɫ��
	shaderProgram.use();
	shaderProgram.setInt("Texture1", 0);
	shaderProgram.setInt("Texture2", 1);

	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		// ��ؼ��̰�������
		proccessInput(window);

		// ��Ⱦ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// �������������Ļ����ɫ
		glClear(GL_COLOR_BUFFER_BIT);			// �����ɫ���壬���glClearColor�����õ���ɫ

		// ���������Ԫ
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// �����任����
		glm::mat4 trans(1.0f);
		trans = glm::translate(trans, glm::vec3(move_x, move_y, 0.0f));
		trans = glm::rotate(trans, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));	// ��z����ʱ��ת
		trans = glm::scale(trans, glm::vec3(scale_x, scale_y, 1.0f));	// xy�᷽������


		// ������ɫ������
		shaderProgram.use();
		shaderProgram.setMatrix4fv("transform", glm::value_ptr(trans));

		// ����ͼԪ
		glBindVertexArray(vao);
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
	
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		move_y += 0.01f;
		if (move_y >= 400.0f) move_y = 400.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		move_y -= 0.01f;
		if (move_y <= -400.0f) move_y = -400.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		move_x -= 0.01f;
		if (move_x <= -400.0f) move_x = -400.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		move_x += 0.01f;
		if (move_x >= 400.0f) move_x = 400.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		scale_y += 0.01f;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		scale_y -= 0.01f;
		if (scale_y <= 0.0f) scale_y = 0.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		scale_x += 0.01f;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		scale_x -= 0.01f;
		if (scale_x <= 0.0f) scale_x = 0.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		angle += 5.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		angle -= 5.0f;
	}
}