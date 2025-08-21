#pragma once 

#include "ifcg/common/camera.hpp"

namespace camera
{
	class Camera3D : public Camera
	{
	public:
		Camera3D(int width, int height);
		
		void update(float FOVdeg, float nearPlane, float farPlane, GLuint shaderID);
		void inputs(GLFWwindow* window);
		void setPos(glm::vec3 pos);

		glm::mat4 getProjectionMatrix();
		glm::mat4 getViewMatrix();
		
	private:
		void translate(float t, glm::vec3 pos);
		void rotate(float angle, glm::vec3 axis);
		
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);

		bool firstClick = true;
		
		float pitch = 0.0f;
		
		int width;
		int height;
		
		float speed = 0.1f;
		float sensitivity = 100.0f;
	};
}

using namespace camera;