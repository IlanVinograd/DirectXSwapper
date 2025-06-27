#include "d3d9.h"

HRESULT m_IDirect3DQuery9::QueryInterface(THIS_ REFIID riid, void** ppvObj)
{
	if ((riid == IID_IDirect3DQuery9 || riid == IID_IUnknown) && ppvObj)
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

ULONG m_IDirect3DQuery9::AddRef(THIS)
{
	return ProxyInterface->AddRef();
}

ULONG m_IDirect3DQuery9::Release(THIS)
{
	return ProxyInterface->Release();
}

HRESULT m_IDirect3DQuery9::GetDevice(THIS_ IDirect3DDevice9** ppDevice)
{
	if (!ppDevice)
	{
		return D3DERR_INVALIDCALL;
	}

	m_pDeviceEx->AddRef();

	*ppDevice = m_pDeviceEx;

	return D3D_OK;
}

D3DQUERYTYPE m_IDirect3DQuery9::GetType(THIS)
{
	return ProxyInterface->GetType();
}

DWORD m_IDirect3DQuery9::GetDataSize(THIS)
{
	return ProxyInterface->GetDataSize();
}

HRESULT m_IDirect3DQuery9::Issue(THIS_ DWORD dwIssueFlags)
{
	return ProxyInterface->Issue(dwIssueFlags);
}

HRESULT m_IDirect3DQuery9::GetData(THIS_ void* pData, DWORD dwSize, DWORD dwGetDataFlags)
{
	return ProxyInterface->GetData(pData, dwSize, dwGetDataFlags);
}
