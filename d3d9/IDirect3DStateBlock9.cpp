#include "d3d9.h"

HRESULT m_IDirect3DStateBlock9::QueryInterface(THIS_ REFIID riid, void** ppvObj)
{
	if ((riid == IID_IDirect3DStateBlock9 || riid == IID_IUnknown) && ppvObj)
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

ULONG m_IDirect3DStateBlock9::AddRef(THIS)
{
	return ProxyInterface->AddRef();
}

ULONG m_IDirect3DStateBlock9::Release(THIS)
{
	return ProxyInterface->Release();
}

HRESULT m_IDirect3DStateBlock9::GetDevice(THIS_ IDirect3DDevice9** ppDevice)
{
	if (!ppDevice)
	{
		return D3DERR_INVALIDCALL;
	}

	m_pDeviceEx->AddRef();

	*ppDevice = m_pDeviceEx;

	return D3D_OK;
}

HRESULT m_IDirect3DStateBlock9::Capture(THIS)
{
	return ProxyInterface->Capture();
}

HRESULT m_IDirect3DStateBlock9::Apply(THIS)
{
	return ProxyInterface->Apply();
}
