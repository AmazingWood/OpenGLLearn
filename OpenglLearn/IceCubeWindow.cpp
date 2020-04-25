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

	glEnable(GL_DEPTH_TEST);

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
	OGLShader modelShader("./model_vertex.glsl", "./model_fragment.glsl", "modelShader");
	IceCubeModelLoader ourModel("./assets/nanosuit/nanosuit.obj");

	//loop the windows!
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.05f, 0.3f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//******************************************************should be delete
		modelShader.activateProgram("modelShader");

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800.0 / (float)600.0, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 4.5f), glm::vec3(0.0f, 0.0f, 0.0f) + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelShader.setMat4("modelShader", "projection", projection);
		modelShader.setMat4("modelShader", "view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene 
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		modelShader.setMat4("modelShader", "model", model);

		ourModel.drawMeshes(modelShader, "modelShader");
		//******************************************************should be delete

		
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
