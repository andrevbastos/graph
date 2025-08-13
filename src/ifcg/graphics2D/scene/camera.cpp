#include "ifcg/graphics2D/scene/camera.hpp"

namespace camera
{
	Camera2D::Camera2D(int width, int height)
		: width(width), height(height) {}

	void Camera2D::update(float FOVdeg, float nearPlane, float farPlane, GLuint shaderID)
	{
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		view = glm::lookAt(position, position + orientation, up);
		projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

		glUniformMatrix4fv(
			glGetUniformLocation(shaderID, "cam"),
			1,
			GL_FALSE,
			glm::value_ptr(projection * view)
		);
	}

	void Camera2D::inputs(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			position += speed * up;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			position += speed * -glm::normalize(glm::cross(orientation, up));
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			position += speed * -up;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			position += speed * glm::normalize(glm::cross(orientation, up));
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			position += speed * -orientation;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			position += speed * orientation;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			speed = 0.4f;
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		{
			speed = 0.1f;
		}
	}

	void Camera2D::setPos(glm::vec3 pos)
	{
		position = pos;
	};
}

using namespace camera;