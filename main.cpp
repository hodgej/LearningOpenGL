#include <glad.h>

#include <khrplatform.h>

#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.5f, 0.2f, 1.0f, 1.0f);\n"
"}\n\0";


int main() {
	// Init GLFW with current GLFW version and Core profile
	std::cout << "Init GLFW...";
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// init new window and attach to context in this thread
	GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW Test", NULL, NULL);
	std::cout << "Create window...";
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
	std::cout << "Setting up GLAD...";
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to load GLAD" << std::endl;
		return -1;
	}

	/*
	* Drawing a triangle
	
		* The Process below generalized:

		// ..:: Initialization code (done once (unless your object frequently changes)) :: ..

			// 1. bind Vertex Array Object
			glBindVertexArray(VAO);
			// 2. copy our vertices array in a buffer for OpenGL to use
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			// 3. then set our vertex attributes pointers
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);


			[...]

		// ..:: Drawing code (in render loop) :: ..
			// 4. draw the object
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			someOpenGLFunctionThatDrawsOurTriangle();



	*/

	std::cout << "Generating Image...";
	// Creating our two required shaders:
	// Vertex Shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // Creates an empty shader object behind the scenes and returns a reference ID to it
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // replaces source code of a shader object
	glCompileShader(vertexShader); // Compiles shader object with the attached source of the shader object

	// Check for success
	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

	}

	//Fragment Shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, & fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	// Shader program
	// Links our shaders into the complete shader cycle.
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram(); // creates a shader program and returns an ID 
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glUseProgram(shaderProgram); // following draw calls will now use our shader program

	glDeleteShader(fragmentShader); // no longer need our shader objects
	glDeleteShader(vertexShader);






	/*
	To draw a rectangle like this, we would need to use these vertices:
	float vertices[] = {
	// first triangle
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f,  0.5f, 0.0f,  // top left
	// second triangle
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left
};
	This is a lot of overhead, with 50% of points being duplicates.
	This can be solved using an Element Buffer Object, which allows us to specify the order in which points are placed, and thus eliminate the duplicate points for the rectanglee. 

	
	
	*/

	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};

	unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
	};

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	unsigned int VBO; // buffer id int for our VBO - Vertex Buffer Object. Differnet VBO's could store different types of vertex data (eg. 1 for color, 1 for pos), or not. Just saying.
	/*
		A VAO allows storing vertex attribute configurations so calls to configure vertex attributes don't have to be needlessly repeated

		// Imagine we have 5 vertex attributes (5 attributes per vertex dude) and 100s of objects; configring all these attributes per object quickly becomes difficult.
		// // What if there were a way to store all these state configurations into an object?
		// using a VAO (Vertex array object) can prevent having to repeat this process needlessly
		// any subsequent vertex attribute calls will be stored inside the vao

		* A VAO Stores:
			- Calls to glEnableVertexAttribArray or glDisableVertexAttribArray.
			- Vertex attribute configurations via glVertexAttribPointer.
			- Vertex buffer objects associated with vertex attributes by calls to glVertexAttribPointer.

		Now when we want to draw an object, we only have to bind the corresponding VAO. This makes switching between different vertex data/attribute configs as easy as binding a different VAO.

		The VAO Stores STATE

	
	*/
	unsigned int VAO; // id for our VAO - Vertex Array Object
	// generates a VAO behind the scenes and returns a reference ID to it. 
	glGenVertexArrays(1, &VAO); // argument 1 specifies how many VAOs we want to generate. obv just one here.
	glBindVertexArray(VAO); // Bind the Vertex Array Object first. 

	glGenBuffers(1, &VBO); // generate new buffer and assigns VBO it's reference ID (int). the "1" means only one buffer id is being generated.
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind GL_ARRAY_BUFFER to our vertex buffer. You can only have one buffer bound to GL_ARRAY_BUFFER at a time. (You can only work with them one at a time, but that's not to say you can't have multiple buffer objects working with the array buffer)
										// Now GL_ARRAY_BUFFER is associated with our buffer of if VBO
	// copy our vertex data into the buffer's memory; this puts our data in GPU memory as managed by our buffer VBO
	// GL_STATIC Draw is good for data that is set only once and used many times.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Tell OpenGL how to interpret the vertex data per vertex attribute
	// A vertex attribute could be something like POSITION, COLOR, HUE, ETC.
	// In this case we are adjust vertex attribute 0 (location=0 in our shader), which is POSITION.
	// Position has three floats, thus a stride of 3*4 bits within our VBO.
	// This tells OpenGL How to access data for the attribute of Position per vertex. 
	// **** This creates an ASSOCIATION between the currently bound VAO and VBO for future draw calls.
		// This creates a stored association between the current VBO and VAO in the VAO's state. 
	// The VAO "remembers" which VBO corresponds with a vertex attribute. 
		// each vertex attribute corresponds to a VBO
	// These associations are made upon the glVertexAttribPointer call. 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // p1: which vertex atribute we want to configure.  (location=0 is in vertex shader too), p2: size of the vertex attribute (vec3; 3 values)
								// p3: type of data (GL_FLOAT is type of vec in GLSL), p4: specifies if normalize or not. 
								// p5: stride; tells us space between consequtive vertex attributes; 3 floats each means 3*4(bits) each. When our array is tightly packed we can specify 0 and have opengl do this for us.
								// p6: offset of where position data begins in the buffer. 0 for now
	// Our VAO maintains an array of vertex attribute pointers. Each index corresponds to a vertex atribute. 
	// These are the specifications for how to interpret VBO data. 
	glEnableVertexAttribArray(0); // enable that vertex attribute of POSITION

	// iunbind VAO here, before unbinding your EBO (unbinding your EBO before unbinding VAO would remove the association between the two)
	glBindBuffer(GL_ARRAY_BUFFER, 0);// we can safely unbind as the call to glVertexAttribPointer registered VBO as the vertex attribute's BOUND vertex buffer object.  
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	

	std::cout << "Done!" << std::endl;
	while (!glfwWindowShouldClose(window)) {
		// Each iteration of this loop is called a frame

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state-setting function to set "clear" color
		glClear(GL_COLOR_BUFFER_BIT); // fill the color buffer with the clear color (

		glUseProgram(shaderProgram);
		// Bind VAO
		glBindVertexArray(VAO); // note the VAO is linked to it's relevant VBO. This link is created when glVertexAttribPointer is called. 
		// [NOTE: this is from before i used an EBO] draws based on associated VBO data given the current Vector Array Object (VAO).

		// we are drawing from elements rather than from array with glDrawArrays, which we would without an EBO
		// we thus need an EBO bound to GL_ELEMENT_ARRAY_BUFFER, which is the EBO that would be used.
		// luckily this ISN'T necessary here because our VAO has an EBO related with it, because we binded a new EBO to the buffer within the bounds of our VAO 
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);




		glfwSwapBuffers(window);  // swap the color buffer and render it's output on screen
						   // Two buffers are used; one for the current image, and one for the next;
						  // This is to avoid flickering, as the image is rendered left-to-right. This allows you do render out of view while the other buffer displays the previous frame, then switch when ready.
		glfwPollEvents();// check for input and call any registered callback functions 
		processInput(window);
		
	}

	glfwTerminate(); // cleans up glfw resources
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
