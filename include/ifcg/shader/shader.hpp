#pragma once

/**
* \author André B.
* \date February 27, 2025
* \version March 15, 2025
* \brief Constructor for Shader class
**/

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

namespace shader
{
	class Shader
	{
	public:
		GLuint id;
		Shader(); 
		Shader(const char* vertexFile, const char* fragmentFile);
		
		void activate();
		void terminate();
		private:
		void compileErrors(unsigned int shader, const char* type);
	};
}

using namespace shader;