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
const unsigned int SCR_WIDTH = (int)(2048 * 1.0);
const unsigned int SCR_HEIGHT = (int)(1080 * 1.0);

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
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	// ��ʼ��glad: ��������OpenGL���ܵ�ָ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ����ȫ�ֵ�Opengl״̬
	glEnable(GL_DEPTH_TEST);	// ����Ȳ���
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);	// ��ģ�����
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

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

	// ģ�Ͷ���
	Model AyakaModel("src/modelsource/genshin_impact_obj/Ayaka model/Ayaka model.pmx");
	Model GanyuModel("src/modelsource/genshin_impact_obj/Ganyu model/Ganyu model.pmx");
	Model ThomaModel("src/modelsource/genshin_impact_obj/Genshin impact thoma/Thoma.pmx");
	Model HuTaoModel("src/modelsource/genshin_impact_obj/Hu Tao model/Hu Tao.pmx");
	Model LaSignoraModel("src/modelsource/genshin_impact_obj/La signora model/La signora model.pmx");
	Model scaramoucheModel("src/modelsource/genshin_impact_obj/Scaramouche model/scaramouche Model done.pmx");

	// shader����
	//Shader lightshader("src/shadersource/VertexShaderSource.glsl", "src/shadersource/LightFragmentShaderSource.glsl");
	Shader modelShader("src/shadersource/model_loading_vs.glsl", "src/shadersource/model_loading_fs.glsl");
	Shader frameShader("src/shadersource/model_loading_vs.glsl", "src/shadersource/LightFragmentShaderSource.glsl");

	// ����ģ��Shader�Ĺ�����Ϣ
	modelShader.use();
	{
		modelShader.setFloat("material.shininess", 32.0f);

		modelShader.setBool("dirLight.is_set", true);
		modelShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		modelShader.setVec3("dirLight.lb.ambient", 0.8f, 0.8f, 0.8f);
		modelShader.setVec3("dirLight.lb.diffuse", 0.4f, 0.4f, 0.4f);
		modelShader.setVec3("dirLight.lb.specular", 0.5f, 0.5f, 0.5f);

		modelShader.setBool("pointLights[0].is_set", false);
		modelShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		modelShader.setVec3("pointLights[0].lb.ambient", 0.05f, 0.05f, 0.05f);
		modelShader.setVec3("pointLights[0].lb.diffuse", 0.8f, 0.8f, 0.8f);
		modelShader.setVec3("pointLights[0].lb.specular", 1.0f, 1.0f, 1.0f);
		modelShader.setFloat("pointLights[0].at.constant", 1.0f);
		modelShader.setFloat("pointLights[0].at.linear", 0.09f);
		modelShader.setFloat("pointLights[0].at.quadratic", 0.032f);

		modelShader.setBool("pointLights[1].is_set", false);
		modelShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		modelShader.setVec3("pointLights[1].lb.ambient", 0.05f, 0.05f, 0.05f);
		modelShader.setVec3("pointLights[1].lb.diffuse", 0.8f, 0.8f, 0.8f);
		modelShader.setVec3("pointLights[1].lb.specular", 1.0f, 1.0f, 1.0f);
		modelShader.setFloat("pointLights[1].at.constant", 1.0f);
		modelShader.setFloat("pointLights[1].at.linear", 0.09f);
		modelShader.setFloat("pointLights[1].at.quadratic", 0.032f);

		modelShader.setBool("pointLights[2].is_set", false);
		modelShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		modelShader.setVec3("pointLights[2].lb.ambient", 0.05f, 0.05f, 0.05f);
		modelShader.setVec3("pointLights[2].lb.diffuse", 0.8f, 0.8f, 0.8f);
		modelShader.setVec3("pointLights[2].lb.specular", 1.0f, 1.0f, 1.0f);
		modelShader.setFloat("pointLights[2].at.constant", 1.0f);
		modelShader.setFloat("pointLights[2].at.linear", 0.09f);
		modelShader.setFloat("pointLights[2].at.quadratic", 0.032f);

		modelShader.setBool("pointLights[3].is_set", false);
		modelShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		modelShader.setVec3("pointLights[3].lb.ambient", 0.05f, 0.05f, 0.05f);
		modelShader.setVec3("pointLights[3].lb.diffuse", 0.8f, 0.8f, 0.8f);
		modelShader.setVec3("pointLights[3].lb.specular", 1.0f, 1.0f, 1.0f);
		modelShader.setFloat("pointLights[3].at.constant", 1.0f);
		modelShader.setFloat("pointLights[3].at.linear", 0.09f);
		modelShader.setFloat("pointLights[3].at.quadratic", 0.032f);

		modelShader.setBool("spotLight.is_set", false);
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	// �������ɫ����\��Ȼ���\ģ�建��

		// ͶӰ����(project matrix) 		
		glm::mat4 projection = glm::perspective(
			glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		// �۲����(view matrix)
		glm::mat4 view = camera.GetViewMatrix();					

		// �����Դ�����ù�Դ����
		//lightshader.use();
		//{			
		//	lightshader.setMat4("projection", projection);
		//	lightshader.setMat4("view", view);

		//	// ����4����Դ
		//	glBindVertexArray(lightVAO);
		//	for (unsigned int i = 0; i < 4; ++i)
		//	{
		//		glm::mat4 lightmodel(1.0f);
		//		lightmodel = glm::translate(lightmodel, pointLightPositions[i]);
		//		lightmodel = glm::scale(lightmodel, glm::vec3(0.1f));	// ��С��Դ���
		//		lightshader.setMat4("model", lightmodel);
		//		glDrawArrays(GL_TRIANGLES, 0, 36);
		//	}
		//}


		// ���ģ�ͻ���
		{			
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);

			// ����ն�����ɫ��������VP�������λ��
			modelShader.use();
			modelShader.setMat4("projection", projection);
			modelShader.setMat4("view", view);
			modelShader.setVec3("cameraPos", camera.GetCameraPosition());

			// �������ģ��M���󼰷���������
			glm::mat4 leftModelMat(1.0f);		// ģ�;���(model matrix)
			leftModelMat = glm::translate(leftModelMat, glm::vec3(-1.0f, -0.9f, 0.0f));
			leftModelMat = glm::rotate(leftModelMat, glm::radians(currentFrame * 50), glm::vec3(0.0f, 1.0f, 0.0f));
			leftModelMat = glm::scale(leftModelMat, glm::vec3(0.1f));
			glm::mat3 leftNormalMat = glm::mat3(glm::transpose(glm::inverse(leftModelMat)));// ���߾���
			modelShader.setMat4("model", leftModelMat);
			modelShader.setMat3("NormalMat", leftNormalMat);

			//AyakaModel.Draw(modelShader);
			//ThomaModel.Draw(modelShader);
			LaSignoraModel.Draw(modelShader);


			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			glDisable(GL_DEPTH_TEST);
			frameShader.use();
			frameShader.setMat4("projection", projection);
			frameShader.setMat4("view", view);
			leftModelMat = glm::scale(leftModelMat, glm::vec3(1.01f));
			frameShader.setMat4("model", leftModelMat);
			LaSignoraModel.Draw(modelShader);
			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 0, 0xFF);
			glEnable(GL_DEPTH_TEST);
		}




		//// �����Ҳ�ģ��M���󼰷���������
		//glm::mat4 rightModelMat(1.0f);
		//rightModelMat = glm::translate(rightModelMat, glm::vec3(1.0f, -0.9f, 0.0f));
		//rightModelMat = glm::rotate(rightModelMat, glm::radians(currentFrame * 50), glm::vec3(0.0f, 1.0f, 0.0f));
		//rightModelMat = glm::scale(rightModelMat, glm::vec3(0.1f));
		//glm::mat3 rightNormalMat = glm::mat3(glm::transpose(glm::inverse(rightModelMat)));
		//modelShader.setMat4("model", rightModelMat);
		//modelShader.setMat3("NormalMat", rightModelMat);
		//
		//// �����Ҳ�ģ��
		////GanyuModel.Draw(modelShader);
		////HuTaoModel.Draw(modelShader);
		//scaramoucheModel.Draw(modelShader);

		glfwSwapBuffers(window);				// ����ָ�����ڵ�ǰ��˻�����
		glfwPollEvents();						// �������й����¼�
	}

	// �ͷš�ɾ��֮ǰ�����������Դ
	glDeleteVertexArrays(2, &lightVAO);
	glDeleteBuffers(2, &lightVBO);
	//lightshader.deleteProgram();
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
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
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
	else
	{
		firstMouse = true;
	}	
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