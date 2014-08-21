#include <assert.h>
#include <d3d11.h>
#include <windows.h>

#include "engine/gpu_device_d3d11.h"
#include "engine/gpu_device_context_d3d11.h"

c_gpu_device_context_d3d11::c_gpu_device_context_d3d11()
{
	m_context= NULL;
	m_device= NULL;
	m_render_target= NULL;
}

c_gpu_device_context_d3d11::~c_gpu_device_context_d3d11()
{
	destroy();
}

void c_gpu_device_context_d3d11::clear_color_buffer(
	const c_color &color)
{
	assert(m_render_target);
	assert(m_context);

	m_context->ClearRenderTargetView(m_render_target, color.values());
}

void c_gpu_device_context_d3d11::initialize(
	ID3D11DeviceContext *context)
{
	assert(m_context == NULL);
	assert(context != NULL);
	m_context= context;
}

bool c_gpu_device_context_d3d11::attach_to_swap_chain(
	c_gpu_device_d3d11	*device)
{
	assert(device);
	m_device= device;

	if (m_render_target)
	{
		m_render_target->Release();
		m_render_target= NULL;
	}

	return device->create_render_target(&m_render_target);
}

void c_gpu_device_context_d3d11::destroy()
{
	if (m_render_target != NULL)
	{
		m_render_target->Release();
		m_render_target= NULL;
	}

	if (m_context != NULL)
	{
		m_context->Release();
		m_context= NULL;
	}
}

bool c_gpu_device_context_d3d11::release_from_swap_chain()
{
	if (m_render_target)
	{
		m_render_target->Release();
		m_render_target= NULL;
	}

	return true;
}
