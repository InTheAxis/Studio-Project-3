#include "Application.h"

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Utility/WinDebug.h"
#include "Utility/Input/ControllerMouse.h"
#include "Utility/Input/ControllerKeyboard.h"
#include "Manager/MgrScene.h"

Application::Application()
{
}

Application::~Application()
{
}

void Application::Create()
{
	//set the error callback
	glfwSetErrorCallback(error_callback);

	//initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	//create a "borderless full-screen" window and create its OpenGL context
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	//create a window and create its OpenGL context
#ifdef _DEBUG
	m_window = glfwCreateWindow(m_window_width, m_window_height, "SP3", NULL, NULL);
#else
	m_window_height = mode->height;
	m_window_width = mode->height * 16.f / 9.f;
	m_window = glfwCreateWindow(m_width, m_height, "SP3", glfwGetPrimaryMonitor(), NULL);
#endif
	
	if (!m_window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//set windows position
	glfwSetWindowPos(m_window, Application::GetWindowHalfWidth() >> 1, Application::GetWindowHalfHeight() >> 1);

	//this function makes the context of the specified window current on the calling thread.
	glfwMakeContextCurrent(m_window);

	//sets callbacks needing window
	glfwSetWindowSizeCallback(m_window, resize_callback);
	glfwSetMouseButtonCallback(m_window, mouse_click_callback);
	glfwSetScrollCallback(m_window, mouse_scroll_callback);

	//hide the cursor
	//glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);	

	//needed for core profile
	glewExperimental = true; 
	//initialize GLEW
	GLenum err = glewInit();

	//if GLEW hasn't initialized
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}	

	//init managers
	MgrScene::Instance()->Start();
}

void Application::Run()
{		
	//SceneManager::GetInstance()->SetActiveScene("Start");
	m_timer.StartTimer();    // Start timer to calculate how long it takes to render this frame

	//// Init the FPSCounter
	//CFPSCounter::GetInstance()->Init();
	double deltaTime = 0.0;

	while (!glfwWindowShouldClose(m_window) && !ShouldQuit())
	{
		glfwPollEvents();
		CheckForInput();		

		deltaTime = m_timer.GetElapsedTime();
		//CFPSCounter::GetInstance()->Update(deltaTime);
		//SceneManager::GetInstance()->Update(deltaTime);
		//SceneManager::GetInstance()->Render();

		MgrScene::Instance()->Update(deltaTime);

		//Swap buffers
		glfwSwapBuffers(m_window);

		m_timer.WaitUntil(frameTime); // Frame rate limiter. Limits each frame to a specified time in ms.

		InputReset();
	}		
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
	//clean up managers
	MgrScene::Instance()->End();	
}

int Application::GetWindowHeight()
{
	return m_window_height;
}

int Application::GetWindowWidth()
{
	return m_window_width;
}

int Application::GetWindowHalfHeight()
{
	return m_window_height >> 1;
}

int Application::GetWindowHalfWidth()
{
	return m_window_width >> 1;
}

bool Application::IsKeyPressed(unsigned short key)
{
	return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

bool Application::ShouldQuit()
{
	return IsKeyPressed(VK_ESCAPE);
}

void Application::CheckForInput()
{
	//// Update Mouse Position
	double mouse_currX, mouse_currY;
	glfwGetCursorPos(m_window, &mouse_currX, &mouse_currY);
	glfwSetCursorPos(m_window, Application::GetWindowHalfWidth(), Application::GetWindowHalfHeight()); //reset mouse to center
	ControllerMouse::Instance()->UpdateMousePosition(mouse_currX, mouse_currY);	

	// Update Keyboard Input
	for (int i = 0; i < ControllerKeyboard::MAX_KEYS; ++i)
		ControllerKeyboard::Instance()->UpdateKeyboardStatus(i, IsKeyPressed(i));
}

void Application::InputReset()
{
	// Call input systems to update at end of frame
	ControllerMouse::Instance()->EndFrameUpdate();
	ControllerKeyboard::Instance()->EndFrameUpdate();
}


/////////////Callbacks/////////////

void Application::error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

void Application::resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

void Application::mouse_click_callback(GLFWwindow* window, int button, int action, int mods)
{
	// Send the callback to the mouse controller to handle
	if (action == GLFW_PRESS)
		ControllerMouse::Instance()->UpdateMouseButtonPressed(button);
	else
		ControllerMouse::Instance()->UpdateMouseButtonReleased(button);
}

void Application::mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	ControllerMouse::Instance()->UpdateMouseScroll(xoffset, yoffset);
}