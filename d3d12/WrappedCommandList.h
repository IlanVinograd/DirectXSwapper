#pragma once
#include <d3d12.h>
#include "logger.h"

struct ResourceRange {
    D3D12_GPU_VIRTUAL_ADDRESS base;
    UINT64 size;
    ID3D12Resource* resource;
};

extern D3D12_VERTEX_BUFFER_VIEW lastVBView;
extern D3D12_INDEX_BUFFER_VIEW lastIBView;
extern std::vector<ResourceRange> g_Resources;

class WrappedCommandList : public ID3D12GraphicsCommandList {
public:
    WrappedCommandList(ID3D12GraphicsCommandList* real) : m_real(real) {}
    virtual ~WrappedCommandList() {}

    // IUnknown
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override { return m_real->QueryInterface(riid, ppvObject); }
    ULONG STDMETHODCALLTYPE AddRef() override { return m_real->AddRef(); }
    ULONG STDMETHODCALLTYPE Release() override { return m_real->Release(); }

    // ID3D12GraphicsCommandList
    HRESULT STDMETHODCALLTYPE Close() override { return m_real->Close(); }
    HRESULT STDMETHODCALLTYPE Reset(ID3D12CommandAllocator* pAllocator, ID3D12PipelineState* pInitialState) override {
        return m_real->Reset(pAllocator, pInitialState);
    }
    void STDMETHODCALLTYPE DrawInstanced(UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation) override;
    void STDMETHODCALLTYPE DrawIndexedInstanced(UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation) override;
    void STDMETHODCALLTYPE Dispatch(UINT ThreadGroupCountX, UINT ThreadGroupCountY, UINT ThreadGroupCountZ) override {
        m_real->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
    }
    void STDMETHODCALLTYPE CopyBufferRegion(ID3D12Resource* pDstBuffer, UINT64 DstOffset,
        ID3D12Resource* pSrcBuffer, UINT64 SrcOffset, UINT64 NumBytes) override {
        m_real->CopyBufferRegion(pDstBuffer, DstOffset, pSrcBuffer, SrcOffset, NumBytes);
    }
    void STDMETHODCALLTYPE CopyTextureRegion(const D3D12_TEXTURE_COPY_LOCATION* pDst, UINT DstX, UINT DstY, UINT DstZ,
        const D3D12_TEXTURE_COPY_LOCATION* pSrc, const D3D12_BOX* pSrcBox) override {
        m_real->CopyTextureRegion(pDst, DstX, DstY, DstZ, pSrc, pSrcBox);
    }
    void STDMETHODCALLTYPE CopyResource(ID3D12Resource* pDstResource, ID3D12Resource* pSrcResource) override {
        m_real->CopyResource(pDstResource, pSrcResource);
    }
    void STDMETHODCALLTYPE CopyTiles(ID3D12Resource* pTiledResource, const D3D12_TILED_RESOURCE_COORDINATE* pTileRegionStartCoordinate,
        const D3D12_TILE_REGION_SIZE* pTileRegionSize, ID3D12Resource* pBuffer, UINT64 BufferStartOffsetInBytes,
        D3D12_TILE_COPY_FLAGS Flags) override {
        m_real->CopyTiles(pTiledResource, pTileRegionStartCoordinate, pTileRegionSize, pBuffer, BufferStartOffsetInBytes, Flags);
    }
    void STDMETHODCALLTYPE ResolveSubresource(ID3D12Resource* pDstResource, UINT DstSubresource,
        ID3D12Resource* pSrcResource, UINT SrcSubresource, DXGI_FORMAT Format) override {
        m_real->ResolveSubresource(pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format);
    }
    void STDMETHODCALLTYPE IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY PrimitiveTopology) override {
        m_real->IASetPrimitiveTopology(PrimitiveTopology);
    }
    void STDMETHODCALLTYPE RSSetViewports(UINT NumViewports, const D3D12_VIEWPORT* pViewports) override {
        m_real->RSSetViewports(NumViewports, pViewports);
    }
    void STDMETHODCALLTYPE RSSetScissorRects(UINT NumRects, const D3D12_RECT* pRects) override {
        m_real->RSSetScissorRects(NumRects, pRects);
    }
    void STDMETHODCALLTYPE OMSetRenderTargets(UINT NumRTVs, const D3D12_CPU_DESCRIPTOR_HANDLE* pRTVs,
        BOOL RTsSingleHandleToDescriptorRange, const D3D12_CPU_DESCRIPTOR_HANDLE* pDSV) override {
        m_real->OMSetRenderTargets(NumRTVs, pRTVs, RTsSingleHandleToDescriptorRange, pDSV);
    }
    void STDMETHODCALLTYPE ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetView, const FLOAT ColorRGBA[4], UINT NumRects, const D3D12_RECT* pRects) override {
        m_real->ClearRenderTargetView(RenderTargetView, ColorRGBA, NumRects, pRects);
    }
    void STDMETHODCALLTYPE ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView, D3D12_CLEAR_FLAGS ClearFlags,
        FLOAT Depth, UINT8 Stencil, UINT NumRects, const D3D12_RECT* pRects) override {
        m_real->ClearDepthStencilView(DepthStencilView, ClearFlags, Depth, Stencil, NumRects, pRects);
    }
    void STDMETHODCALLTYPE OMSetBlendFactor(const FLOAT BlendFactor[4]) override {
        m_real->OMSetBlendFactor(BlendFactor);
    }
    void STDMETHODCALLTYPE OMSetStencilRef(UINT StencilRef) override {
        m_real->OMSetStencilRef(StencilRef);
    }
    void STDMETHODCALLTYPE SetPipelineState(ID3D12PipelineState* pPipelineState) override {
        m_real->SetPipelineState(pPipelineState);
    }
    void STDMETHODCALLTYPE ResourceBarrier(UINT NumBarriers, const D3D12_RESOURCE_BARRIER* pBarriers) override {
        m_real->ResourceBarrier(NumBarriers, pBarriers);
    }
    void STDMETHODCALLTYPE ExecuteBundle(ID3D12GraphicsCommandList* pCommandList) override {
        m_real->ExecuteBundle(pCommandList);
    }
    void STDMETHODCALLTYPE SetDescriptorHeaps(UINT NumDescriptorHeaps, ID3D12DescriptorHeap* const* ppDescriptorHeaps) override {
        m_real->SetDescriptorHeaps(NumDescriptorHeaps, ppDescriptorHeaps);
    }
    void STDMETHODCALLTYPE SetComputeRootSignature(ID3D12RootSignature* pRootSignature) override {
        m_real->SetComputeRootSignature(pRootSignature);
    }
    void STDMETHODCALLTYPE SetGraphicsRootSignature(ID3D12RootSignature* pRootSignature) override {
        m_real->SetGraphicsRootSignature(pRootSignature);
    }
    void STDMETHODCALLTYPE SetComputeRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor) override {
        m_real->SetComputeRootDescriptorTable(RootParameterIndex, BaseDescriptor);
    }
    void STDMETHODCALLTYPE SetGraphicsRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor) override {
        m_real->SetGraphicsRootDescriptorTable(RootParameterIndex, BaseDescriptor);
    }
    void STDMETHODCALLTYPE SetComputeRoot32BitConstant(UINT RootParameterIndex, UINT SrcData, UINT DestOffsetIn32BitValues) override {
        m_real->SetComputeRoot32BitConstant(RootParameterIndex, SrcData, DestOffsetIn32BitValues);
    }
    void STDMETHODCALLTYPE SetGraphicsRoot32BitConstant(UINT RootParameterIndex, UINT SrcData, UINT DestOffsetIn32BitValues) override {
        m_real->SetGraphicsRoot32BitConstant(RootParameterIndex, SrcData, DestOffsetIn32BitValues);
    }
    void STDMETHODCALLTYPE SetComputeRoot32BitConstants(UINT RootParameterIndex, UINT Num32BitValuesToSet,
        const void* pSrcData, UINT DestOffsetIn32BitValues) override {
        m_real->SetComputeRoot32BitConstants(RootParameterIndex, Num32BitValuesToSet, pSrcData, DestOffsetIn32BitValues);
    }
    void STDMETHODCALLTYPE SetGraphicsRoot32BitConstants(UINT RootParameterIndex, UINT Num32BitValuesToSet,
        const void* pSrcData, UINT DestOffsetIn32BitValues) override {
        m_real->SetGraphicsRoot32BitConstants(RootParameterIndex, Num32BitValuesToSet, pSrcData, DestOffsetIn32BitValues);
    }
    void STDMETHODCALLTYPE SetComputeRootConstantBufferView(UINT RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) override {
        m_real->SetComputeRootConstantBufferView(RootParameterIndex, BufferLocation);
    }
    void STDMETHODCALLTYPE SetGraphicsRootConstantBufferView(UINT RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) override {
        m_real->SetGraphicsRootConstantBufferView(RootParameterIndex, BufferLocation);
    }
    void STDMETHODCALLTYPE SetComputeRootShaderResourceView(UINT RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) override {
        m_real->SetComputeRootShaderResourceView(RootParameterIndex, BufferLocation);
    }
    void STDMETHODCALLTYPE SetGraphicsRootShaderResourceView(UINT RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) override;

    void STDMETHODCALLTYPE SetComputeRootUnorderedAccessView(UINT RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) override {
        m_real->SetComputeRootUnorderedAccessView(RootParameterIndex, BufferLocation);
    }
    void STDMETHODCALLTYPE SetGraphicsRootUnorderedAccessView(UINT RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) override {
        m_real->SetGraphicsRootUnorderedAccessView(RootParameterIndex, BufferLocation);
    }
    void STDMETHODCALLTYPE IASetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW* pView) override;
    void STDMETHODCALLTYPE IASetVertexBuffers(UINT StartSlot, UINT NumViews, const D3D12_VERTEX_BUFFER_VIEW* pViews) override;
    void STDMETHODCALLTYPE SOSetTargets(UINT StartSlot, UINT NumViews, const D3D12_STREAM_OUTPUT_BUFFER_VIEW* pViews) override {
        m_real->SOSetTargets(StartSlot, NumViews, pViews);
    }

    void STDMETHODCALLTYPE BeginEvent(UINT Metadata, const void* pData, UINT Size) override {
        m_real->BeginEvent(Metadata, pData, Size);
    }

    void STDMETHODCALLTYPE EndEvent() override {
        m_real->EndEvent();
    }

    void STDMETHODCALLTYPE SetMarker(UINT Metadata, const void* pData, UINT Size) override {
        m_real->SetMarker(Metadata, pData, Size);
    }

    HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID guid, UINT DataSize, const void* pData) override {
        return m_real->SetPrivateData(guid, DataSize, pData);
    }

    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(REFGUID guid, const IUnknown* pData) override {
        return m_real->SetPrivateDataInterface(guid, pData);
    }

    HRESULT STDMETHODCALLTYPE SetName(LPCWSTR Name) override {
        return m_real->SetName(Name);
    }

    HRESULT STDMETHODCALLTYPE GetDevice(REFIID riid, void** ppvDevice) override {
        return m_real->GetDevice(riid, ppvDevice);
    }

    HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID guid, UINT* pDataSize, void* pData) override {
        return m_real->GetPrivateData(guid, pDataSize, pData);
    }

    D3D12_COMMAND_LIST_TYPE STDMETHODCALLTYPE GetType() override {
        return m_real->GetType();
    }

    void STDMETHODCALLTYPE ClearUnorderedAccessViewUint(D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap,
        D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle, ID3D12Resource* pResource,
        const UINT Values[4], UINT NumRects, const D3D12_RECT* pRects) override {
        m_real->ClearUnorderedAccessViewUint(ViewGPUHandleInCurrentHeap, ViewCPUHandle, pResource, Values, NumRects, pRects);
    }

    void STDMETHODCALLTYPE ClearUnorderedAccessViewFloat(D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap,
        D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle, ID3D12Resource* pResource,
        const FLOAT Values[4], UINT NumRects, const D3D12_RECT* pRects) override {
        m_real->ClearUnorderedAccessViewFloat(ViewGPUHandleInCurrentHeap, ViewCPUHandle, pResource, Values, NumRects, pRects);
    }

    void STDMETHODCALLTYPE DiscardResource(ID3D12Resource* pResource, const D3D12_DISCARD_REGION* pRegion) override {
        m_real->DiscardResource(pResource, pRegion);
    }

    void STDMETHODCALLTYPE BeginQuery(ID3D12QueryHeap* pQueryHeap, D3D12_QUERY_TYPE Type, UINT Index) override {
        m_real->BeginQuery(pQueryHeap, Type, Index);
    }

    void STDMETHODCALLTYPE EndQuery(ID3D12QueryHeap* pQueryHeap, D3D12_QUERY_TYPE Type, UINT Index) override {
        m_real->EndQuery(pQueryHeap, Type, Index);
    }

    void STDMETHODCALLTYPE ResolveQueryData(ID3D12QueryHeap* pQueryHeap, D3D12_QUERY_TYPE Type, UINT StartIndex, UINT NumQueries,
        ID3D12Resource* pDestinationBuffer, UINT64 AlignedDestinationBufferOffset) override {
        m_real->ResolveQueryData(pQueryHeap, Type, StartIndex, NumQueries, pDestinationBuffer, AlignedDestinationBufferOffset);
    }

    void STDMETHODCALLTYPE SetPredication(ID3D12Resource* pBuffer, UINT64 AlignedBufferOffset, D3D12_PREDICATION_OP Operation) override {
        m_real->SetPredication(pBuffer, AlignedBufferOffset, Operation);
    }

    void STDMETHODCALLTYPE ExecuteIndirect(ID3D12CommandSignature* pCommandSignature, UINT MaxCommandCount,
        ID3D12Resource* pArgumentBuffer, UINT64 ArgumentBufferOffset,
        ID3D12Resource* pCountBuffer, UINT64 CountBufferOffset) override;

    void STDMETHODCALLTYPE ClearState(ID3D12PipelineState* pPipelineState) override {
        m_real->ClearState(pPipelineState);
    }

    ID3D12CommandList* GetBase() const { return m_real; }
private:
    ID3D12GraphicsCommandList* m_real;
};