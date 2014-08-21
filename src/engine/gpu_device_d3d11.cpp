
#include <assert.h>
#include <dxgi.h>
#include <d3d11.h>

#include "engine/gpu_device_d3d11.h"
#include "engine/gpu_device_context_d3d11.h"
#include "utilities/logger.h"
#include "window/window_win32.h"

const DXGI_FORMAT k_d3d11_back_buffer_format= DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

c_gpu_device_d3d11::c_gpu_device_d3d11()
{
	m_device= NULL;
	m_device_context= NULL;
	m_swap_chain= NULL;
	m_window= NULL;

	m_width= 0;
	m_height= 0;


	m_logger= new c_logger();
}

c_gpu_device_d3d11::~c_gpu_device_d3d11()
{
	destroy();

	delete m_logger;
	m_logger= NULL;
}

bool c_gpu_device_d3d11::create(
	const c_window_win32 *window)
{
	assert(window);
	m_window= window;
	m_width= m_window->get_width();
	m_height= m_window->get_height();

	DXGI_SWAP_CHAIN_DESC swap_chain_description;
	ZeroMemory(&swap_chain_description, sizeof(DXGI_SWAP_CHAIN_DESC));

	swap_chain_description.BufferCount=			2;
	swap_chain_description.BufferDesc.Format=	k_d3d11_back_buffer_format;
	swap_chain_description.BufferDesc.Height=	m_window->get_height();
	swap_chain_description.BufferDesc.Width=	m_window->get_width();
	swap_chain_description.BufferUsage=			DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_description.OutputWindow=		m_window->get_window_handle();
	swap_chain_description.Windowed=			m_window->get_fullscreen() == false;
	swap_chain_description.SampleDesc.Count=	1;
	swap_chain_description.SampleDesc.Quality=	0;

	ID3D11DeviceContext *device_context= NULL;
	D3D_FEATURE_LEVEL actual_feature_level;

	HRESULT result= D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swap_chain_description,
		&m_swap_chain,
		&m_device,
		&actual_feature_level,
		&device_context);

	handle_d3d11_error("c_gpu_device_d3d11::create()", "", result);

	m_device_context= new c_gpu_device_context_d3d11();
	m_device_context->initialize(device_context);
	m_device_context->attach_to_swap_chain(this);

	return SUCCEEDED(result);
}

bool c_gpu_device_d3d11::create_render_target(
	ID3D11RenderTargetView **render_target_view)
{
	assert(m_swap_chain);
	ID3D11Texture2D* back_buffer;
	HRESULT result= m_swap_chain->GetBuffer(0, __uuidof(*back_buffer), ( LPVOID* )&back_buffer);

	handle_d3d11_error(
		"c_gpu_device_context::create()",
		"Getting back buffer",
		result);

	if (SUCCEEDED(result))
	{
		D3D11_TEXTURE2D_DESC back_buffer_desc;
		back_buffer->GetDesc(&back_buffer_desc);

		result= m_device->CreateRenderTargetView(back_buffer, NULL,	render_target_view);

		handle_d3d11_error(
			"c_gpu_device_context::create()",
			"Creating render target view",
			result);

		back_buffer->Release();
		back_buffer= NULL;
	}

	return SUCCEEDED(result);
}

void c_gpu_device_d3d11::destroy()
{
	if (m_device)
	{
		m_device->Release();
	}

	if (m_device_context)
	{
		m_device_context->destroy();
	}

	if (m_swap_chain)
	{
		m_swap_chain->Release();
	}

	m_device= NULL;
	m_device_context= NULL;
	m_swap_chain= NULL;
	m_window= NULL;
}

int32 c_gpu_device_d3d11::get_back_buffer_height()
{
	return m_height;
}

int32 c_gpu_device_d3d11::get_back_buffer_width()
{
	return m_width;
}

c_gpu_device_context_d3d11 *c_gpu_device_d3d11::get_context()
{
	return m_device_context;
}

ID3D11Device *c_gpu_device_d3d11::get_device()
{
	return m_device;
}

void c_gpu_device_d3d11::handle_d3d11_error(
	const char	*function_name,
	const char	*description,
	HRESULT		 result)
{
	switch (result)
	{
	case D3D11_ERROR_FILE_NOT_FOUND:
		m_logger->error("%s %s (%s)", function_name, description, "file was not found.");
		break;

	case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
		m_logger->error("%s %s (%s)", 
			function_name, 
			description, 
			"There are too many unique instances of a particular type of state object.");
		break;

	case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
		m_logger->error("%s %s (%s)", 
			function_name, 
			description, 
			"There are too many unique instances of a particular type of view object.");
		break;

	case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
		m_logger->error("%s %s (%s)", 
			function_name, 
			description, 
			"The first call to ID3D11DeviceContext::Map after either "
			"ID3D11Device::CreateDeferredContext or ID3D11DeviceContext::FinishCommandList per "
			"Resource was not D3D11_MAP_WRITE_DISCARD.");
		break;

	case E_FAIL:
		m_logger->error("%s %s (%s)", 
			function_name, 
			description, 
			"Attempted to create a device with the debug layer enabled and the "
			"layer is not installed.");
		break;

	case E_INVALIDARG:
		m_logger->error("%s %s (%s)", 
			function_name, 
			description, 
			"An invalid parameter was passed to the returning function.");
		break;

	case E_OUTOFMEMORY:
		m_logger->error("%s %s (%s)", 
			function_name, 
			description, 
			"Direct3D could not allocate sufficient memory to complete the call.");
		break;

	case E_NOTIMPL:
		m_logger->error("%s %s (%s)", 
			function_name, 
			description, 
			"The method call isn't implemented with the passed parameter combination.");
		break;

	}
}

void c_gpu_device_d3d11::present()
{
	if (m_swap_chain)
	{
		m_swap_chain->Present(0,0);
	}
}

void c_gpu_device_d3d11::resize(
	uint32 width, 
	uint32 height)
{
	if (width != m_width || height != m_height)
	{
		m_width= width;
		m_height= height;

		assert(m_device_context);

		m_device_context->release_from_swap_chain();

		if (m_width > 0 && m_height > 0)
		{
			HRESULT result= m_swap_chain->ResizeBuffers(0, width, height, k_d3d11_back_buffer_format, 0);
			handle_d3d11_error("c_gpu_device_d3d11::resize()", "", result);

			m_device_context->attach_to_swap_chain(this);
		}
	}
}
