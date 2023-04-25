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


// 函数声明
void frambuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void proccessInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

// 窗口大小
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// 摄像机状态
Camera camera;
float lastX = SCR_WIDTH * 0.5;	// 鼠标在x轴前一帧的位置
float lastY = SCR_HEIGHT * 0.5;	// 鼠标在y轴前一帧的位置
bool firstMouse = true;	// 鼠标是否第一次进入窗口

// 记录没帧之间的时间差值
float deltaTime = 0.0f;	// 当前帧与上一帧的时间差
float lastFrame = 0.0f;	// 上一帧的时间

// 光源在世界坐标系下的位置
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	// 初始化glfw
	glfwInit();
	{		
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // 设置主版本号
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // 设置次版本号
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 使用核心模式
	}

	// 创建glfw窗口
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenglPractice", NULL, NULL);
	{
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);	// 窗口的上下文设置为当前线程的主上下文	
		glfwSetFramebufferSizeCallback(window, frambuffer_size_callback); // 调整窗口尺寸时同步调整视口尺寸
		glfwSetCursorPosCallback(window, mouse_callback);// 鼠标位置
		glfwSetScrollCallback(window, scroll_callback);	 // 滚轮转动

		// 程序运行时，隐藏掉光标
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	// 初始化glad: 加载所有OpenGL功能的指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 配置全局的Opengl状态
	glEnable(GL_DEPTH_TEST);	// 打开深度测试

	/*
	// 设定立方体在世界坐标系的位置
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

	// 设置多个点光源在世界坐标系的位置
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	// 设置顶点数组标准化设备坐标(Normalized Device Coordinates)
	// 设置每个顶点对应的颜色信息
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

	// 生成被照对象的VAO、VBO
	unsigned int objVAO, vbo;
	{
		glGenVertexArrays(1, &objVAO);
		glGenBuffers(1, &vbo);

		glBindVertexArray(objVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);				
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// 被照对象顶点位置数据解释
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// 解绑vbo和vao（防止被意外修改）
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	// 生成光源的VAO
	unsigned int lightVAO;
	{
		glGenVertexArrays(1, &lightVAO);
		glBindVertexArray(lightVAO);

		// 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// 光源立方体顶点位置数据解释
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// 解绑vbo和vao（防止被意外修改）
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	// 生成着色器程序
	Shader objshader("src/shadersource/VertexShaderSource.vs", "src/shadersource/FragmentShaderSource.fs");
	Shader lightshader("src/shadersource/VertexShaderSource.vs", "src/shadersource/LightFragmentShaderSource.fs");

	// 加载纹理图片
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

	// 外部导入的模型对象
	Model ourModel("src/modelsource/nanosuit/nanosuit.obj");

	// 外部导入模型的shader
	Shader modelShader("src/shadersource/model_loading_vs.glsl","src/shadersource/model_loading_fs.glsl");

	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// 监控键盘按键输入
		proccessInput(window);

		// 渲染
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// 设置用来清空屏幕的颜色
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 清除颜色\深度缓冲

		modelShader.use();

		// 投影矩阵(project matrix) 		
		glm::mat4 projection = glm::perspective(
			glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		modelShader.setMat4("projection", projection);

		// 观察矩阵(view matrix)
		glm::mat4 view = camera.GetViewMatrix();		
		modelShader.setMat4("view", view);

		// 模型矩阵
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		modelShader.setMat4("model", model);

		ourModel.Draw(modelShader);

		//glDrawArrays(GL_TRIANGLES, 0, 36);

		/*
		// 激活光源，设置光源属性
		lightshader.use();
		{
			lightshader.setMat4("projection", projection);
			lightshader.setMat4("view", view);
		}

		// 绘制4个光源
		glBindVertexArray(lightVAO);
		for (unsigned int i = 0; i < 4; ++i)
		{
			glm::mat4 lightmodel(1.0f);
			lightmodel = glm::translate(lightmodel, pointLightPositions[i]);
			lightmodel = glm::scale(lightmodel, glm::vec3(0.1f));	// 缩小光源体积
			lightshader.setMat4("model", lightmodel);
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// 激活被照对象着色器，设置属性
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

		// 绘制10个被照立方体
		glBindVertexArray(objVAO);
		for (unsigned int i = 0; i < 10; ++i)
		{
			// 创建模型矩阵(model matrix
			glm::mat4 model(1.0f);

			// 让模型移动到指定的世界坐标系位置
			model = glm::translate(model, cubePositions[i]);

			// 让模型以不同的速度绕局部坐标的x轴旋转
			float rad = currentFrame * glm::radians(10.0f) * (i + 1);
			model = glm::rotate(model, rad, glm::vec3(0.5f, 1.0f, 0.0f));

			// 转换矩阵传入着色器
			objshader.setMat4("model", model);

			// 法线矩阵
			glm::mat3 NormalMat = glm::mat3(glm::transpose(glm::inverse(model)));
			objshader.setMat3("NormalMat", NormalMat);

			// 绘制立方体
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		*/

		glfwSwapBuffers(window);				// 交换指定窗口的前后端缓冲区
		glfwPollEvents();						// 处理所有挂起事件
	}

	// 释放、删除之前分配的所有资源
	//glDeleteVertexArrays(2, &objVAO);
	//glDeleteBuffers(2, &vbo);
	//objshader.deleteProgram();
	modelShader.deleteProgram();
	glfwTerminate();
    return 0;
}

// 回调函数：根据窗口大小的变化调整视口的大小
void frambuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// 回调函数：鼠标移动
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float xPos = (float)xpos;
	float yPos = (float)ypos;

	// 第一次时初始化位置
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	// 计算两帧之间的鼠标位移 
	float xoffset = xPos - lastX;
	float yoffset = lastY - yPos;// 窗口坐标和opengl的y轴坐标是反的

	// 保存当前帧的位置
	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xoffset, yoffset, true);
}

// 回调函数：鼠标滚轮
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}

// 监控键盘按键的按下和松开
void proccessInput(GLFWwindow* window)
{	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);// 用户按下esc键，关闭窗口
	
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