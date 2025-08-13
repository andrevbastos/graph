#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "ifcg/shader/shader.hpp"

namespace camera
{
    class Camera
    {
    public:
        virtual ~Camera() = default;

        virtual void update(float FOVdeg, float nearPlane, float farPlane, GLuint shaderID) = 0;
        virtual void inputs(GLFWwindow* window) = 0;
		virtual void setPos(glm::vec3 pos) = 0;

        virtual glm::mat4 getProjectionMatrix() = 0;
        virtual glm::mat4 getViewMatrix() = 0;
    };
}

using namespace camera;