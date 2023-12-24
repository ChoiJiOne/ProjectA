#pragma once

#include <glad/glad_wgl.h>
#include <glad/glad.h>

#include "Assertion.h"


#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(REL_WITH_DEB_INFO_MODE)
/**
 * @brief 에러 코드에 대응하는 메시지를 얻습니다.
 * 
 * @param errorCode 확인할 에러 코드입니다.
 * 
 * @return 에러 코드에 대응하는 메시지를 반환합니다.
 * 
 * @note
 * - https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetError.xhtml
 * - https://www.khronos.org/opengl/wiki/OpenGL_Error
 */
inline std::string GetGLErrorCodeMessageA(const GLenum& errorCode)
{
	std::string errorMessage;

	switch (errorCode)
	{
	case GL_NO_ERROR:
		errorMessage = "No error has been recorded. (GL_NO_ERROR)";
		break;

	case GL_INVALID_ENUM:
		errorMessage = "An unacceptable value is specified for an enumerated argument. (GL_INVALID_ENUM)";
		break;

	case GL_INVALID_VALUE:
		errorMessage = "A numeric argument is out of range. (GL_INVALID_VALUE)";
		break;

	case GL_INVALID_OPERATION:
		errorMessage = "The specified operation is not allowed in the current state. (GL_INVALID_OPERATION)";
		break;

	case GL_INVALID_FRAMEBUFFER_OPERATION:
		errorMessage = "The framebuffer object is not complete. (GL_INVALID_FRAMEBUFFER_OPERATION)";
		break;

	case GL_OUT_OF_MEMORY:
		errorMessage = "There is not enough memory left to execute the command. (GL_OUT_OF_MEMORY)";
		break;

	case GL_STACK_UNDERFLOW:
		errorMessage = "An attempt has been made to perform an operation that would cause an internal stack to underflow. (GL_STACK_UNDERFLOW)";
		break;

	case GL_STACK_OVERFLOW:
		errorMessage = "An attempt has been made to perform an operation that would cause an internal stack to overflow. (GL_STACK_OVERFLOW)";
		break;

	case GL_CONTEXT_LOST:
		errorMessage = "This occurs when the OpenGL context becomes lost due to a graphics card reset. (GL_CONTEXT_LOST)";
		break;

	default:
		errorMessage = "Undefined error message.";
	}

	return errorMessage;
}


/**
 * @brief 에러 코드에 대응하는 메시지를 얻습니다.
 *
 * @param errorCode 확인할 에러 코드입니다.
 *
 * @return 에러 코드에 대응하는 메시지를 반환합니다.
 *
 * @note
 * - https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetError.xhtml
 * - https://www.khronos.org/opengl/wiki/OpenGL_Error
 */
inline std::wstring GetGLErrorCodeMessageW(const GLenum& errorCode)
{
	std::wstring errorMessage;

	switch (errorCode)
	{
	case GL_NO_ERROR:
		errorMessage = L"No error has been recorded. (GL_NO_ERROR)";
		break;

	case GL_INVALID_ENUM:
		errorMessage = L"An unacceptable value is specified for an enumerated argument. (GL_INVALID_ENUM)";
		break;

	case GL_INVALID_VALUE:
		errorMessage = L"A numeric argument is out of range. (GL_INVALID_VALUE)";
		break;

	case GL_INVALID_OPERATION:
		errorMessage = L"The specified operation is not allowed in the current state. (GL_INVALID_OPERATION)";
		break;

	case GL_INVALID_FRAMEBUFFER_OPERATION:
		errorMessage = L"The framebuffer object is not complete. (GL_INVALID_FRAMEBUFFER_OPERATION)";
		break;

	case GL_OUT_OF_MEMORY:
		errorMessage = L"There is not enough memory left to execute the command. (GL_OUT_OF_MEMORY)";
		break;

	case GL_STACK_UNDERFLOW:
		errorMessage = L"An attempt has been made to perform an operation that would cause an internal stack to underflow. (GL_STACK_UNDERFLOW)";
		break;

	case GL_STACK_OVERFLOW:
		errorMessage = L"An attempt has been made to perform an operation that would cause an internal stack to overflow. (GL_STACK_OVERFLOW)";
		break;

	case GL_CONTEXT_LOST:
		errorMessage = L"This occurs when the OpenGL context becomes lost due to a graphics card reset. (GL_CONTEXT_LOST)";
		break;

	default:
		errorMessage = L"Undefined error message.";
	}

	return errorMessage;
}
#endif


/**
 * @brief OpenGL API 호출값을 검사합니다.
 * 
 * @param X 검사할 호출값입니다.
 * @param ... 평가식을 만족하지 못할 경우 표시할 가변 인자 메시지입니다.
 * 
 * @note
 * - Debug 모드와 Release 모드에서는 평가식을 검사하지만 Shipping 모드에서는 평가식을 검사하지 않습니다.
 * - 디버거가 존재하면 브레이크 포인트가 걸립니다.
 * - 디버거가 존재하지 않으면 크래시 덤프 파일을 생성합니다.
 */
#if defined(DEBUG_MODE)
#ifndef GL_ASSERT
#define GL_ASSERT(X, ...)\
X;\
{\
	GLenum glError = glGetError();\
	if (glError != GL_NO_ERROR)\
	{\
		AssertPrintF("\nOpenGL assertion check point failed!\nFile : %s\nLine : %d\nExpression : %s\nMessage : ", __FILE__, __LINE__, #X);\
		AssertPrintF(__VA_ARGS__);\
		AssertPrintF(L"\nOpenGL error message : %s\n", GetGLErrorCodeMessageW(glError).c_str());\
		__debugbreak();\
		ExitProcess(-1);\
	}\
}
#endif
#elif defined(RELEASE_MODE) || defined(REL_WITH_DEB_INFO_MODE)
#ifndef GL_ASSERT
#define GL_ASSERT(X, ...)\
X;\
{\
	GLenum glError = glGetError();\
	if (glError != GL_NO_ERROR)\
	{\
		AssertPrintF("\nOpenGL assertion check point failed!\nFile : %s\nLine : %d\nExpression : %s\nMessage : ", __FILE__, __LINE__, #X);\
		AssertPrintF(__VA_ARGS__);\
		AssertPrintF(L"\nOpenGL error message : %s\n", GetGLErrorCodeMessageW(glError).c_str());\
		__debugbreak();\
	}\
}
#endif
#else // defined(MIN_SIZE_REL_MODE)
#ifndef GL_ASSERT
#define GL_ASSERT(X, ...)\
X;
#endif
#endif