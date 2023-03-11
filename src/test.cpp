#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// 函数声明
void frambuffer_size_callback(GLFWwindow* window, int width, int height);
void proccessInput(GLFWwindow* window);

// 窗口大小
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 顶点着色器源码
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

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


	// 创建和编译着色器程序
	// 顶点着色器
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	{
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		// 判断是否成功
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		if (!success)
		{			
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				<< infoLog << std::endl;
		}
		else
		{
			std::cout << "INFO::SHADER::VERTEX::COMPILATION_SUCCESS\n"
				<< infoLog << std::endl;
		}
	}


	// 生成一个顶点缓冲对象(vertex buffer object, VBO)
	unsigned int vbo;
	{
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);	// 将对象绑定到缓冲类型

		// 设置顶点数组标准化设备坐标(Normalized Device Coordinates)
		float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f,
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// 将顶点数据传入
	}

	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		// 监控键盘按键输入
		proccessInput(window);		

		// 渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// 设置用来清空屏幕的颜色
		glClear(GL_COLOR_BUFFER_BIT);			// 清楚颜色缓冲，填充glClearColor所设置的颜色

		glfwSwapBuffers(window);	// 交换指定窗口的前后端缓冲区
		glfwPollEvents();			// 处理所有挂起事件
	}

	// 释放、删除之前分配的所有资源
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