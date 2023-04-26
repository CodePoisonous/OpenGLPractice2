#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define  STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Camera.h"
#include "Shader.h"
#include "Model.h"


// ��������
void frambuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void proccessInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

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

// ��Դ����������ϵ�µ�λ��
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	// ��ʼ��glfw
	glfwInit();
	{		
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

	// ���ö�����Դ����������ϵ��λ��
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	// ���ö��������׼���豸����(Normalized Device Coordinates)
	// ����ÿ�������Ӧ����ɫ��Ϣ
	float vertices[] = {
		// positions        
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	// ���ɹ�Դ��VAO
	unsigned int lightVAO, lightVBO;
	{
		glGenVertexArrays(1, &lightVAO);
		glGenBuffers(1, &lightVBO);

		glBindVertexArray(lightVAO);
		glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		

		// ��Դ�����嶥��λ�����ݽ���
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// ���vbo��vao����ֹ�������޸ģ�
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	// �ⲿ�����ģ�Ͷ���
	Model ourModel("src/modelsource/nanosuit/nanosuit.obj");

	// �ⲿ����ģ�͵�shader
	Shader modelShader("src/shadersource/model_loading_vs.glsl","src/shadersource/model_loading_fs.glsl");
	// ��Դshader
	Shader lightshader("src/shadersource/VertexShaderSource.glsl", "src/shadersource/LightFragmentShaderSource.glsl");

	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// ��ؼ��̰�������
		proccessInput(window);

		// ��Ⱦ
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// �������������Ļ����ɫ
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// �����ɫ\��Ȼ���

		// ͶӰ����(project matrix) 		
		glm::mat4 projection = glm::perspective(
			glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		// �۲����(view matrix)
		glm::mat4 view = camera.GetViewMatrix();		

		// ģ�;���(model matrix)
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));

		// ���߾���
		glm::mat3 NormalMat = glm::mat3(glm::transpose(glm::inverse(model)));

		// �����Դ�����ù�Դ����
		lightshader.use();
		{			
			lightshader.setMat4("projection", projection);
			lightshader.setMat4("view", view);

			// ����4����Դ
			glBindVertexArray(lightVAO);
			for (unsigned int i = 0; i < 4; ++i)
			{
				glm::mat4 lightmodel(1.0f);
				lightmodel = glm::translate(lightmodel, pointLightPositions[i]);
				lightmodel = glm::scale(lightmodel, glm::vec3(0.1f));	// ��С��Դ���
				lightshader.setMat4("model", lightmodel);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		// ����ն�����ɫ������������
		modelShader.use();
		{
			modelShader.setMat4("projection", projection);
			modelShader.setMat4("view", view);
			modelShader.setMat4("model", model);
			modelShader.setMat3("NormalMat", NormalMat);

			modelShader.setVec3("cameraPos", camera.GetCameraPosition());			
			modelShader.setFloat("material.shininess", 32.0f);

			modelShader.setBool("dirLight.is_set", true);
			modelShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
			modelShader.setVec3("dirLight.lb.ambient", 0.05f, 0.05f, 0.05f);
			modelShader.setVec3("dirLight.lb.diffuse", 0.4f, 0.4f, 0.4f);
			modelShader.setVec3("dirLight.lb.specular", 1.0f, 1.0f, 1.0f);

			modelShader.setBool("pointLights[0].is_set", true);
			modelShader.setVec3("pointLights[0].position", pointLightPositions[0]);
			modelShader.setVec3("pointLights[0].lb.ambient", 0.05f, 0.05f, 0.05f);
			modelShader.setVec3("pointLights[0].lb.diffuse", 0.8f, 0.8f, 0.8f);
			modelShader.setVec3("pointLights[0].lb.specular", 1.0f, 1.0f, 1.0f);
			modelShader.setFloat("pointLights[0].at.constant", 1.0f);
			modelShader.setFloat("pointLights[0].at.linear", 0.09f);
			modelShader.setFloat("pointLights[0].at.quadratic", 0.032f);
			
			modelShader.setBool("pointLights[1].is_set", true);
			modelShader.setVec3("pointLights[1].position", pointLightPositions[1]);
			modelShader.setVec3("pointLights[1].lb.ambient", 0.05f, 0.05f, 0.05f);
			modelShader.setVec3("pointLights[1].lb.diffuse", 0.8f, 0.8f, 0.8f);
			modelShader.setVec3("pointLights[1].lb.specular", 1.0f, 1.0f, 1.0f);
			modelShader.setFloat("pointLights[1].at.constant", 1.0f);
			modelShader.setFloat("pointLights[1].at.linear", 0.09f);
			modelShader.setFloat("pointLights[1].at.quadratic", 0.032f);
			
			modelShader.setBool("pointLights[2].is_set", true);
			modelShader.setVec3("pointLights[2].position", pointLightPositions[2]);
			modelShader.setVec3("pointLights[2].lb.ambient", 0.05f, 0.05f, 0.05f);
			modelShader.setVec3("pointLights[2].lb.diffuse", 0.8f, 0.8f, 0.8f);
			modelShader.setVec3("pointLights[2].lb.specular", 1.0f, 1.0f, 1.0f);
			modelShader.setFloat("pointLights[2].at.constant", 1.0f);
			modelShader.setFloat("pointLights[2].at.linear", 0.09f);
			modelShader.setFloat("pointLights[2].at.quadratic", 0.032f);
			
			modelShader.setBool("pointLights[3].is_set", true);
			modelShader.setVec3("pointLights[3].position", pointLightPositions[3]);
			modelShader.setVec3("pointLights[3].lb.ambient", 0.05f, 0.05f, 0.05f);
			modelShader.setVec3("pointLights[3].lb.diffuse", 0.8f, 0.8f, 0.8f);
			modelShader.setVec3("pointLights[3].lb.specular", 1.0f, 1.0f, 1.0f);
			modelShader.setFloat("pointLights[3].at.constant", 1.0f);
			modelShader.setFloat("pointLights[3].at.linear", 0.09f);
			modelShader.setFloat("pointLights[3].at.quadratic", 0.032f);
			
			modelShader.setBool("spotLight.is_set", true);
			modelShader.setVec3("spotLight.position", camera.GetCameraPosition());
			modelShader.setVec3("spotLight.direction", camera.GetCameraFront());
			modelShader.setFloat("spotLight.cutOffCos", glm::cos(glm::radians(12.0f)));
			modelShader.setFloat("spotLight.outerCutOffCos", glm::cos(glm::radians(15.0f)));
			modelShader.setVec3("spotLight.lb.ambient", 0.0f, 0.0f, 0.0f);
			modelShader.setVec3("spotLight.lb.diffuse", 1.0f, 1.0f, 1.0f);
			modelShader.setVec3("spotLight.lb.specular", 1.0f, 1.0f, 1.0f);
			modelShader.setFloat("spotLight.at.constant", 1.0f);
			modelShader.setFloat("spotLight.at.linear", 0.09f);
			modelShader.setFloat("spotLight.at.quadratic", 0.032f);
		}
		ourModel.Draw(modelShader);

		glfwSwapBuffers(window);				// ����ָ�����ڵ�ǰ��˻�����
		glfwPollEvents();						// �������й����¼�
	}

	// �ͷš�ɾ��֮ǰ�����������Դ
	glDeleteVertexArrays(2, &lightVAO);
	glDeleteBuffers(2, &lightVBO);
	lightshader.deleteProgram();
	modelShader.deleteProgram();
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
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}

unsigned int loadTexture(const char* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}