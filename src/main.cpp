
#include <windows.h>

#include "engine/engine.h"
#include "window/window_win32.h"

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		c_window_win32 window;
		window.set_instance(hInstance);
		window.create_window("Example", 50, 50, 200, 200);

		c_engine engine;

		if (engine.initialize(&window))
		{
			window.set_engine(&engine);
			window.show();
		}

		CoUninitialize();
	}

	return 0;
}