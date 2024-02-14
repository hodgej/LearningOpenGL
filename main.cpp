#include <glad.h>

#include <khrplatform.h>

#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);


int main() {



	// Init GLFW with current GLFW version and Core profile
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// init new window and attach to context in this thread
	GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW Test", NULL, NULL);

	if (window == NULL) {
		std::cout << "Error creating window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//glViewport(0, 0, 800, 600); // can be smaller than window size
	// register backs after window is created and before render loop is instantiated.
	// callback function for window resize; calls framebuffer_size_callback w/necessary values
	
	// tell GLAD to load addresses of OS-specific functions before using OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to load GLAD" << std::endl;
		return -1;
	}

	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);  // swap the color buffer and render it's output on screen
						   // Two buffers are used; one for the current image, and one for the next;
						  // This is to avoid flickering, as the image is rendered left-to-right. This allows you do render out of view while the other buffer displays the previous frame, then switch when ready.
		glfwPollEvents();// check for input and call any registered callback functions 
		
	}

	glfwTerminate(); // cleans up glfw resources
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}