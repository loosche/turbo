#ifndef __GPU_DEVICE_D3D11_H__
#define __GPU_DEVICE_D3D11_H__

#include <windows.h>

#include "utilities/std_types.h"

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
class c_gpu_device_context_d3d11;
class c_logger;
class c_window_win32;

class c_gpu_device_d3d11
{
	friend class c_gpu_device_context_d3d11;

public:

	c_gpu_device_d3d11();
	~c_gpu_device_d3d11();

	bool create(const c_window_win32 *window);

	bool create_render_target(ID3D11RenderTargetView **render_target_view);
	
	void destroy();

	int32 get_back_buffer_height();

	int32 get_back_buffer_width();

	c_gpu_device_context_d3d11 *get_context();

	ID3D11Device *get_device();

	void present();

	void resize(uint32 width, uint32 height);

private:

	ID3D11Device				*m_device;
	c_gpu_device_context_d3d11	*m_device_context;
	int32						 m_height;
	c_logger					*m_logger;
	IDXGISwapChain				*m_swap_chain;
	const c_window_win32		*m_window;
	int32						 m_width;

	void handle_d3d11_error(
		const char	*function_name,
		const char	*description,
		HRESULT		 result);
};

#endif // __GPU_DEVICE_D3D11_H__