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

// 片段着色器源码
const char* fragmentShader1Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

const char* fragmentShader2Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
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

	// 生成一个顶点着色器对象，并编译顶点着色器源码
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	{
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		int success;	// 判断是否编译成功
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	// 生成片段着色器对象，并编译片段着色器源码
	unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	{
		{
			glShaderSource(fragmentShader1, 1, &fragmentShader1Source, NULL);
			glCompileShader(fragmentShader1);
			int success;	// 判断是否编译成功
			glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				char infoLog[512];
				glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::FRAGMENT1::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
		}

		{
			glShaderSource(fragmentShader2, 1, &fragmentShader2Source, NULL);
			glCompileShader(fragmentShader2);
			int success;	// 判断是否编译成功
			glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				char infoLog[512];
				glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::FRAGMENT2::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
		}
	}

	// 创建一个着色器程序
	unsigned int shaderProgram1 = glCreateProgram();
	unsigned int shaderProgram2 = glCreateProgram();
	{
		{
			glAttachShader(shaderProgram1, vertexShader);	// 链接顶点着色器
			glAttachShader(shaderProgram1, fragmentShader1);	// 链接片段着色器
			glLinkProgram(shaderProgram1);
			int success;	// 判断是否编译成功
			glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
			if (!success)
			{
				char infoLog[512];
				glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::PROGRAM1::LINK_FAILED\n" << infoLog << std::endl;
			}
		}

		{
			glAttachShader(shaderProgram2, vertexShader);	// 链接顶点着色器
			glAttachShader(shaderProgram2, fragmentShader2);	// 链接片段着色器
			glLinkProgram(shaderProgram2);
			int success;	// 判断是否编译成功
			glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
			if (!success)
			{
				char infoLog[512];
				glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::PROGRAM2::LINK_FAILED\n" << infoLog << std::endl;
			}
		}

		// 链接成功后即可删除
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader1);
		glDeleteShader(fragmentShader2);
	}

	// 生成一个顶点数组对象(vertex array object, VAO)
	// 一个顶点缓冲对象(vertex buffer object, VBO)
	// 一个元素\索引缓冲数组(element\index buffer object, EBO\IBO)
	unsigned int vaos[2], vbos[2], ebo;
	{
		// 生成对象
		glGenVertexArrays(2, vaos);
		glGenBuffers(2, vbos);
		glGenBuffers(1, &ebo);

		{
			// 绑定对象
			glBindVertexArray(vaos[0]);
			glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

			// 设置顶点数组标准化设备坐标(Normalized Device Coordinates)
			float vertices[] = {
				 0.7f,  0.5f, 0.0f,	// 右上角
				 0.7f,  0.0f, 0.0f,	// 右下角
				 0.3f,  0.0f, 0.0f,	// 左下角
				 0.3f,  0.5f, 0.0f,	// 左上角
			};
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		// 将顶点数据传入

			// 设置顶点属性指针
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// 解释顶点数据
			glEnableVertexAttribArray(0);	// 启用顶点属性
		}

		{
			// 绑定对象
			glBindVertexArray(vaos[1]);
			glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

			// 设置顶点数组标准化设备坐标(Normalized Device Coordinates)
			float vertices[] = {
				-0.3f,  0.5f, 0.0f,	// 右上角
				-0.3f,  0.0f, 0.0f,	// 右下角
				-0.7f,  0.0f, 0.0f, // 左下角
				-0.7f,  0.5f, 0.0f,	// 左上角
			};
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		// 将顶点数据传入

			// 设置顶点属性指针
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// 解释顶点数据
			glEnableVertexAttribArray(0);	// 启用顶点属性
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

	// 设置图元绘制方式
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// 线
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// 面

	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		// 监控键盘按键输入
		proccessInput(window);

		// 渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// 设置用来清空屏幕的颜色
		glClear(GL_COLOR_BUFFER_BIT);			// 清楚颜色缓冲，填充glClearColor所设置的颜色

		// 绘制三角形
		glUseProgram(shaderProgram1);			// 激活
		glBindVertexArray(vaos[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	// 按ebo规则绘制

		glUseProgram(shaderProgram2);			// 激活
		glBindVertexArray(vaos[1]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	// 按ebo规则绘制

		//glDrawArrays(GL_TRIANGLES, 0, 3);						// 直接绘制
		//glBindVertexArray(0);

		glfwSwapBuffers(window);				// 交换指定窗口的前后端缓冲区
		glfwPollEvents();						// 处理所有挂起事件
	}

	// 释放、删除之前分配的所有资源
	glDeleteVertexArrays(2, vaos);
	glDeleteBuffers(2, vbos);
	glDeleteBuffers(1, &ebo);
	glDeleteProgram(shaderProgram1);
	glDeleteProgram(shaderProgram2);
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