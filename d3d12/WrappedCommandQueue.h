#pragma once

#include <d3d12.h>
#include "logger.h"

class WrappedCommandQueue : public ID3D12CommandQueue {
public:
    WrappedCommandQueue(ID3D12CommandQueue* real) : m_real(real) {}

    // IUnknown
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        return m_real->QueryInterface(riid, ppvObject);
    }
    ULONG STDMETHODCALLTYPE AddRef() override {
        return m_real->AddRef();
    }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ref = m_real->Release();
        if (ref == 0) delete this;
        return ref;
    }

    // ID3D12Object
    HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID guid, UINT* pDataSize, void* pData) override { return m_real->GetPrivateData(guid, pDataSize, pData); }
    HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID guid,UINT DataSize, const void* pData) override { return m_real->SetPrivateData(guid, DataSize, pData); }
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(REFGUID guid, const IUnknown* pData) override { return m_real->SetPrivateDataInterface(guid, pData); }
    HRESULT STDMETHODCALLTYPE SetName(LPCWSTR Name) override { return m_real->SetName(Name); }

    // ID3D12DeviceChild
    HRESULT STDMETHODCALLTYPE GetDevice(REFIID riid, void** ppvDevice) override {
        return m_real->GetDevice(riid, ppvDevice);
    }

    // ID3D12CommandQueue
    void STDMETHODCALLTYPE UpdateTileMappings(ID3D12Resource* pResource, UINT NumResourceRegions, const D3D12_TILED_RESOURCE_COORDINATE* pResourceRegionStartCoordinates, const D3D12_TILE_REGION_SIZE* pResourceRegionSizes, ID3D12Heap* pHeap, UINT NumRanges, const D3D12_TILE_RANGE_FLAGS* pRangeFlags, const UINT* pHeapRangeStartOffsets, const UINT* pRangeTileCounts, D3D12_TILE_MAPPING_FLAGS Flags) override { m_real->UpdateTileMappings(pResource, NumResourceRegions, pResourceRegionStartCoordinates, pResourceRegionSizes, pHeap, NumRanges, pRangeFlags, pHeapRangeStartOffsets, pRangeTileCounts, Flags); }
    void STDMETHODCALLTYPE CopyTileMappings(ID3D12Resource* pDstResource, const D3D12_TILED_RESOURCE_COORDINATE* pDstRegionStartCoordinate, ID3D12Resource* pSrcResource, const D3D12_TILED_RESOURCE_COORDINATE* pSrcRegionStartCoordinate, const D3D12_TILE_REGION_SIZE* pRegionSize, D3D12_TILE_MAPPING_FLAGS Flags) override { m_real->CopyTileMappings(pDstResource, pDstRegionStartCoordinate, pSrcResource, pSrcRegionStartCoordinate, pRegionSize, Flags); }
    void STDMETHODCALLTYPE ExecuteCommandLists(UINT NumCommandLists, ID3D12CommandList* const* ppCommandLists) override;
    void STDMETHODCALLTYPE SetMarker(UINT Metadata, const void* pData, UINT Size) override { m_real->SetMarker(Metadata, pData, Size); }
    void STDMETHODCALLTYPE BeginEvent(UINT Metadata, const void* pData, UINT Size) override { m_real->BeginEvent(Metadata, pData, Size); }
    void STDMETHODCALLTYPE EndEvent() override { m_real->EndEvent(); }
    HRESULT STDMETHODCALLTYPE Signal(ID3D12Fence* pFence, UINT64 Value) override { return m_real->Signal(pFence, Value); }
    HRESULT STDMETHODCALLTYPE Wait(ID3D12Fence* pFence, UINT64 Value) override { return m_real->Wait(pFence, Value); }
    HRESULT STDMETHODCALLTYPE GetTimestampFrequency(UINT64* pFrequency) override { return m_real->GetTimestampFrequency(pFrequency); }
    HRESULT STDMETHODCALLTYPE GetClockCalibration(UINT64* pGpuTimestamp, UINT64* pCpuTimestamp) override { return m_real->GetClockCalibration(pGpuTimestamp, pCpuTimestamp); }
    D3D12_COMMAND_QUEUE_DESC STDMETHODCALLTYPE GetDesc() override { return m_real->GetDesc(); }

private:
    ID3D12CommandQueue* m_real;
};
