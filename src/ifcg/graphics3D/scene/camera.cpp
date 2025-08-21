#include "ifcg/graphics3D/scene/camera.hpp"

namespace camera
{
	Camera3D::Camera3D(int w, int h)
		: width(w), height(h) {}

	void Camera3D::update(float FOVdeg, float nearPlane, float farPlane, GLuint shaderID)
	{	
		view = glm::lookAt(position, position + orientation, up);
		projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

		glUniformMatrix4fv(
			glGetUniformLocation(shaderID, "cam"),
			1,
			GL_FALSE,
			glm::value_ptr(projection * view)
		);
	}

	void Camera3D::inputs(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			translate(speed, orientation);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			translate(speed, -glm::normalize(glm::cross(orientation, up)));
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			translate(speed, -orientation);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			translate(speed, glm::normalize(glm::cross(orientation, up)));
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			translate(speed, up);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			translate(speed, -up);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			translate(speed, glm::normalize(glm::cross(orientation, up)));
			rotate(glm::radians(1.0f), up);
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			translate(speed, -glm::normalize(glm::cross(orientation, up)));
			rotate(glm::radians(-1.0f), up);
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			if (firstClick)
			{
				glfwSetCursorPos(window, (width / 2), (height / 2));
				firstClick = false;
			}

			double mouseX;
			double mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);

			float xoffset = sensitivity * (float)(mouseY - (height / 2)) / height;
			float yoffset = sensitivity * (float)(mouseX - (width / 2)) / width;

			pitch += glm::radians(-xoffset);

			if (pitch >= 1)
				pitch = 1;
			if (pitch <= -1)
				pitch = -1;

			if (abs(pitch) != 1.0f) {
				rotate(glm::radians(-xoffset), glm::normalize(glm::cross(orientation, up)));
			}

			rotate(glm::radians(-yoffset), up);

			glfwSetCursorPos(window, (width / 2), (height / 2));
		} else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			firstClick = true;
		}
	}

	void Camera3D::setPos(glm::vec3 pos)
	{
		position = pos;
	};

	void Camera3D::translate(float amount, glm::vec3 target)
	{
		position += amount * target;
	};

	void Camera3D::rotate(float angle, glm::vec3 axis)
	{
		orientation = glm::rotate(orientation, angle, axis);
	};

	glm::mat4 Camera3D::getProjectionMatrix()
	{
		return projection;
	};

	glm::mat4 Camera3D::getViewMatrix()
	{
		return view;
	};
}