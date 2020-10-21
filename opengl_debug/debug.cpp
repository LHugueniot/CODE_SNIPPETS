#include "GL/glew.h"

static void OpenGLLogMessage(GLenum source, 
							 GLenum type, 
							 GLenum id, 
							 GLenum severity, 
							 GLsizei length,
							 const void* userParam){
	switch(severity){
		case GL_DEBUG_SEVERITY_HIGH:
			std::cout<<"BAD"<<std::endl;
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			std::cout<<"PRETTY BAD"<<std::endl;
			break;
		case GL_DEBUG_SEVERITY_LOW:
			std::cout<<"NOT VERY BAD"<<std::endl;
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			std::cout<<"???"<<std::endl;
			break;
	}
}

int main(int argc, char const *argv[]){

	glDebugMessageCallback(OpenGLLogMessage, nullptr);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	return 0;
}