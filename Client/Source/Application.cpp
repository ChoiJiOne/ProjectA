#include <cstdint>
#include <string>
#include <windows.h>

#include <glad/glad.h>

#include "Assertion.h"
#include "CommandLineUtils.h"
#include "FileManager.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Window.h"
#include "WindowsCrashUtils.h"

bool bIsDone = false;

void RunApplication()
{
	std::wstring shaderPath;
	CommandLineUtils::GetStringValue(L"shaderPath", shaderPath);

	Shader* shader = ResourceManager::Get().CreateResource<Shader>("Shader");
	shader->Initialize(shaderPath + L"Shader.vert", shaderPath + L"Shader.frag");

	std::vector<Vector3f> vertices = {
		Vector3f(-0.5f, -0.5f, 0.0f),
		Vector3f(+0.5f, -0.5f, 0.0f),
		Vector3f(+0.0f, +0.5f, 0.0f),
	};

	uint32_t vbo;
	uint32_t vao;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<uint32_t>(vertices.size()) * sizeof(Vector3f), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (void*)(0));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	while (!bIsDone)
	{
		InputManager::Get().Tick();

		RenderManager::Get().BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);
		{
			shader->Bind();

			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, static_cast<uint32_t>(vertices.size()));

			glBindVertexArray(0);
			glUseProgram(0);

			shader->Unbind();
		}
		RenderManager::Get().EndFrame();
	}

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
	WindowsCrashUtils::RegisterExceptionFilter();
	CommandLineUtils::Parse();
	Window::RegisterWindowClass(L"ProjectA", InputManager::WindowProc);

	Window window;
	window.Create(L"ProjectA", 200, 200, 800, 600, true, false);
	InputManager::Get().SetInputControlWindow(&window);
	RenderManager::Get().SetRenderTargetWindow(&window);

	FileManager::Get().Startup();
	InputManager::Get().Startup();
	RenderManager::Get().Startup();
	ResourceManager::Get().Startup();
	
	auto quitEvent = [&]() { bIsDone = true; };
	auto resizeEvent = [&]() {
		int32_t width;
		int32_t height;
		window.GetSize(width, height);
		glViewport(0, 0, width, height);
	};
		
	InputManager::Get().AddWindowEventAction("CloseLoop",     EWindowEvent::Close,         quitEvent,   true);
	InputManager::Get().AddWindowEventAction("Resize",        EWindowEvent::Resize,        resizeEvent, true);
	InputManager::Get().AddWindowEventAction("ExitMinimize",  EWindowEvent::ExitMinimize,  resizeEvent, true);
	InputManager::Get().AddWindowEventAction("EnterMaximize", EWindowEvent::EnterMaximize, resizeEvent, true);
	InputManager::Get().AddWindowEventAction("ExitMaximize",  EWindowEvent::ExitMaximize,  resizeEvent, true);

	RunApplication();
	
	ResourceManager::Get().Shutdown();
	RenderManager::Get().Shutdown();
	InputManager::Get().Shutdown();
	FileManager::Get().Shutdown();
	window.Destroy();

	Window::UnregisterWindowClass();
	WindowsCrashUtils::UnregisterExceptionFilter();
	return 0;
}