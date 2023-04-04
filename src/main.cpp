#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define  STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Camera.h"
#include "Shader.h"


// ��������
void frambuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void proccessInput(GLFWwindow* window);

// ���ڴ�С
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// �����״̬
Camera camera;
float lastX = SCR_WIDTH * 0.5;	// �����x��ǰһ֡��λ��
float lastY = SCR_HEIGHT * 0.5;	// �����y��ǰһ֡��λ��
bool firstMouse = true;	// ����Ƿ��һ�ν��봰��

// ��¼û֮֡���ʱ���ֵ
float deltaTime = 0.0f;	// ��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f;	// ��һ֡��ʱ��

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
		glfwSetCursorPosCallback(window, mouse_callback);// ���λ��
		glfwSetScrollCallback(window, scroll_callback);	 // ����ת��

		// ��������ʱ�����ص����
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	// ��ʼ��glad: ��������OpenGL���ܵ�ָ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ����ȫ�ֵ�Opengl״̬
	glEnable(GL_DEPTH_TEST);	// ����Ȳ���

	// ������ɫ������
	Shader shaderProgram("src/shadersource/VertexShaderSource.glsl", "src/shadersource/FragmentShaderSource.glsl");

	// �趨����������������ϵ��λ��
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// ����VAO��VBO��EBO
	unsigned int vao, vbo, ebo;
	{
		// ���ɶ���
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		//glGenBuffers(1, &ebo);

		// �󶨶���
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		// ���ö��������׼���豸����(Normalized Device Coordinates)
		// ����ÿ�������Ӧ����ɫ��Ϣ
		float vertices[] = {	
			// ����������		// ��������
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// ���ö�������ָ��
		{
			// λ������
			glVertexAttribPointer(
				0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0); // ���ö������ԣ���ӦvertexShaderSource�е�location = 0��

			// ��������
			glVertexAttribPointer(
				1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1); // ���ö������ԣ���ӦvertexShaderSource�е�location = 1��
		}

		//// ������������
		//unsigned int indices[] = {		
		//	0, 1, 3,	// ��һ��������
		//	1, 2, 3,	// �ڶ���������
		//};
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// ��ؼ��̰�������
		proccessInput(window);

		// ��Ⱦ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// �������������Ļ����ɫ
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// �����ɫ\��Ȼ���

		// ���������Ԫ
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		
		// ������ɫ������
		shaderProgram.use();

		// ����ͶӰ����(project matrix) 		
		glm::mat4 projection(1.0f);
		{
			projection = glm::perspective(glm::radians(camera.Zoom),
				(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			shaderProgram.setMatrix4fv("projection", glm::value_ptr(projection));
		}

		// �����۲����(view matrix)
		glm::mat4 view(1.0f);
		{
			view = camera.GetViewMatrix();
			shaderProgram.setMatrix4fv("view", glm::value_ptr(view));
		}

		// ����10��������
		glBindVertexArray(vao);
		for (unsigned int i = 0; i < 10; ++i)
		{
			// ����ģ�;���(model matrix)
			glm::mat4 model(1.0f);

			// ��ģ���ƶ���ָ������������ϵλ��
			model = glm::translate(model, cubePositions[i]);

			// ��ģ���Բ�ͬ���ٶ��ƾֲ������x����ת
			model = glm::rotate(model,
				currentFrame * glm::radians(25.0f) * (i+1), glm::vec3(0.5f, 1.0f, 0.0f));

			// ת����������ɫ��
			shaderProgram.setMatrix4fv("model", glm::value_ptr(model));

			// ����������
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// ����ͼԪ
		//glBindVertexArray(vao);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	// ��ebo�������
		//glDrawArrays(GL_TRIANGLES, 0, 36);

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

// �ص�����������ƶ�
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float xPos = (float)xpos;
	float yPos = (float)ypos;

	// ��һ��ʱ��ʼ��λ��
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	// ������֮֡������λ�� 
	float xoffset = xPos - lastX;
	float yoffset = lastY - yPos;// ���������opengl��y�������Ƿ���

	// ���浱ǰ֡��λ��
	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xoffset, yoffset, true);
}

// �ص�������������
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}

// ��ؼ��̰����İ��º��ɿ�
void proccessInput(GLFWwindow* window)
{	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);// �û�����esc�����رմ���
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}