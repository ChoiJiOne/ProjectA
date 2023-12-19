#include <cstdint>
#include <string>
#include <windows.h>

#include <glad/glad_wgl.h>
#include <glad/glad.h>

#include "Window.h"
#include "WindowsAssertion.h"

Window window;

LRESULT CALLBACK WindowProc(HWND windowHandle, uint32_t message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		break;

	case WM_CLOSE:
		window.Destroy();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(windowHandle, message, wParam, lParam);
}

void LoadExtension(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);

	PIXELFORMATDESCRIPTOR pfd = {};	
	int32_t pixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, pixelFormat, &pfd);

	HGLRC dummyRenderContext = wglCreateContext(hdc);
	wglMakeCurrent(hdc, dummyRenderContext);
	gladLoadWGLLoader((GLADloadproc)(wglGetProcAddress), hdc);
	
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(dummyRenderContext);
	ReleaseDC(hwnd, hdc);
}


int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
	Window::RegisterWindowClass(L"ProjectA", WindowProc);

	window.Create(L"ProjectA", 200, 200, 800, 600, true, false);

	LoadExtension(window.GetHandle());

	HDC hdc = GetDC(window.GetHandle());

	PIXELFORMATDESCRIPTOR pfd = {};

	const int32_t attributeList[] = {
		WGL_SUPPORT_OPENGL_ARB, TRUE,
		WGL_DRAW_TO_WINDOW_ARB, TRUE,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_DOUBLE_BUFFER_ARB, TRUE,
		WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		0,
	};

	int32_t pixelFormat;
	uint32_t pixCount;
	wglChoosePixelFormatARB(hdc, attributeList, nullptr, 1, &pixelFormat, &pixCount);
	DescribePixelFormat(hdc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	int a = SetPixelFormat(hdc, pixelFormat, &pfd);
	//WINDOWS_ASSERT(SetPixelFormat(hdc, pixelFormat, &pfd), "failed to set pixel format...");

	const int32_t attributes[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		WGL_CONTEXT_FLAGS_ARB, 0,
		WGL_CONTEXT_PROFILE_MASK_ARB,
		WGL_CONTEXT_CORE_PROFILE_BIT_ARB, 
		0,
	};

	HGLRC renderContext = wglCreateContextAttribsARB(hdc, 0, attributes);
	wglMakeCurrent(hdc, renderContext);

	gladLoadGL();

	bool bIsDone = false;
	MSG msg = {};
	while (!bIsDone)
	{
		MSG msg = {};
		while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bIsDone = true;
			}

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		glViewport(0, 0, 800, 600);
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SwapBuffers(hdc);
	}

	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(renderContext);
	ReleaseDC(window.GetHandle(), hdc);

	Window::UnregisterWindowClass();
	return 0;
}