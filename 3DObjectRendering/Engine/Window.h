#pragma once
#include <Windows.h>


class Window
{
public:
	INT init(HINSTANCE t_hInstance, UINT t_width, UINT t_height);
	BOOL run();
	void deInit();

	HWND getWindowHandle() { return m_hWnd; }

private:
	HWND m_hWnd = nullptr;
};

