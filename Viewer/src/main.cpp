#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>
#include <iostream>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"

/**
 * Fields
 */
bool show_demo_window = false;
bool show_another_window = false;
bool Tm = false, Sm = false, Rm = false;
bool Tw = false, Sw = false, Rw = false;
bool facecheck = false;
bool vertexcheck = false;
bool rtxm = false,rtym = false, rtzm = false;
bool rtxw = false,rtyw = false, rtzw = false;


glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);

/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char **argv)
{
	int windowWidth = 1280, windowHeight = 720;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();
	
	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
		StartFrame();
		DrawImguiMenus(io, scene);
		RenderFrame(window, scene, renderer, io);
    }

	Cleanup(window);
    return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	#if __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	
	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	
	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		// TODO: Set new aspect ratio
	}

	if (!io.WantCaptureKeyboard)
	{
		// TODO: Handle keyboard events here
		if (io.KeysDown[65])
		{
			// A key is down
			// Use the ASCII table for more key codes (https://www.asciitable.com/)
		}
	}

	if (!io.WantCaptureMouse)
	{
		// TODO: Handle mouse events here
		if (io.MouseDown[0])
		{
			// Left mouse button is down
		}
	}

	renderer.ClearColorBuffer(clear_color);
	renderer.Render(scene);
	renderer.SwapBuffers();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	/**
	 * MeshViewer menu
	 */
	ImGui::Begin("MeshViewer Menu");
	
	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene.AddModel(Utils::LoadMeshModel(outPath));
					free(outPath);
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}

			}
			ImGui::EndMenu();
		}

		// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
	}

	// Controls
	ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
	// TODO: Add more controls as needed
	
	ImGui::End();

	/**
	 * Imgui demo - you can remove it once you are familiar with imgui
	 */
	
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f, ft = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);
		//ImGui::SliderFloat("transformation ", &ft, 0.0f, 1.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	if(scene.GetModelCount())
	{
		
		const char* listbox_items[] = {"local transformation","world transformation"};
		const char* listbox_rotations[] = { "rotate around x","rotate around y","rotate around z" };
		static int listbox_item_current = 0;
		static int current_rotation = 0;
		static int Counterbox = 0, facenormals_counter = 0, vernormals_counter = 0;

		int scale = 0, trans = 0, rotate = 0;
		
		ImGui::Begin("transformation");
	    ImGui::ListBox("select\n", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items),2);
		

		static float anglem = 0.0f;
		static float ax_m = 0.0f;
		static float lastax_m = 0.0f;
		static float ay_m = 0.0f;
		static float lastay_m = 0.0f;
		
		static float az_m = 0.0f;
		static float lastaz_m = 0.0f;


		static float anglew = 0.0f;
		static float ax_w = 0.0f;
		static float lastax_w = 0.0f;
		static float ay_w = 0.0f;
		static float lastay_w = 0.0f;

		static float az_w = 0.0f;
		static float lastaz_w = 0.0f;
		
		
		static float Tm_vec[3] = { 0.0f, 0.0f, 0.0f }; //translate vector to the modle, the vector contains x,y,z steps
		static float Tw_vec[3] = { 0.0f, 0.0f, 0.0f }; //translate vector to the world, the vector contains x,y,z steps
		static float Sm_vec[3] = { 1.0f, 1.0f, 1.0f }; //scale vector to the modle, the vector contains x,y,z steps
		static float Sw_vec[3] = { 1.0f, 1.0f, 1.0f };//scale vector to the world, the vector contains x,y,z steps
		
	


		
		if (listbox_item_current == 0) // local(modle)
		{
			
			ImGui::Checkbox("translation", &Tm);
			if (Tm)
			{

				ImGui::InputFloat("tranlation x steps", &Tm_vec[0]);
				ImGui::InputFloat("tranlation y steps", &Tm_vec[1]);
				ImGui::InputFloat("tranlation z steps", &Tm_vec[2]);

				scene.GetActiveModel().Set_Tm_mat(glm::translate(glm::vec3(Tm_vec[0], Tm_vec[1], Tm_vec[2])));


			}
			ImGui::Checkbox("Rotation", &Rm);
			if (Rm)
			{
				ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "select any rotation:\n");
				ImGui::ListBox("", &current_rotation, listbox_rotations, IM_ARRAYSIZE(listbox_rotations), 3);

				ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "set the angle:\n");
				
				if (current_rotation==0) //around x
				{
					ImGui::SliderAngle("slider angle", &ax_m, -360, 360);
					
					if (lastax_m != ax_m)
					{
						lastax_m = ax_m;
						scene.GetActiveModel().Set_Rm_mat(glm::rotate(ax_m, glm::vec3(1, 0, 0)),0);
						scene.GetActiveModel().Set_transmatrix();
					}

					
					
				}
				if (current_rotation == 1)// around y
				{
					ImGui::SliderAngle("slider angle", &ay_m, -360, 360);
					if (lastay_m != ay_m)
					{
						lastay_m = ay_m;
						scene.GetActiveModel().Set_Rm_mat(glm::rotate(ay_m, glm::vec3(0, 1, 0)),1);
						scene.GetActiveModel().Set_transmatrix();
					}
				}
				if (current_rotation == 2) // around z
				{
					ImGui::SliderAngle("slider angle", &az_m, -360, 360);

					if (lastaz_m != az_m)
					{
						lastaz_m = az_m;
						scene.GetActiveModel().Set_Rm_mat(glm::rotate(az_m, glm::vec3(0, 0, 1)), 2);
						scene.GetActiveModel().Set_transmatrix();
					}
				
				
				}

			}
			ImGui::Checkbox("scale", &Sm);
			
			auto model = scene.GetActiveModel();
			
			if (Sm)
			{

				ImGui::InputFloat("X", &Sm_vec[0]);
				ImGui::InputFloat("Y", &Sm_vec[1]);
				ImGui::InputFloat("Z", &Sm_vec[2]);

				scene.GetActiveModel().Set_Sm_mat(glm::scale(glm::vec3(Sm_vec[0], Sm_vec[1], Sm_vec[2])));
				
			}
			

		      
		}
		else //world
		{


			ImGui::Checkbox("translation", &Tw);
			if (Tw)
			{

				ImGui::InputFloat("tranlation x steps", &Tw_vec[0]);
				ImGui::InputFloat("tranlation y steps", &Tw_vec[1]);
				ImGui::InputFloat("tranlation z steps", &Tw_vec[2]);

				scene.GetActiveModel().Set_Tw_mat(glm::translate(glm::vec3(Tw_vec[0], Tw_vec[1], Tw_vec[2])));

			}
			ImGui::Checkbox("Rotation", &Rw);
			if (Rw)
			{

				ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "select any rotation:\n");
				ImGui::ListBox("", &current_rotation, listbox_rotations, IM_ARRAYSIZE(listbox_rotations), 3);

				ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "set the angle:\n");

				if (current_rotation == 0)
				{
					ImGui::SliderAngle("slider angle", &ax_w, -360, 360);

					if (lastax_w != ax_w)
					{
						lastax_w = ax_w;
						scene.GetActiveModel().Set_Rm_mat(glm::rotate(ax_w, glm::vec3(1, 0, 0)), 0);
						scene.GetActiveModel().Set_transmatrix();
					}



				}
				if (current_rotation == 1)
				{
					ImGui::SliderAngle("slider angle", &ay_w, -360, 360);
					if (lastay_w != ay_w)
					{
						lastay_w = ay_w;
						scene.GetActiveModel().Set_Rm_mat(glm::rotate(ay_w, glm::vec3(0, 1, 0)), 1);
						scene.GetActiveModel().Set_transmatrix();
					}
				}
				if (current_rotation == 2)
				{
					ImGui::SliderAngle("slider angle", &az_w, -360, 360);

					if (lastaz_w != az_w)
					{
						lastaz_w = az_w;
						scene.GetActiveModel().Set_Rm_mat(glm::rotate(az_w, glm::vec3(0, 0, 1)), 2);
						scene.GetActiveModel().Set_transmatrix();
					}


				}

			}
			
			ImGui::Checkbox("scale", &Sw);
			
			auto model = scene.GetActiveModel();
			if (Sw)
			{

				ImGui::InputFloat("X", &Sw_vec[0]);
				ImGui::InputFloat("Y", &Sw_vec[1]);
				ImGui::InputFloat("Z", &Sw_vec[2]);

				scene.GetActiveModel().Set_Sw_mat(glm::scale(glm::vec3(Sw_vec[0], Sw_vec[1], Sw_vec[2])));

			}
			
			

		}
		scene.GetActiveModel().Set_transmatrix();
		scene.GetActiveModel().Set_Reset(0);
		int reset = ImGui::Button("Reset");
		if (reset)
		{
			scene.GetActiveModel().Set_Reset(1);
			Tm = 0;
			Rm = 0;
			Sm = 0;
			Tw = 0;
			Rw = 0;
			Sw = 0;
			ax_m = 0;
			ay_m = 0;
			az_m = 0;
			ax_w = 0;
			ay_w = 0;
			az_w = 0;
			lastax_m = 0;
			lastay_m = 0;
			lastaz_m = 0;
			lastax_w = 0;
			lastay_w = 0;
			lastaz_w = 0;
			Tm_vec[0] = 0; 
			Tm_vec[1] = 0; 
			Tm_vec[2] = 0; 

			Tw_vec[0] = 0;
			Tw_vec[1] = 0;
			Tw_vec[2] = 0;

			Sw_vec[0] = 1;
			Sw_vec[1] = 1;
			Sw_vec[2] = 1;

			Sm_vec[0] = 1;
			Sm_vec[1] = 1;
			Sm_vec[2] = 1;

		   
			
		}
		

		

		if (ImGui::Button("Show bounding box"))
		{
			Counterbox++;
		}
		if ((Counterbox % 2) == 0)
		{
			scene.GetActiveModel().Set_showbox(0);
		}
		else
		{
			scene.GetActiveModel().Set_showbox(1);
		}
			
		
			
			if (ImGui::Checkbox("show face normals",&facecheck))
			{
				facenormals_counter++;
				if ((facenormals_counter % 2) == 0)
				{
					scene.GetActiveModel().Set_facenormals(0);
				}
				else
				{
					scene.GetActiveModel().Set_facenormals(1);
				}
			}
			
			if (ImGui::Checkbox("Show vertices normals", &vertexcheck))
			{
				vernormals_counter++;
				if ((vernormals_counter % 2) == 0)
				{
					scene.GetActiveModel().Set_vernormals(0);
				}
				else
				{
					scene.GetActiveModel().Set_vernormals(1);
				}
			}
			
		

		
		ImGui::End();

	}


}