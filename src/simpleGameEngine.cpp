#include <stdio.h>
#include <iostream>
#include <vector>
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
#include "mesh.h"
#include "logger.h"
#include "simpleGameEngine.h"
#include "camera.h"
#include "scene.h"


namespace Lynx {

bool Game::mouseLock;
int Game::polygonMode;
bool Game::keys[1024];
bool Game::debugMode;
int Game::activeScene;
float Game::pitch;
float Game::yaw;
float Game::lastX;
float Game::lastY;
double Game::mouseXPos, Game::mouseYPos;
bool Game::firstMouse;
std::vector<Scene*> Game::Scenes;

Game::Game(unsigned int width, unsigned int height):
	logger("main.log", LOG_DEBUG, false)
{

	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;

	initWindow();

}


Game::~Game(){
	OnLast();
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
	glfwTerminate();
}

void Game::SetDebugMode(bool mode){
	debugMode = mode;
}

void Game::initWindow(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  
  	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, windowName, NULL, NULL);
	if (window == NULL)
	{
	    printf("Failed to create window\n");
	    glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);  
	glfwSetCursorPosCallback(window, MouseCallback);  
	glfwSetKeyCallback(window, KeyCallback);

	bool err = glewInit() != GLEW_OK;   

    if(err){
        printf("Failed to initialize GLEW ! \n");
    }

	glEnable(GL_DEPTH_TEST);

	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    OnInit();


}

int Game::CreateScene(const char* name){
    Scenes.push_back(new Scene(name, &resourceManager));
    printf("Scene %s created\n", Scenes[Scenes.size()-1]->name);
    return Scenes.size()-1;
}

int Game::BindScene(Scene* scene){
    Scenes.push_back(scene);
    return Scenes.size()-1;
}

Scene* Game::GetActiveScene(){
    return Scenes[activeScene];
}

bool Game::SetActiveScene(int id){
    if(id <= (Scenes.size()-1)){
        activeScene = id;
        return true;
    }else{
        logger.log(LOG_ERROR, "Scene does not exist");
        return false;
    }
}

void Game::Run(){
	while((!glfwWindowShouldClose(window))|running)
	{
        float current_FrameTime = glfwGetTime();
		delta_time = current_FrameTime - last_FrameTime;
		last_FrameTime = current_FrameTime;
        glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
		
		OnUpdate();

		//ProcessInput();

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(debugMode){
			DebugWindow();
		}

		OnRender();
        Scenes[activeScene]->Render();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	    glfwSwapBuffers(window);  
	    glfwPollEvents();

    }
}


void Game::MouseCallback(GLFWwindow* window, double xpos, double ypos){
    /*
	if(!mouseLock){return;}
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
	Scenes[activeScene]->Cameras["penis"]->front = glm::normalize(direction);
    */
    mouseXPos = xpos;
    mouseYPos = ypos;
}

void Game::ProcessInput(GLFWwindow *window)
{	
	
	/*
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
    }*/
    

    	
    	
}

void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	

    if (key ==  GLFW_KEY_I && action == GLFW_PRESS){
        if(polygonMode<1){polygonMode++;}else{polygonMode = 0;}
    	//glPolygonMode(GL_FRONT_AND_BACK, polygonModes[polygonMode]);

    }
    if(key == GLFW_KEY_L && action == GLFW_PRESS){
    	if(mouseLock){
    		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    		mouseLock = false;
    	}else{
    		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    		mouseLock = true;
    	}
    }

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, true);



    if(key >= 0 && key < 1024){
    	if(action == GLFW_PRESS)
    		keys[key] = true;
    	else if(action == GLFW_RELEASE)
    		keys[key] = false;

    }

    if(keys[GLFW_KEY_LEFT_SHIFT]&&keys[GLFW_KEY_LEFT_CONTROL]&&keys[GLFW_KEY_F1]){
    	if(debugMode){debugMode = false;}else{debugMode = true;}
    }

    if(keys[GLFW_KEY_LEFT_SHIFT]&&keys[GLFW_KEY_LEFT_CONTROL]&&keys[GLFW_KEY_F2]){
    	if(mouseLock){
    		mouseLock = false; 
    		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    	}else{
    		mouseLock = true; 
    		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    	}
    }


}

