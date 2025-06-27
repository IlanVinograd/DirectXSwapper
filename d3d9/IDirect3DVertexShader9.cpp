#include "d3d9.h"

HRESULT m_IDirect3DVertexShader9::QueryInterface(THIS_ REFIID riid, void** ppvObj)
{
	if ((riid == IID_IDirect3DVertexShader9 || riid == IID_IUnknown) && ppvObj)
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

ULONG m_IDirect3DVertexShader9::AddRef(THIS)
{
	return ProxyInterface->AddRef();
}

ULONG m_IDirect3DVertexShader9::Release(THIS)
{
	return ProxyInterface->Release();
}

HRESULT m_IDirect3DVertexShader9::GetDevice(THIS_ IDirect3DDevice9** ppDevice)
{
	if (!ppDevice)
	{
		return D3DERR_INVALIDCALL;
	}

	m_pDeviceEx->AddRef();

	*ppDevice = m_pDeviceEx;

	return D3D_OK;
}

HRESULT m_IDirect3DVertexShader9::GetFunction(THIS_ void* pData, UINT* pSizeOfData)
{
	return ProxyInterface->GetFunction(pData, pSizeOfData);
}
