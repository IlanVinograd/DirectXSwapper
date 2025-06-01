#include "d3d9.h"

HRESULT m_IDirect3DSwapChain9Ex::QueryInterface(THIS_ REFIID riid, void** ppvObj)
{
	if ((riid == IID_IDirect3DSwapChain9 || riid == IID_IUnknown) && ppvObj)
	{
		AddRef();

		*ppvObj = this;

		return S_OK;
	}

	HRESULT hr = ProxyInterface->QueryInterface(riid, ppvObj);

	if (SUCCEEDED(hr))
	{
		genericQueryInterface(riid, ppvObj, m_pDeviceEx);
	}

	return hr;
}

ULONG m_IDirect3DSwapChain9Ex::AddRef(THIS)
{
	return ProxyInterface->AddRef();
}

ULONG m_IDirect3DSwapChain9Ex::Release(THIS)
{
	return ProxyInterface->Release();
}

HRESULT m_IDirect3DSwapChain9Ex::Present(THIS_ CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags)
{
	return ProxyInterface->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}

HRESULT m_IDirect3DSwapChain9Ex::GetFrontBufferData(THIS_ IDirect3DSurface9* pDestSurface)
{
	if (pDestSurface)
	{
		pDestSurface = static_cast<m_IDirect3DSurface9 *>(pDestSurface)->GetProxyInterface();
	}

	return GetFrontBufferData(pDestSurface);
}

HRESULT m_IDirect3DSwapChain9Ex::GetBackBuffer(THIS_ UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
	HRESULT hr = ProxyInterface->GetBackBuffer(BackBuffer, Type, ppBackBuffer);

	if (SUCCEEDED(hr) && ppBackBuffer)
	{
		*ppBackBuffer = m_pDeviceEx->ProxyAddressLookupTable->FindAddress<m_IDirect3DSurface9>(*ppBackBuffer);
	}

	return hr;
}

HRESULT m_IDirect3DSwapChain9Ex::GetRasterStatus(THIS_ D3DRASTER_STATUS* pRasterStatus)
{
	return ProxyInterface->GetRasterStatus(pRasterStatus);
}

HRESULT m_IDirect3DSwapChain9Ex::GetDisplayMode(THIS_ D3DDISPLAYMODE* pMode)
{
	return ProxyInterface->GetDisplayMode(pMode);
}

HRESULT m_IDirect3DSwapChain9Ex::GetDevice(THIS_ IDirect3DDevice9** ppDevice)
{
	if (!ppDevice)
	{
		return D3DERR_INVALIDCALL;
	}

	m_pDeviceEx->AddRef();

	*ppDevice = m_pDeviceEx;

	return D3D_OK;
}

HRESULT m_IDirect3DSwapChain9Ex::GetPresentParameters(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	return ProxyInterface->GetPresentParameters(pPresentationParameters);
}

HRESULT m_IDirect3DSwapChain9Ex::GetLastPresentCount(THIS_ UINT* pLastPresentCount)
{
	return ProxyInterface->GetLastPresentCount(pLastPresentCount);
}

HRESULT m_IDirect3DSwapChain9Ex::GetPresentStats(THIS_ D3DPRESENTSTATS* pPresentationStatistics)
{
	return ProxyInterface->GetPresentStats(pPresentationStatistics);
}

HRESULT m_IDirect3DSwapChain9Ex::GetDisplayModeEx(THIS_ D3DDISPLAYMODEEX* pMode, D3DDISPLAYROTATION* pRotation)
{
	return ProxyInterface->GetDisplayModeEx(pMode, pRotation);
}
