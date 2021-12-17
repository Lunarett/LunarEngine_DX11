#include "Window.h"


LRESULT CALLBACK WndProc(HWND t_hWnd, UINT t_msg, WPARAM t_wParam, LPARAM t_lParam)
{
	switch (t_msg)
	{
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (t_wParam == VK_ESCAPE) DestroyWindow(t_hWnd);
		break;

	case WM_MOUSEMOVE:
		break;

	default:
		return DefWindowProc(t_hWnd, t_msg, t_wParam, t_lParam);
	}

	return 0;
}


INT Window::init(HINSTANCE hInstance, UINT width, UINT height)
{
	WNDCLASS windowClass = {};
	windowClass.hInstance = hInstance;
	windowClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpszClassName = TEXT("3D object Rendering");
	windowClass.lpfnWndProc = WndProc;

	if (RegisterClass(&windowClass) == 0) return 10;

	UINT screenWidth = GetSystemMetrics(SM_CXSCREEN);
	UINT screenHeight = GetSystemMetrics(SM_CYSCREEN);
	RECT wr = { (screenWidth - width) / 2, (screenHeight - height) / 2, (screenWidth + width) / 2, (screenHeight + height) / 2 };
	DWORD styleWindowed = WS_OVERLAPPEDWINDOW;
	DWORD styleFWindowed = WS_POPUP;
	AdjustWindowRect(&wr, styleFWindowed, false);

	m_hWnd = CreateWindow(windowClass.lpszClassName, windowClass.lpszClassName, styleFWindowed, wr.left, wr.top, wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, hInstance, nullptr);

	if (m_hWnd == nullptr) return 15;

	ShowWindow(m_hWnd, SW_SHOW);
	SetFocus(m_hWnd);

	return 0;
}

BOOL Window::run()
{
	static MSG msg = {};
	if (PeekMessage(&msg, nullptr, 0, UINT_MAX, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.message != WM_QUIT;
}

void Window::deInit()
{
}
