#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

// 函数声明
void frambuffer_size_callback(GLFWwindow* window, int width, int height);
void proccessInput(GLFWwindow* window);

// 窗口大小
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
	}

	// 初始化glad: 加载所有OpenGL功能的指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 生成着色器程序
	Shader shaderProgram("src/VertexShaderSource.glsl", "src/FragmentShaderSource.glsl");

	// 生成一个顶点数组对象(vertex array object, VAO)
	// 一个顶点缓冲对象(vertex buffer object, VBO)
	// 一个元素\索引缓冲数组(element\index buffer object, EBO\IBO)
	unsigned int vao, vbo, ebo;
	{
		// 生成对象
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		{
			// 绑定对象
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

			// 设置顶点数组标准化设备坐标(Normalized Device Coordinates)
			// 设置每个顶点对应的颜色信息
			float vertices[] = {
				// 位置				// 颜色
				 0.5f,  0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	// 右上角
				 0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	// 右下角
				-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	// 左下角
				-0.5f,  0.5f, 0.0f,	0.5f, 0.5f, 0.5f,	// 左上角
			};
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		// 将顶点数据传入

			// 设置顶点属性指针
			// 位置属性
			glVertexAttribPointer(
				0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);	// 解释顶点数据
			glEnableVertexAttribArray(0);	// 启用顶点属性（对应vertexShaderSource中的location = 0）

			// 颜色属性
			glVertexAttribPointer(
				1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));	// 解释顶点数据
			glEnableVertexAttribArray(1);	// 启用顶点属性（对应vertexShaderSource中的location = 1）
		}

		// 设置索引数组
		unsigned int indices[] = {
			// 索引是从0开始
			// 此例的索引值是顶点数组vertices的下标
			// 这样可以由下标代表顶点组合成矩形
			0, 1, 3,	// 第一个三角形
			1, 2, 3,	// 第二个三角形
		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// 解绑vbo和vao（防止被意外修改）
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	// 激活着色器
	shaderProgram.use();
	glBindVertexArray(vao);

	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		// 监控键盘按键输入
		proccessInput(window);

		// 渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// 设置用来清空屏幕的颜色
		glClear(GL_COLOR_BUFFER_BIT);			// 清楚颜色缓冲，填充glClearColor所设置的颜色

		// 绘制图元
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	// 按ebo规则绘制

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

// 监控键盘按键的按下和松开
void proccessInput(GLFWwindow* window)
{	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);// 用户按下esc键，关闭窗口
}