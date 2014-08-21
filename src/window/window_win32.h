#ifndef __WINDOW_WIN32_H__
#define __WINDOW_WIN32_H__

#include <windows.h>
#include "utilities/std_types.h"

class c_engine;

class c_window_win32
{
	public:
		c_window_win32();
		~c_window_win32();


		void create_window(
			const char *title, 
			int32 top_left, 
			int32 top_right, 
			int32 width, 
			int32 height);

		bool get_fullscreen() const { return m_fullscreen; }

		uint32 get_height() const { return m_height; }

		uint32 get_width() const { return m_width; }

		HWND get_window_handle() const { return m_window_handle; }

		void set_engine(c_engine *engine);

		void set_instance(HINSTANCE instance_handle);

		void show();

	protected:

		LRESULT handle_message(UINT message, WPARAM w_param, LPARAM l_param);
		void	paint(PAINTSTRUCT *paint_struct);
		
	private:
		
		static LRESULT CALLBACK message_router(
			HWND window_handle, 
			UINT message, 
			WPARAM w_param, 
			LPARAM l_param);

		LRESULT destroy();

		void on_paint();

		void on_print(HDC context_handle);

		void register_window();

		void run_message_loop();

		LRESULT resize(WPARAM w_param, LPARAM l_param);

		static const char	*m_class_name;
		bool				 m_fullscreen;
		c_engine			*m_engine;
		uint32				 m_height;
		HINSTANCE			 m_instance_handle;
		uint32				 m_width;
		HWND				 m_window_handle;
};

#endif __WINDOW_WIN32_H__