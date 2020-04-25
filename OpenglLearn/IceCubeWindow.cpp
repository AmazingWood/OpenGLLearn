#include "IceCubeWindow.h"

IceCubeWindow::IceCubeWindow(int windowWidth, int windowHeight, std::string windowName)
{
	//basic info init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//create window point
	window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), NULL, NULL);
	if (window == NULL)
	{
		throw std::logic_error("Failed to create GLFW window");
	}
	glfwMakeContextCurrent(window);
	//glad to manage the point
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::logic_error("Failed to initialize GLAD");
	}
	glViewport(0, 0, windowWidth, windowHeight);

	/*
	*
	*	we should deal with the callback list at here, just like this:
	*   reg the callback to resize the window
	*   glfwSetFramebufferSizeCallback(window, OGLCallBack::framebuffer_size_callback);
	*
	*/

}

void IceCubeWindow::IceCubeDisplay()
{

	IceCubeModelLoader ourModel("assets/nanosuit/nanosuit.obj");

	//loop the windows!
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*
		*
		*	insert a function, to transport the model which we need to display in the window.
		*
		*/

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

IceCubeWindow::~IceCubeWindow()
{
	//always remember to destroy what u create in ur destructor
	glfwDestroyWindow(window);
	glfwTerminate();
}
