#include "d3d9.h"

HRESULT m_IDirect3DVertexBuffer9::QueryInterface(THIS_ REFIID riid, void** ppvObj)
{
	if ((riid == IID_IDirect3DVertexBuffer9 || riid == IID_IUnknown || riid == IID_IDirect3DResource9) && ppvObj)
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

ULONG m_IDirect3DVertexBuffer9::AddRef(THIS)
{
	return ProxyInterface->AddRef();
}

ULONG m_IDirect3DVertexBuffer9::Release(THIS)
{
	return ProxyInterface->Release();
}

HRESULT m_IDirect3DVertexBuffer9::GetDevice(THIS_ IDirect3DDevice9** ppDevice)
{
	if (!ppDevice)
	{
		return D3DERR_INVALIDCALL;
	}

	m_pDeviceEx->AddRef();

	*ppDevice = m_pDeviceEx;

	return D3D_OK;
}

HRESULT m_IDirect3DVertexBuffer9::SetPrivateData(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return ProxyInterface->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

HRESULT m_IDirect3DVertexBuffer9::GetPrivateData(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return ProxyInterface->GetPrivateData(refguid, pData, pSizeOfData);
}

HRESULT m_IDirect3DVertexBuffer9::FreePrivateData(THIS_ REFGUID refguid)
{
	return ProxyInterface->FreePrivateData(refguid);
}

DWORD m_IDirect3DVertexBuffer9::SetPriority(THIS_ DWORD PriorityNew)
{
	return ProxyInterface->SetPriority(PriorityNew);
}

DWORD m_IDirect3DVertexBuffer9::GetPriority(THIS)
{
	return ProxyInterface->GetPriority();
}

void m_IDirect3DVertexBuffer9::PreLoad(THIS)
{
	return ProxyInterface->PreLoad();
}

D3DRESOURCETYPE m_IDirect3DVertexBuffer9::GetType(THIS)
{
	return ProxyInterface->GetType();
}

HRESULT m_IDirect3DVertexBuffer9::Lock(THIS_ UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags)
{
	return ProxyInterface->Lock(OffsetToLock, SizeToLock, ppbData, Flags);
}

HRESULT m_IDirect3DVertexBuffer9::Unlock(THIS)
{
	return ProxyInterface->Unlock();
}

HRESULT m_IDirect3DVertexBuffer9::GetDesc(THIS_ D3DVERTEXBUFFER_DESC *pDesc)
{
	return ProxyInterface->GetDesc(pDesc);
}
