
#include "Build_Options.h"
#include "Platform.h"
#include "Window.h"
#include "Shared.h"
#include "_INIT_VULKAN.h"

#include <string>
#include <assert.h>

#if VK_USE_PLATFORM_WIN32_KHR
LRESULT CALLBACK WindowsEventHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window * window = reinterpret_cast<Window*>(
		GetWindowLongPtrW(hWnd, GWLP_USERDATA));

	switch (uMsg) {
	case WM_CLOSE:
		window->Close();
		return 0;
	case WM_SIZE:
		// we get here if the window has changed size, we should rebuild most
		// of our window resources before rendering to this window again.
		// ( no need for this because our window sizing by hand is disabled )
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

uint64_t	Window::_win32_class_id_counter = 0;

void Window::_InitOSWindow()
{
	WNDCLASSEX win_class{};  // 만들어질 윈도우의 여러가지 특성을 정의하기 위함.
	assert(_surface_size_x > 0);
	assert(_surface_size_y > 0);

	_win32_instance = // 인스턴스 Module의 핸들을 지정.
		GetModuleHandle(nullptr); // 현재 실행파일의 Module값을 가져오는데 사용하는 함수!
	_win32_class_name = _window_name + "_" + std::to_string(_win32_class_id_counter);
	_win32_class_id_counter++;

	// Initialize the window class structure:
	win_class.cbSize = sizeof(WNDCLASSEX);
	win_class.style = CS_HREDRAW | CS_VREDRAW;
	win_class.lpfnWndProc = WindowsEventHandler;
	win_class.cbClsExtra = 0;
	win_class.cbWndExtra = 0;
	win_class.hInstance = _win32_instance; // hInstance
	win_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	win_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	win_class.lpszMenuName = NULL;
	win_class.lpszClassName = _win32_class_name.c_str();
	win_class.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	// Register window class:
	if (!RegisterClassEx(&win_class)) { //윈도우 클래스를 등록한다.
		// It didn't work, so try to give a useful error:
		assert(0 && "Cannot create a window in which to draw!\n");
		fflush(stdout);
		std::exit(-1);
	}

	DWORD ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	// Create window with the registered class:
	RECT wr = { 0, 0, LONG(_surface_size_x), LONG(_surface_size_y) };
	AdjustWindowRectEx(&wr, style, FALSE, ex_style); // 
	_win32_window = CreateWindowEx(0,
		_win32_class_name.c_str(),		// class name
		_window_name.c_str(),			// app name
		style,							// window style
		CW_USEDEFAULT, CW_USEDEFAULT,	// x/y coords
		wr.right - wr.left,				// width
		wr.bottom - wr.top,				// height
		NULL,							// handle to parent
		NULL,							// handle to menu
		_win32_instance,				// hInstance
		NULL);							// no extra parameters
	if (!_win32_window) {
		// It didn't work, so try to give a useful error:
		assert(1 && "Cannot create a window in which to draw!\n");
		fflush(stdout);
		std::exit(-1);
	}
	SetWindowLongPtr(_win32_window, GWLP_USERDATA, (LONG_PTR)this);

	ShowWindow(_win32_window, SW_SHOW);
	SetForegroundWindow(_win32_window);
	SetFocus(_win32_window);
}

void Window::_DeInitOSWindow()
{
	DestroyWindow(_win32_window);
	UnregisterClass(_win32_class_name.c_str(), _win32_instance);
}

void Window::_UpdateOSWindow()
{
	MSG msg;
	if (PeekMessage(&msg, _win32_window, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window::_InitOSSurface()
{
	VkWin32SurfaceCreateInfoKHR create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	create_info.hinstance = _win32_instance;
	create_info.hwnd = _win32_window;
	vkCreateWin32SurfaceKHR(_renderer->GetVulkanInstance(), &create_info, nullptr, &_surface);
}

#endif