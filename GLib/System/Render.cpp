#include <d3d11.h>

#include <assert.h>

#include "System\Device.h"

namespace GLib
{
	bool BeginRendering(const float i_ClearColor[4])
	{
		assert(g_pSwapChain);

		SafeDeviceInterface<ID3D11DeviceContext> ImmediateContext = GetImmediateContext();

		// Just clear the backbuffer
		ImmediateContext->ClearRenderTargetView(g_pRenderTargetView, i_ClearColor);

		if (g_pDepthStencilView)
			ImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		return true;
	}

	void EndRendering(void)
	{
		HRESULT hResult = g_pSwapChain->Present(0, 0);
		if (hResult == DXGI_ERROR_DEVICE_REMOVED)
		{
			hResult = g_pD3DDevice->GetDeviceRemovedReason();
		}
		assert(hResult == S_OK);
	}

} // namespace GLib