#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ��������
void frambuffer_size_callback(GLFWwindow* window, int width, int height);
void proccessInput(GLFWwindow* window);

// ���ڴ�С
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// ������ɫ��Դ��
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Ƭ����ɫ��Դ��
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
//"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"FragColor = ourColor;\n"
"}\0";


int main()
{
	// ��ʼ��glfw
	{
		glfwInit();
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
	}

	// ��ʼ��glad: ��������OpenGL���ܵ�ָ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ����һ��������ɫ�����󣬲����붥����ɫ��Դ��
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	{
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		int success;	// �ж��Ƿ����ɹ�
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	// ����Ƭ����ɫ�����󣬲�����Ƭ����ɫ��Դ��
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	{
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		int success;	// �ж��Ƿ����ɹ�
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT1::COMPILATION_FAILED\n" << infoLog << std::endl;
		}		
	}

	// ����һ����ɫ������
	unsigned int shaderProgram = glCreateProgram();
	{
		glAttachShader(shaderProgram, vertexShader);	// ���Ӷ�����ɫ��
		glAttachShader(shaderProgram, fragmentShader);	// ����Ƭ����ɫ��
		glLinkProgram(shaderProgram);
		int success;	// �ж��Ƿ����ɹ�
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM1::LINK_FAILED\n" << infoLog << std::endl;
		}

		// ���ӳɹ��󼴿�ɾ��
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	// ����һ�������������(vertex array object, VAO)
	// һ�����㻺�����(vertex buffer object, VBO)
	// һ��Ԫ��\������������(element\index buffer object, EBO\IBO)
	unsigned int vao, vbo, ebo;
	{
		// ���ɶ���
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		{
			// �󶨶���
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

			// ���ö��������׼���豸����(Normalized Device Coordinates)
			float vertices[] = {
				 0.5f,  0.5f, 0.0f,	// ���Ͻ�
				 0.5f, -0.5f, 0.0f,	// ���½�
				-0.5f, -0.5f, 0.0f,	// ���½�
				-0.5f,  0.5f, 0.0f,	// ���Ͻ�
			};
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		// ���������ݴ���

			// ���ö�������ָ��
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// ���Ͷ�������
			glEnableVertexAttribArray(0);	// ���ö�������
		}

		// ������������
		unsigned int indices[] = {
			// �����Ǵ�0��ʼ
			// ����������ֵ�Ƕ�������vertices���±�
			// �����������±��������ϳɾ���
			0, 1, 3,	// ��һ��������
			1, 2, 3,	// �ڶ���������
		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// ���vbo��vao����ֹ�������޸ģ�
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	// ����ͼԪ���Ʒ�ʽ
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// ��
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// ��

	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		// ��ؼ��̰�������
		proccessInput(window);

		// ��Ⱦ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// �������������Ļ����ɫ
		glClear(GL_COLOR_BUFFER_BIT);			// �����ɫ���壬���glClearColor�����õ���ɫ

		// ������ɫ��
		glUseProgram(shaderProgram);

		// ����uniform��ɫ
		float timeValue = (float)glfwGetTime();
		float greenValue = sin(timeValue);
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		// ����ͼԪ
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	// ��ebo�������

		//glDrawArrays(GL_TRIANGLES, 0, 3);						// ֱ�ӻ���
		//glBindVertexArray(0);

		glfwSwapBuffers(window);				// ����ָ�����ڵ�ǰ��˻�����
		glfwPollEvents();						// �������й����¼�
	}

	// �ͷš�ɾ��֮ǰ�����������Դ
	glDeleteVertexArrays(2, &vao);
	glDeleteBuffers(2, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
    return 0;
}

// �ص����������ݴ��ڴ�С�ı仯�����ӿڵĴ�С
void frambuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// ��ؼ��̰����İ��º��ɿ�
void proccessInput(GLFWwindow* window)
{	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);// �û�����esc�����رմ���
}