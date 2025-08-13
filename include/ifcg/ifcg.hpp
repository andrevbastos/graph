#pragma once

/**
* \author André B.
* \date March 20, 2025
* \version March 20, 2025
* \brief Constructor for IFCG class using Singleton pattern
**/

#include <iostream>
#include <algorithm>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <functional>
#include <thread>
#include <chrono>

#include "ifcg/shader/shader.hpp"
#include "ifcg/common/mesh.hpp"
#include "ifcg/common/camera.hpp"

#include "ifcg/graphics2D/scene/camera.hpp"
#include "ifcg/graphics3D/scene/camera.hpp"
#include "ifcg/graphics2D/geometry/mesh.hpp"
#include "ifcg/graphics3D/geometry/mesh.hpp"

#if defined(_WIN32) && defined(IFCG_BUILD_SHARED)
	#define IFCG_API __declspec(dllexport)
#elif defined(_WIN32) && !defined(IFCG_BUILD_SHARED)
	#define IFCG_API __declspec(dllimport)
#else
	#define IFCG_API
#endif

namespace ifcg
{
    class IFCG
	{
	public:
		static void init();

		static void setFramesPerSecond(int fps);
        static void loop(const std::function<void()>& gameLoopBody);
		static void createWindow(GLuint w, GLuint h);
		static void destroyWindow();
		static bool shouldClose();
    	static void clearBuffer(float r, float g, float b, float a = 1.0f);
		static void readInputs();
		static void processInput();
		static void swapBuffer();
		
		static void setup2D();
		static void setup3D();

		static void addMesh(Mesh* mesh);
		static void removeMesh(Mesh* mesh);
		static void render();
		
		static void terminate();
		
		static GLFWwindow* window;
		static Camera* camera;
		static Shader shader;
		
		static std::vector<Mesh*> renderQueue;
		static GLuint width;
		static GLuint height;
	private:
		IFCG();
		static void terminate_();
		
		static IFCG* instance;
		static double frameTimeTarget; 
	};
};

using namespace ifcg;