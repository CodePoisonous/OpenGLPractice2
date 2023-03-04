#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void frambuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// ���ݴ��ڵĴ�Сʵʱ�����ӿڵĴ�С
	glViewport(0, 0, width, height);
}

void proccessInput(GLFWwindow* window)
{
	// �û�����esc�����رմ���
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
    glfwInit();     // ��ʼ��glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // �������汾��
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // ���ôΰ汾��
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // ʹ�ú���ģʽ

	// ����glfw����
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenglPractice", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);	// ���ڵ�����������Ϊ��ǰ�̵߳���������

	// ��ʼ��glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// �����ӿ�
	glViewport(0, 0, 800, 600);	
	glfwSetFramebufferSizeCallback(window, frambuffer_size_callback);

	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		// ��ؼ��̰�������
		proccessInput(window);		

		// ��Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// �������������Ļ����ɫ
		glClear(GL_COLOR_BUFFER_BIT);			// �����ɫ���壬���glClearColor�����õ���ɫ

		glfwSwapBuffers(window);	// ����ָ�����ڵ�ǰ��˻�����
		glfwPollEvents();			// �������й����¼�
	}

	// �ͷš�ɾ��֮ǰ�����������Դ
	glfwTerminate();
    return 0;
}