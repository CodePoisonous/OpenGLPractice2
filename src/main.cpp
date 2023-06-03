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
#include "Mesh.h"

using namespace std;

// ��������
void frambuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void proccessInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
unsigned int LoadCubeMap(const vector<string>& faces);

// ���ڴ�С
const unsigned int SCR_WIDTH = (int)(2048);
const unsigned int SCR_HEIGHT = (int)(1080);

// �����״̬
Camera camera(glm::vec3(0.0f, 0.8f, 3.0f));
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

	//glEnable(GL_STENCIL_TEST);	// ��ģ�����
	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);		//һ��ʼģ�建���Ĭ��ֵ��0�����������ڲ�����1��λ�û������塣
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);	//���ͨ�����ԣ��ͽ�ģ�建���е�ֵ����Ϊָ����refֵ�����������ģ����£���������ϣ�

	//glEnable(GL_BLEND);			// �򿪻��
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_CULL_FACE);		// �����޳�
	//glCullFace(GL_FRONT);

	// ���ö�����Դ����������ϵ��λ��
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	// ���ö��������׼���豸����(Normalized Device Coordinates)
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

	// ��պ�����
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
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

	// ������պе�VAO
	unsigned int skyboxVAO, skyboxVBO;
	{
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);

		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

		// ��Դ�����嶥��λ�����ݽ���
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// ���vbo��vao����ֹ�������޸ģ�
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	// ������պ�����
	vector<string> skyboxFaces
	{
		"src/texture/skybox/right.jpg",
		"src/texture/skybox/left.jpg",
		"src/texture/skybox/top.jpg",
		"src/texture/skybox/bottom.jpg",
		"src/texture/skybox/front.jpg",
		"src/texture/skybox/back.jpg"
	};
	unsigned int skyboxTexture = LoadCubeMap(skyboxFaces);

	// ģ�Ͷ���
	//Model AyakaModel("src/modelsource/genshin_impact_obj/Ayaka model/Ayaka model.pmx");
	//Model GanyuModel("src/modelsource/genshin_impact_obj/Ganyu model/Ganyu model.pmx");
	//Model ThomaModel("src/modelsource/genshin_impact_obj/Genshin impact thoma/Thoma.pmx");
	//Model HuTaoModel("src/modelsource/genshin_impact_obj/Hu Tao model/Hu Tao.pmx");
	Model LaSignoraModel("src/modelsource/genshin_impact_obj/La signora model/La signora model.pmx");
	Model scaramoucheModel("src/modelsource/genshin_impact_obj/Scaramouche model/scaramouche Model done.pmx");

	// shader����
	//Shader lightshader("src/shadersource/VertexShaderSource.glsl", "src/shadersource/LightFragmentShaderSource.glsl");
	Shader modelShader("src/shadersource/model_loading_vs.glsl", "src/shadersource/model_loading_fs.glsl");
	//Shader frameShader("src/shadersource/model_loading_vs.glsl", "src/shadersource/LightFragmentShaderSource.glsl");
	Shader skyboxShader("src/shadersource/skybox_vs.glsl", "src/shadersource/skybox_fs.glsl");

	// ����ģ��Shader�Ĺ�����Ϣ
	modelShader.use();
	{
		modelShader.setFloat("material.shininess", 32.0f);

		modelShader.setBool("dirLight.is_set", true);
		modelShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		modelShader.setVec3("dirLight.lb.ambient", 0.8f, 0.8f, 0.8f);
		modelShader.setVec3("dirLight.lb.diffuse", 0.4f, 0.4f, 0.4f);
		modelShader.setVec3("dirLight.lb.specular", 0.01f, 0.01f, 0.01f);

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

	// ģ�ͻ���ģʽ
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);// �����߿�ģʽ

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT/* | GL_STENCIL_BUFFER_BIT*/);	// �������ɫ����\��Ȼ���\ģ�建��

		// ͶӰ����(project matrix) 		
		glm::mat4 projection = glm::perspective(
			glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		// �۲����(view matrix)
		glm::mat4 view = camera.GetViewMatrix();					

		// ���ģ�;���(model matrix)
		glm::mat4 leftModelMat(1.0f);
		leftModelMat = glm::translate(leftModelMat, glm::vec3(-1.0f, -0.9f, 0.0f));
		leftModelMat = glm::rotate(leftModelMat, glm::radians(currentFrame * 50), glm::vec3(0.0f, 1.0f, 0.0f));
		leftModelMat = glm::scale(leftModelMat, glm::vec3(0.1f));
		glm::mat3 leftNormalMat = glm::mat3(glm::transpose(glm::inverse(leftModelMat)));// ���߾���

		// �Ҳ�ģ�;���(model matrix)
		glm::mat4 rightModelMat(1.0f);
		rightModelMat = glm::translate(rightModelMat, glm::vec3(1.0f, -0.9f, 0.0f));
		rightModelMat = glm::rotate(rightModelMat, glm::radians(currentFrame * 50), glm::vec3(0.0f, 1.0f, 0.0f));
		rightModelMat = glm::scale(rightModelMat, glm::vec3(0.1f));
		glm::mat3 rightNormalMat = glm::mat3(glm::transpose(glm::inverse(rightModelMat)));

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

		// ģ�ͻ���
		{
			//glStencilFunc(GL_ALWAYS, 1, 0xFF);	// ����ͨ��ģ�建��
			//glStencilMask(0xFF);				// glStencilOp��Ч���ڻ��Ƶĵط���ģ�建���ֵ����Ϊ1

			// ����ն�����ɫ��������VP�������λ��
			modelShader.use();
			modelShader.setMat4("projection", projection);
			modelShader.setMat4("view", view);
			modelShader.setVec3("cameraPos", camera.GetCameraPosition());

			modelShader.setMat4("model", leftModelMat);
			modelShader.setMat3("NormalMat", leftNormalMat);
			//AyakaModel.Draw(modelShader);
			//ThomaModel.Draw(modelShader);
			LaSignoraModel.Draw(modelShader);

			modelShader.setMat4("model", rightModelMat);
			modelShader.setMat3("NormalMat", rightModelMat);
			//GanyuModel.Draw(modelShader);
			//HuTaoModel.Draw(modelShader);
			scaramoucheModel.Draw(modelShader);
		}

// 		// �߿����
// 		{
// 			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);// ��ģ�建��ֵ��Ϊ1�ĵط�����
// 			glStencilMask(0x00);				// glStencilOpʧЧ��������ģ�建��
// 			glDisable(GL_DEPTH_TEST);			// �ر���Ȳ���
// 
// 			frameShader.use();
// 			frameShader.setMat4("projection", projection);
// 			frameShader.setMat4("view", view);
// 
// 			// ���ģ�ͱ߿�
// 			leftModelMat = glm::scale(leftModelMat, glm::vec3(1.01f));
// 			frameShader.setMat4("model", leftModelMat);
// 			LaSignoraModel.Draw(frameShader);
// 
// 			// �Ҳ�ģ�ͱ߿�
// 			rightModelMat = glm::scale(rightModelMat, glm::vec3(1.01f));
// 			frameShader.setMat4("model", rightModelMat);
// 			scaramoucheModel.Draw(frameShader);
// 
// 			// ��ģ�建�����㣬Ϊ��һ����Ⱦ��׼����
// 			glStencilMask(0xFF);
// 			glStencilFunc(GL_ALWAYS, 0, 0xFF);
// 			glEnable(GL_DEPTH_TEST);
// 		}

		// ��պл���
		{
			glDepthFunc(GL_LEQUAL);	// ��Ȳ���ͨ���Ż�����պ�
			skyboxShader.use();

			glm::mat4 ModelMat(1.0f);
			ModelMat = glm::scale(ModelMat, glm::vec3(10.0f));
			skyboxShader.setMat4("projection", projection);
			skyboxShader.setMat4("view", view);
			skyboxShader.setMat4("model", ModelMat);

			glBindVertexArray(skyboxVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glDepthFunc(GL_LESS);
		}

		glfwSwapBuffers(window);				// ����ָ�����ڵ�ǰ��˻�����
		glfwPollEvents();						// �������й����¼�
	}

	// �ͷš�ɾ��֮ǰ�����������Դ
	glDeleteVertexArrays(21, &lightVAO);
	glDeleteBuffers(1, &lightVBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);

	//lightshader.deleteProgram();
	modelShader.deleteProgram();
	skyboxShader.deleteProgram();

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
		if (nrComponents == 1) format = GL_RED;
		else if (nrComponents == 3) format = GL_RGB;
		else if (nrComponents == 4) format = GL_RGBA;

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

unsigned int LoadCubeMap(const vector<string>& faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrComponents;
	for (unsigned int i = 0; i < faces.size(); ++i)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1) format = GL_RED;
			else if (nrComponents == 3) format = GL_RGB;
			else if (nrComponents == 4) format = GL_RGBA;
						
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
				0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

