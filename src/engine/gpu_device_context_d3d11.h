#ifndef __GPU_DEVICE_CONTEXT_D3D11_H__
#define __GPU_DEVICE_CONTEXT_D3D11_H__

#include "utilities/color.h"

struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct IDXGISwapChain;

class c_gpu_device_d3d11;

class c_gpu_device_context_d3d11
{
public:
	c_gpu_device_context_d3d11();
	~c_gpu_device_context_d3d11();

	bool attach_to_swap_chain(c_gpu_device_d3d11 *device);

	void clear_color_buffer(const c_color &color);

	void initialize(ID3D11DeviceContext *context);

	bool release_from_swap_chain();

	void destroy();

private:

	ID3D11DeviceContext		*m_context;
	c_gpu_device_d3d11		*m_device;
	ID3D11RenderTargetView	*m_render_target;

};

#endif __GPU_DEVICE_CONTEXT_D3D11_H__