#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void frambuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// 根据窗口的大小实时调整视口的大小
	glViewport(0, 0, width, height);
}

void proccessInput(GLFWwindow* window)
{
	// 用户按下esc键，关闭窗口
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
    glfwInit();     // 初始化glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // 设置主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // 设置次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 使用核心模式

	// 创建glfw窗口
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenglPractice", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);	// 窗口的上下文设置为当前线程的主上下文

	// 初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 设置视口
	glViewport(0, 0, 800, 600);	
	glfwSetFramebufferSizeCallback(window, frambuffer_size_callback);

	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		// 监控键盘按键输入
		proccessInput(window);		

		// 渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// 设置用来清空屏幕的颜色
		glClear(GL_COLOR_BUFFER_BIT);			// 清楚颜色缓冲，填充glClearColor所设置的颜色

		glfwSwapBuffers(window);	// 交换指定窗口的前后端缓冲区
		glfwPollEvents();			// 处理所有挂起事件
	}

	// 释放、删除之前分配的所有资源
	glfwTerminate();
    return 0;
}