void Game::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void Game::DebugWindow(){
	ImGui::Begin("Objects");
    //ImGui::Text("Current Scene : Scene #%d ( %s ) ", activeScene, Scenes[activeScene]->name);  
	//ImGui::Text("FPS : %d", (int)round(1/delta_time));
    
    if(Scenes[activeScene]->Sprites.size()>0){
        ImGui::Text("Sprites");
        for(const auto &spr : Scenes[activeScene]->Sprites){
            if (ImGui::Button(spr.first))
            {
                selectedType = 1;
                selectedSprite = spr.second;
                selectedName = spr.first;
            }
        }
    }
    
    if(Scenes[activeScene]->Cameras.size()>0){
        ImGui::Separator();
        ImGui::Text("Cameras");
        for(const auto &cam : Scenes[activeScene]->Cameras){
            if (ImGui::Button(cam.first))
            {
                selectedType = 2;
                selectedCamera = cam.second;
                selectedName = cam.first;
            }
        }
    }

    if(Scenes[activeScene]->Objects3D.size()>0){
        ImGui::Separator();
        ImGui::Text("3D Meshes");
        for(const auto &obj : Scenes[activeScene]->Objects3D){
            if (ImGui::Button(obj.first))
            {
                selectedType = 3;
                selectedMesh3D = obj.second;
                selectedName = obj.first;
            }
        }
    }
    ImGui::Separator();
    ImGui::Text("Resources");

    for(const auto &shdr : resourceManager.ShaderMap){
        if (ImGui::Button(shdr.first))
        {
            selectedType = 4;
            selectedShader = shdr.second;
            selectedName = shdr.first;
        }
    }
    ImGui::End();

    ImGui::Begin("Inspector");

    if(selectedType == 1){
        ImGui::Text("Selected : %s",  selectedName);
        ImGui::Text("Type : 2D Sprite");

        ImGui::Text("XYZ : ");
        ImGui::SameLine();
        ImGui::InputFloat3("##1",glm::value_ptr(selectedSprite->pos));
        ImGui::Text("Colour : ");
        ImGui::SameLine();
        ImGui::SliderFloat3("##2", glm::value_ptr(selectedSprite->color), 0.0f, 1.0f);
    }else if(selectedType == 2){
        ImGui::Text("Selected : %s", selectedName);
        ImGui::Text("Type : Camera");
        ImGui::Text("XYZ : ");
        ImGui::SameLine();
        ImGui::InputFloat3("##1",glm::value_ptr(selectedCamera->pos));
        ImGui::Text("Front (ang) : ");
        ImGui::SameLine();
        ImGui::InputFloat3("##2",glm::value_ptr(selectedCamera->front));
    }else if(selectedType == 3){
        ImGui::Text("Selected : %s", selectedName);
        ImGui::Text("Type : Mesh 3D");
        ImGui::Text("XYZ : ");
        ImGui::SameLine();
        ImGui::InputFloat3("##1",glm::value_ptr(selectedMesh3D->pos));
    }else if(selectedType == 4){
        ImGui::Text("Selected : %s", selectedName);
        ImGui::Text("Type : Shader");
        int attrib_count, uniform_count;
        glGetProgramiv(selectedShader->getProgram(), GL_ACTIVE_ATTRIBUTES, &attrib_count);
        glGetProgramiv(selectedShader->getProgram(), GL_ACTIVE_UNIFORMS, &uniform_count);
        ImGui::Text("Active attributes ( %d ) : ", attrib_count);
        ImGui::Text("Active uniforms ( %d ) : ", uniform_count);
        
        if (ImGui::CollapsingHeader("Attributes"))
        {
            
            for (int i = 0; i < attrib_count; i++)
            {

                int count;
                int length;
                GLsizei size;
                GLenum type;
                char name[512];
                glGetActiveAttrib(selectedShader->getProgram(), (GLuint)i, 512, &length, &size, &type, name);
                if (ImGui::TreeNode("%s##%d", name, i))
                {
                    ImGui::Text("Name %s ", name);
                    ImGui::Text("Type %u ", type);
                    ImGui::TreePop();
                }
            }
        }
    }

    ImGui::End();

}

}
