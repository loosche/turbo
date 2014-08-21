#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "utilities/std_types.h"

class c_gpu_device_context_d3d11;
class c_gpu_device_d3d11;
class c_window_win32;

class c_engine
{
public:
	c_engine();
	~c_engine();

	void destroy();

	bool initialize(c_window_win32 *window);

	void resize_back_buffer(int32 width, int32 height);

	void step();

private:
	
	c_gpu_device_d3d11	*m_device;
	c_window_win32		*m_window;

};

#endif