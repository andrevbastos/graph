#pragma once 

#include "ifcg/common/camera.hpp"

namespace camera
{
	class Camera2D : public Camera
	{
	public:
		Camera2D(int width, int height);
		
		void update(float FOVdeg, float nearPlane, float farPlane, GLuint shaderID);
		void inputs(GLFWwindow* window);
		void setPos(glm::vec3 pos);

		glm::mat4 getProjectionMatrix() { return glm::mat4(1.0f); };
        glm::mat4 getViewMatrix() { return glm::mat4(1.0f); };
	private:
		glm::vec3 position = glm::vec3(0.0f, 0.0f, -0.5f);
		glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

		int width;
		int height;

		float speed = 0.1f;
		float sensitivity = 100.0f;
	};
}

using namespace camera;