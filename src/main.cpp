#include <stdio.h>
#include <math.h>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#include "shader.h"
#include "texture.h"
#include "vertexArray.h"
#include "vertexBuffer.h"
#include "camera.h"
#include "renderer.h"	


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void gui_createWindow();

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

float delta_time = 0.0f;
float last_FrameTime = 0.0f;
float camera_Speed_Multiplier = 1.0f;

float lastX = 400, lastY = 300;
float pitch, yaw;
bool firstMouse = true;
bool lockMouse = true;

GLint polygonModes[] = {
	GL_FILL,
	GL_LINE
};
int polygonMode;
int textures;

Camera camera(SCR_WIDTH, SCR_HEIGHT);

int main()
{
	glfwSetErrorCallback(glfw_error_callback);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  
  	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Game engine", NULL, NULL);
	if (window == NULL)
	{
	    printf("Failed to create window\n");
	    glfwTerminate();
	    return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
	glfwSetCursorPosCallback(window, mouse_callback);  
	glfwSetKeyCallback(window, key_callback);

	/*
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
	    printf("Failed to load GLAD\n");
	    return -1;
	} 
	*/

	bool err = glewInit() != GLEW_OK;   

	glEnable(GL_DEPTH_TEST);

	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

	// Texture loading

	Texture tex1("res/images/box.jpg", &textures, GL_NEAREST, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	// Create shader
	
	/*

	//Textured Cube

	float vertices[] = {

	    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};*/
	/*
	// Non textured cube

	float vertices[] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };
    */
    // Non-Textured cube with normal vectors for diffuse lightning 
    float vertices[] = {
	    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
	     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
	     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
	    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
	    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

	    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	glm::vec3 cubePositions[] = {
	    glm::vec3( 0.0f,  0.0f,  0.0f),
	    glm::vec3(1.2f, 1.0f, 2.0f)
	};
	/*
	unsigned int VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1,	&EBO);

	glBindVertexArray(VAO);

	// Copies triangle's vertices into the vertex buffer's memory 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	*/

	// Renderer, VBO and VAO's setup

	Renderer renderer;

	VertexBuffer VBO(vertices, sizeof(vertices));

	VertexArray VAO;
	VAO.Bind();

	camera.pos = glm::vec3(0.0f,0.0f,3.0f);
	camera.front = glm::vec3(0.0f,0.0f,-1.0f);
	camera.up = glm::vec3(0.0f, 1.0f, 0.0f);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	
	

	/*
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	*/

	Shader shader1("res/shaders/diffuse.vs", "res/shaders/diffuse.fs");
    Shader shader2("res/shaders/lamp_cube.vs", "res/shaders/lamp_cube.fs");


	while(!glfwWindowShouldClose(window))
	{
		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

		// Get current frametime and calculate deltaTime
		float current_FrameTime = glfwGetTime();
		delta_time = current_FrameTime - last_FrameTime;
		last_FrameTime = current_FrameTime;
		glfwPollEvents();
		// input
		
	    processInput(window);

	    ImGui::Begin("Lightning example");  
		ImGui::Text("FPS : %f", 1/delta_time);
		ImGui::End();
	   
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::mat4 view = camera.GetView();
		glm::mat4 projection = camera.GetProjection();

		

		VAO.Bind();
		for(unsigned int i = 0;i<(sizeof(cubePositions)/sizeof(glm::vec3));i++){
			glm::mat4 model = glm::mat4(1.0f);
			float modelX = sin(glfwGetTime()) * 4;
			float modelZ = cos(glfwGetTime()) * 4;
			glm::vec3 lightPos = glm::vec3(cubePositions[i]+glm::vec3(modelX, 0, modelZ));

			if(i%2){
				// Lighted cube	
				model = glm::translate(model,cubePositions[i]);
				shader1.use();
				shader1.setVec3("objectColor", 1.0f, 0.5f, 0.5f);
    			shader1.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    			shader1.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
    			glUniformMatrix4fv(glGetUniformLocation(shader1.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
				glUniformMatrix4fv(glGetUniformLocation(shader1.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
				glUniformMatrix4fv(glGetUniformLocation(shader1.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
				
			}else{
				// Lamp cube
				model = glm::translate(model,lightPos);
				shader2.use();
				glUniformMatrix4fv(glGetUniformLocation(shader2.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
				glUniformMatrix4fv(glGetUniformLocation(shader2.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
				glUniformMatrix4fv(glGetUniformLocation(shader2.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
			}
			
			
			renderer.draw(36);
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	    glfwSwapBuffers(window);  
	    glfwPollEvents();
	    
	}
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    shader1.destroy();
    shader2.destroy();
    glfwTerminate();
	return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	if(lockMouse){return;}
	if (firstMouse) // initially set to true
	{
	    lastX = xpos;
	    lastY = ypos;
	    firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.25f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	

	yaw   += xoffset;
	pitch += yoffset;  

	if(pitch > 89.0f)
	  pitch =  89.0f;
	if(pitch < -89.0f)
	  pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera.front = glm::normalize(direction);
}

void processInput(GLFWwindow *window)
{
	float cameraSpeed = 2.5f * delta_time * camera_Speed_Multiplier;
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.pos += cameraSpeed * camera.front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.pos -= cameraSpeed * camera.front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.pos -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.pos += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
    	camera_Speed_Multiplier = 3.0f;
    }else{
    	camera_Speed_Multiplier = 1.0f;
    }

    	
    	
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key ==  GLFW_KEY_I && action == GLFW_PRESS){
        if(polygonMode<1){polygonMode++;}else{polygonMode = 0;}
    	glPolygonMode(GL_FRONT_AND_BACK, polygonModes[polygonMode]);

    }
    if(key == GLFW_KEY_L && action == GLFW_PRESS){
    	if(lockMouse){
    		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    		lockMouse = false;
    	}else{
    		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    		lockMouse = true;
    	}
    }
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void gui_createWindow(){

}