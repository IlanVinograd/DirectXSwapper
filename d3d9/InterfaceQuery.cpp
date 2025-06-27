#include "d3d9.h"

void genericQueryInterface(REFIID riid, LPVOID *ppvObj, m_IDirect3DDevice9Ex* m_pDeviceEx)
{
	if (!ppvObj || !*ppvObj || !m_pDeviceEx)
	{
		return;
	}

	if (riid == IID_IDirect3D9 || riid == IID_IDirect3D9Ex)
	{
		IDirect3D9 *pD3D9 = nullptr;
		if (SUCCEEDED(m_pDeviceEx->GetDirect3D(&pD3D9)) && pD3D9)
		{
			IDirect3D9 *pD3D9wrapper = nullptr;
			if (SUCCEEDED(pD3D9->QueryInterface(riid, (LPVOID*)&pD3D9wrapper)) && pD3D9wrapper)
			{
				pD3D9wrapper->Release();
			}
			pD3D9->Release();
			return;
		}
	}

	if (riid == IID_IDirect3DDevice9 || riid == IID_IDirect3DDevice9Ex)
	{
		IDirect3DDevice9 *pD3DDevice9 = nullptr;
		if (SUCCEEDED(m_pDeviceEx->QueryInterface(riid, (LPVOID*)&pD3DDevice9)) && pD3DDevice9)
		{
			pD3DDevice9->Release();
		}
		return;
	}

	if (riid == IID_IDirect3DSwapChain9 || riid == IID_IDirect3DSwapChain9Ex)
	{
		*ppvObj = m_pDeviceEx->ProxyAddressLookupTable->FindAddress<m_IDirect3DSwapChain9Ex>(*ppvObj, riid);
		return;
	}

#define QUERYINTERFACE(x) \
	if (riid == IID_ ## x) \
		{ \
			*ppvObj = m_pDeviceEx->ProxyAddressLookupTable->FindAddress<m_ ## x>(*ppvObj); \
			return; \
		}

	QUERYINTERFACE(IDirect3DCubeTexture9);
	QUERYINTERFACE(IDirect3DIndexBuffer9);
	QUERYINTERFACE(IDirect3DPixelShader9);
	QUERYINTERFACE(IDirect3DQuery9);
	QUERYINTERFACE(IDirect3DStateBlock9);
	QUERYINTERFACE(IDirect3DSurface9);
	QUERYINTERFACE(IDirect3DTexture9);
	QUERYINTERFACE(IDirect3DVertexBuffer9);
	QUERYINTERFACE(IDirect3DVertexDeclaration9);
	QUERYINTERFACE(IDirect3DVertexShader9);
	QUERYINTERFACE(IDirect3DVolume9);
	QUERYINTERFACE(IDirect3DVolumeTexture9);
}
