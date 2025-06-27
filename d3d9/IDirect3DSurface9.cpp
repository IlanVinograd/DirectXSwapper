#include "d3d9.h"

HRESULT m_IDirect3DSurface9::QueryInterface(THIS_ REFIID riid, void** ppvObj)
{
	if ((riid == IID_IDirect3DSurface9 || riid == IID_IUnknown || riid == IID_IDirect3DResource9) && ppvObj)
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

ULONG m_IDirect3DSurface9::AddRef(THIS)
{
	return ProxyInterface->AddRef();
}

ULONG m_IDirect3DSurface9::Release(THIS)
{
	return ProxyInterface->Release();
}

HRESULT m_IDirect3DSurface9::GetDevice(THIS_ IDirect3DDevice9** ppDevice)
{
	if (!ppDevice)
	{
		return D3DERR_INVALIDCALL;
	}

	m_pDeviceEx->AddRef();

	*ppDevice = m_pDeviceEx;

	return D3D_OK;
}

HRESULT m_IDirect3DSurface9::SetPrivateData(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return ProxyInterface->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

HRESULT m_IDirect3DSurface9::GetPrivateData(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return ProxyInterface->GetPrivateData(refguid, pData, pSizeOfData);
}

HRESULT m_IDirect3DSurface9::FreePrivateData(THIS_ REFGUID refguid)
{
	return ProxyInterface->FreePrivateData(refguid);
}

DWORD m_IDirect3DSurface9::SetPriority(THIS_ DWORD PriorityNew)
{
	return ProxyInterface->SetPriority(PriorityNew);
}

DWORD m_IDirect3DSurface9::GetPriority(THIS)
{
	return ProxyInterface->GetPriority();
}

void m_IDirect3DSurface9::PreLoad(THIS)
{
	return ProxyInterface->PreLoad();
}

D3DRESOURCETYPE m_IDirect3DSurface9::GetType(THIS)
{
	return ProxyInterface->GetType();
}

HRESULT m_IDirect3DSurface9::GetContainer(THIS_ REFIID riid, void** ppContainer)
{
	HRESULT hr = ProxyInterface->GetContainer(riid, ppContainer);

	if (SUCCEEDED(hr))
	{
		genericQueryInterface(riid, ppContainer, m_pDeviceEx);
	}

	return hr;
}

HRESULT m_IDirect3DSurface9::GetDesc(THIS_ D3DSURFACE_DESC *pDesc)
{
	return ProxyInterface->GetDesc(pDesc);
}

HRESULT m_IDirect3DSurface9::LockRect(THIS_ D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
	return ProxyInterface->LockRect(pLockedRect, pRect, Flags);
}

HRESULT m_IDirect3DSurface9::UnlockRect(THIS)
{
	return ProxyInterface->UnlockRect();
}

HRESULT m_IDirect3DSurface9::GetDC(THIS_ HDC *phdc)
{
	return ProxyInterface->GetDC(phdc);
}

HRESULT m_IDirect3DSurface9::ReleaseDC(THIS_ HDC hdc)
{
	return ProxyInterface->ReleaseDC(hdc);
}
