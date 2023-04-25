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

	/*
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
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	// ���ɱ��ն����VAO��VBO
	unsigned int objVAO, vbo;
	{
		glGenVertexArrays(1, &objVAO);
		glGenBuffers(1, &vbo);

		glBindVertexArray(objVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);				
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// ���ն��󶥵�λ�����ݽ���
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// ���vbo��vao����ֹ�������޸ģ�
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	// ���ɹ�Դ��VAO
	unsigned int lightVAO;
	{
		glGenVertexArrays(1, &lightVAO);
		glBindVertexArray(lightVAO);

		// ֻ��Ҫ��VBO�����ٴ�����VBO�����ݣ���Ϊ���ӵ�VBO�������Ѿ���������ȷ�������嶥������
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// ��Դ�����嶥��λ�����ݽ���
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// ���vbo��vao����ֹ�������޸ģ�
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	// ������ɫ������
	Shader objshader("src/shadersource/VertexShaderSource.vs", "src/shadersource/FragmentShaderSource.fs");
	Shader lightshader("src/shadersource/VertexShaderSource.vs", "src/shadersource/LightFragmentShaderSource.fs");

	// ��������ͼƬ
	unsigned int diffuseMap = loadTexture("src/texture/container.png");
	unsigned int specularMap = loadTexture("src/texture/container_specular.png");
	{
		objshader.use();
		objshader.setInt("material.diffuse", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		objshader.setInt("material.specular", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
	}
	*/

	// �ⲿ�����ģ�Ͷ���
	Model ourModel("src/modelsource/nanosuit/nanosuit.obj");

	// �ⲿ����ģ�͵�shader
	Shader modelShader("src/shadersource/model_loading_vs.glsl","src/shadersource/model_loading_fs.glsl");

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

		modelShader.use();

		// ͶӰ����(project matrix) 		
		glm::mat4 projection = glm::perspective(
			glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		modelShader.setMat4("projection", projection);

		// �۲����(view matrix)
		glm::mat4 view = camera.GetViewMatrix();		
		modelShader.setMat4("view", view);

		// ģ�;���
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		modelShader.setMat4("model", model);

		ourModel.Draw(modelShader);

		//glDrawArrays(GL_TRIANGLES, 0, 36);

		/*
		// �����Դ�����ù�Դ����
		lightshader.use();
		{
			lightshader.setMat4("projection", projection);
			lightshader.setMat4("view", view);
		}

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

		// ����ն�����ɫ������������
		objshader.use();
		{
			objshader.setMat4("projection", projection);
			objshader.setMat4("view", view);
			objshader.setVec3("cameraPos", camera.GetCameraPosition());			
			objshader.setFloat("material.shininess", 32.0f);

			objshader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
			objshader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
			objshader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
			objshader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

			objshader.setVec3("pointLights[0].position", pointLightPositions[0]);
			objshader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
			objshader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
			objshader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
			objshader.setFloat("pointLights[0].constant", 1.0f);
			objshader.setFloat("pointLights[0].linear", 0.09f);
			objshader.setFloat("pointLights[0].quadratic", 0.032f);
			
			objshader.setVec3("pointLights[1].position", pointLightPositions[1]);
			objshader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
			objshader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
			objshader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
			objshader.setFloat("pointLights[1].constant", 1.0f);
			objshader.setFloat("pointLights[1].linear", 0.09f);
			objshader.setFloat("pointLights[1].quadratic", 0.032f);
			
			objshader.setVec3("pointLights[2].position", pointLightPositions[2]);
			objshader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
			objshader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
			objshader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
			objshader.setFloat("pointLights[2].constant", 1.0f);
			objshader.setFloat("pointLights[2].linear", 0.09f);
			objshader.setFloat("pointLights[2].quadratic", 0.032f);
			
			objshader.setVec3("pointLights[3].position", pointLightPositions[3]);
			objshader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
			objshader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
			objshader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
			objshader.setFloat("pointLights[3].constant", 1.0f);
			objshader.setFloat("pointLights[3].linear", 0.09f);
			objshader.setFloat("pointLights[3].quadratic", 0.032f);
			
			objshader.setVec3("spotLight.position", camera.GetCameraPosition());
			objshader.setVec3("spotLight.direction", camera.GetCameraFront());
			objshader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
			objshader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
			objshader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
			objshader.setFloat("spotLight.constant", 1.0f);
			objshader.setFloat("spotLight.linear", 0.09f);
			objshader.setFloat("spotLight.quadratic", 0.032f);
			objshader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
			objshader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
		}

		// ����10������������
		glBindVertexArray(objVAO);
		for (unsigned int i = 0; i < 10; ++i)
		{
			// ����ģ�;���(model matrix
			glm::mat4 model(1.0f);

			// ��ģ���ƶ���ָ������������ϵλ��
			model = glm::translate(model, cubePositions[i]);

			// ��ģ���Բ�ͬ���ٶ��ƾֲ������x����ת
			float rad = currentFrame * glm::radians(10.0f) * (i + 1);
			model = glm::rotate(model, rad, glm::vec3(0.5f, 1.0f, 0.0f));

			// ת����������ɫ��
			objshader.setMat4("model", model);

			// ���߾���
			glm::mat3 NormalMat = glm::mat3(glm::transpose(glm::inverse(model)));
			objshader.setMat3("NormalMat", NormalMat);

			// ����������
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		*/

		glfwSwapBuffers(window);				// ����ָ�����ڵ�ǰ��˻�����
		glfwPollEvents();						// �������й����¼�
	}

	// �ͷš�ɾ��֮ǰ�����������Դ
	//glDeleteVertexArrays(2, &objVAO);
	//glDeleteBuffers(2, &vbo);
	//objshader.deleteProgram();
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