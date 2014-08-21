#include <assert.h>

#include "engine/engine.h"
#include "engine/gpu_device_context_d3d11.h"
#include "engine/gpu_device_d3d11.h"
#include "window/window_win32.h"

c_engine::c_engine()
{
	m_device= NULL;
	m_window= NULL;
}

c_engine::~c_engine()
{
	destroy();
}

void c_engine::destroy()
{
	if (m_device)
	{
		m_device->destroy();
		delete m_device;
		m_device= NULL;
	}

	m_window= NULL;
}

bool c_engine::initialize(
	c_window_win32 *window)
{
	assert(window);

	m_window= window;

	assert(m_device == NULL);
	m_device= new c_gpu_device_d3d11;

	return m_device->create(m_window);
}

void c_engine::resize_back_buffer(
	int32	width,
	int32	height)
{
	if (m_device)
	{
		m_device->resize(width, height);
	}
}

void c_engine::step()
{
	c_gpu_device_context_d3d11 *context= m_device->get_context();
	
	assert(context);
	context->clear_color_buffer(k_color_blue);

	m_device->present();
}
