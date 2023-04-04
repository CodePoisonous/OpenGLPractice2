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


// 函数声明
void frambuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void proccessInput(GLFWwindow* window);

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

int main()
{
	// 初始化glfw
	{
		glfwInit();
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

	// 生成着色器程序
	Shader shaderProgram("src/shadersource/VertexShaderSource.glsl", "src/shadersource/FragmentShaderSource.glsl");

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

	// 生成VAO、VBO、EBO
	unsigned int vao, vbo, ebo;
	{
		// 生成对象
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		//glGenBuffers(1, &ebo);

		// 绑定对象
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		// 设置顶点数组标准化设备坐标(Normalized Device Coordinates)
		// 设置每个顶点对应的颜色信息
		float vertices[] = {	
			// 立方体坐标		// 纹理坐标
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

		// 设置顶点属性指针
		{
			// 位置属性
			glVertexAttribPointer(
				0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0); // 启用顶点属性（对应vertexShaderSource中的location = 0）

			// 纹理属性
			glVertexAttribPointer(
				1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1); // 启用顶点属性（对应vertexShaderSource中的location = 1）
		}

		//// 设置索引数组
		//unsigned int indices[] = {		
		//	0, 1, 3,	// 第一个三角形
		//	1, 2, 3,	// 第二个三角形
		//};
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// 解绑vbo和vao（防止被意外修改）
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	// 生成一个纹理(texture)
	unsigned int texture1;
	{
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		
		// 设置环绕方式和过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	  // X,Y轴纹理环绕方式为重复
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 放大，缩小时纹理过滤方式为线性插值
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// 加载并生成纹理
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

		// 设置环绕方式和过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	  // X,Y轴纹理环绕方式为重复
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 放大，缩小时纹理过滤方式为线性插值
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// 加载并生成纹理
		int width, height, nrchannels;
		stbi_set_flip_vertically_on_load(true);	// 翻转y轴方向
		unsigned char* data = stbi_load("src/texture/awesomeface.png", &width, &height, &nrchannels, 0);
		if (data)
		{
			// png图片有alpha值，得设置成GL_RGBA
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture2" << std::endl;
		}

		stbi_image_free(data);
	}


	// 激活着色器
	shaderProgram.use();
	shaderProgram.setInt("Texture1", 0);
	shaderProgram.setInt("Texture2", 1);

	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// 监控键盘按键输入
		proccessInput(window);

		// 渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// 设置用来清空屏幕的颜色
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 清除颜色\深度缓冲

		// 激活并绑定纹理单元
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		
		// 激活着色器程序
		shaderProgram.use();

		// 创建投影矩阵(project matrix) 		
		glm::mat4 projection(1.0f);
		{
			projection = glm::perspective(glm::radians(camera.Zoom),
				(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			shaderProgram.setMatrix4fv("projection", glm::value_ptr(projection));
		}

		// 创建观察矩阵(view matrix)
		glm::mat4 view(1.0f);
		{
			view = camera.GetViewMatrix();
			shaderProgram.setMatrix4fv("view", glm::value_ptr(view));
		}

		// 绘制10个立方体
		glBindVertexArray(vao);
		for (unsigned int i = 0; i < 10; ++i)
		{
			// 创建模型矩阵(model matrix)
			glm::mat4 model(1.0f);

			// 让模型移动到指定的世界坐标系位置
			model = glm::translate(model, cubePositions[i]);

			// 让模型以不同的速度绕局部坐标的x轴旋转
			model = glm::rotate(model,
				currentFrame * glm::radians(25.0f) * (i+1), glm::vec3(0.5f, 1.0f, 0.0f));

			// 转换矩阵传入着色器
			shaderProgram.setMatrix4fv("model", glm::value_ptr(model));

			// 绘制立方体
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// 绘制图元
		//glBindVertexArray(vao);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	// 按ebo规则绘制
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);				// 交换指定窗口的前后端缓冲区
		glfwPollEvents();						// 处理所有挂起事件
	}

	// 释放、删除之前分配的所有资源
	glDeleteVertexArrays(2, &vao);
	glDeleteBuffers(2, &vbo);
	glDeleteBuffers(1, &ebo);
	shaderProgram.deleteProgram();
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
}