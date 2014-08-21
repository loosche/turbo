#include "engine/engine.h"
#include "window/window_win32.h"

#include <assert.h>

const char* c_window_win32::m_class_name= "window_win32";

c_window_win32::c_window_win32()
{
	m_engine= NULL;

	m_width= 0;
	m_height= 0;
	m_fullscreen= false;
}

c_window_win32::~c_window_win32()
{
	if (IsWindow(m_window_handle))
	{
		destroy();
	}
}

void c_window_win32::create_window(
	const char	*title, 
	int32		 top_left, 
	int32		 top_right, 
	int32		 width, 
	int32		 height)
{
	register_window();

	m_width= width;
	m_height= height;

	m_window_handle= CreateWindowEx(
		0,
		c_window_win32::m_class_name,
		title,
		WS_OVERLAPPEDWINDOW,
		top_left,
		top_right,
		m_width,
		m_height,
		NULL,
		NULL,
		m_instance_handle,
		this);
}

LRESULT c_window_win32::destroy()
{
	LRESULT result= DefWindowProc(m_window_handle, WM_NCDESTROY, 0, 0);
	SetWindowLongPtr(m_window_handle, GWLP_USERDATA, 0);
	PostQuitMessage(0);

	return result;
}

LRESULT c_window_win32::handle_message(
	UINT	message, 
	WPARAM	w_param, 
	LPARAM	l_param)
{
	LRESULT result;

	switch (message)
	{
	case WM_CREATE:
		result= 0;
		break;

	case WM_NCDESTROY:
		result= destroy();
		break;

	case WM_SIZE:
		result= resize(w_param, l_param);
		break;

	case WM_SETFOCUS:
		SetFocus(m_window_handle);
		result= 0;
		break;

	default:
		result= DefWindowProc(m_window_handle, message, w_param, l_param);
		break;
	}

	return result;
}

LRESULT CALLBACK c_window_win32::message_router(
	HWND	window_handle, 
	UINT	message, 
	WPARAM	w_param, 
	LPARAM	l_param)
{
	c_window_win32 *window= NULL;

	if (message == WM_NCCREATE)
	{
		// Retrieve the window instance from the window creation data and store it
		window= reinterpret_cast<c_window_win32*>(((LPCREATESTRUCT)l_param)->lpCreateParams);

		window->m_window_handle= window_handle;
		SetWindowLongPtr(window_handle, GWLP_USERDATA, reinterpret_cast<LPARAM>(window));
	}
	else
	{
		// Retrieve the window instance associated with the window_handle
		window= reinterpret_cast<c_window_win32*>(GetWindowLongPtr(window_handle, GWLP_USERDATA));
	}

	LRESULT result;

	if (window != NULL)
	{
		result= window->handle_message(message, w_param, l_param);
	}
	else
	{
		result= DefWindowProc(window_handle, message, w_param, l_param);
	}

	return result;
}

void c_window_win32::register_window()
{
	WNDCLASS window_class;

	window_class.style=			0;
	window_class.lpfnWndProc=	c_window_win32::message_router;
	window_class.cbClsExtra=	0;
	window_class.cbWndExtra=	0;
	window_class.hInstance=		m_instance_handle;
	window_class.hIcon=			NULL;
	window_class.hCursor=		LoadCursor(NULL, IDC_ARROW);
	window_class.hbrBackground=	(HBRUSH)(COLOR_WINDOW+1);
	window_class.lpszMenuName=	NULL;
	window_class.lpszClassName= c_window_win32::m_class_name;

	RegisterClass(&window_class);
}

LRESULT c_window_win32::resize(
	WPARAM	w_param,
	LPARAM	l_param)
{
	LRESULT result= 0;

	if (IsWindow(m_window_handle))
	{
		m_width= HIWORD(l_param);
		m_height= LOWORD(l_param);

		if (m_engine)
		{
			m_engine->resize_back_buffer(m_width, m_height);
		}
	}

	return 0;
}

void c_window_win32::run_message_loop()
{
	if (IsWindow(m_window_handle))
	{
		MSG message;

		while (true)
		{
			while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}

			if (message.message == WM_QUIT)
			{
				break;
			}

			m_engine->step();
		}
	}
}

void c_window_win32::set_engine(
	c_engine	*engine)
{
	assert(engine);
	m_engine= engine;
}

void c_window_win32::set_instance(
	HINSTANCE	instance_handle)
{
	m_instance_handle= instance_handle;
}

void c_window_win32::show()
{
	if (IsWindow(m_window_handle))
	{
		ShowWindow(m_window_handle, SW_SHOWDEFAULT);

		run_message_loop();
	}
}
