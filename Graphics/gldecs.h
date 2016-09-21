#pragma once
#define GLEW_STATIC

#include "GLEW\glew.h"
#include "GLFW\glfw3.h"

#ifdef _DEBUG
#include <iostream>

//#define glog(detail, extra) \
//do{\
//	std::cout << "In " << __FILE__ << " at " << __FUNCTION__ << " on line " << __LINE__ << " : " << detail << " , " << extra << std::endl;\
//} while(0)

#define glog(detail, extra) \
do{\
	std::cout << "In " << __FILE__ << " at " << __FUNCTION__ << " on line " \
			  << __LINE__ << " : " << detail << " , " << extra << std::endl; \
\
} while(0)

#define glog_glCompileShader(shader) \
do\
{\
glCompileShader(shader);\
GLint success = GL_FALSE;\
glGetShaderiv(shader,GL_COMPILE_STATUS, &success);\
if(success == GL_FALSE)\
{\
	int length = 0;\
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);\
	char *log = (char*)malloc(length);\
	glGetShaderInfoLog(shader,length,0,log);\
	glog("Shader failed to compile!\n",log);\
	free(log);\
}\
}while(0)

#define glog_glLinkProgram(shader) \
do\
{\
glLinkProgram(shader);\
GLint success = GL_FALSE;\
glGetProgramiv(shader,GL_LINK_STATUS, &success);\
if(success == GL_FALSE)\
{\
	int length = 0;\
	glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &length);\
	char *log = (char*)malloc(length);\
	glGetProgramInfoLog(shader,length,0,log);\
	glog("Shader failed to compile!\n",log);\
	free(log);\
}\
}while(0)

#else
#define glog(detail,extra)
#define glog_glCompileShader(shader) glCompileShader(shader)
#define glog_glLinkProgram(shader) glLinkProgram(shader)
#endif // DEBUG