#include <Windows.h>
#include <d3d12.h>
#include "dllmain.h"
#include "logger.h"
#include "WrappedDevice.h"

extern "C" void call_original(void);
extern "C" FARPROC wrapPtr = NULL;

static HMODULE d3d12dll = nullptr;

PFN_D3D12CreateDevice m_D3D12CreateDevice;

FARPROC m_SetAppCompatStringPointer;
//FARPROC m_D3D12CreateDevice;
FARPROC m_D3D12GetDebugInterface;
FARPROC m_D3D12CoreCreateLayeredDevice;
FARPROC m_D3D12CoreGetLayeredDeviceSize;
FARPROC m_D3D12CoreRegisterLayers;
FARPROC m_D3D12CreateRootSignatureDeserializer;
FARPROC m_D3D12CreateVersionedRootSignatureDeserializer;
FARPROC m_D3D12DeviceRemovedExtendedData;
FARPROC m_D3D12EnableExperimentalFeatures;
FARPROC m_D3D12GetInterface;
FARPROC m_D3D12PIXEventsReplaceBlock;
FARPROC m_D3D12PIXGetThreadInfo;
FARPROC m_D3D12PIXNotifyWakeFromFenceSignal;
FARPROC m_D3D12PIXReportCounter;
FARPROC m_D3D12SerializeRootSignature;
FARPROC m_D3D12SerializeVersionedRootSignature;
FARPROC m_GetBehaviorValue;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH: 
        {
            Logger::Init();
            Logger::LogInfo() << "DLL PROCESS ATTACH - Successfully" << std::endl;

            char path[MAX_PATH];
            GetSystemDirectoryA(path, MAX_PATH);
            strcat_s(path, "\\d3d12.dll");
            Logger::LogInfo() << "Loading " << path << std::endl;
            d3d12dll = LoadLibraryA(path);

            m_D3D12CreateDevice = (PFN_D3D12CreateDevice)GetProcAddress(d3d12dll, "D3D12CreateDevice");
            m_SetAppCompatStringPointer = GetProcAddress(d3d12dll,"SetAppCompatStringPointer");
            m_D3D12GetDebugInterface = GetProcAddress(d3d12dll, "D3D12GetDebugInterface");
            m_D3D12CoreCreateLayeredDevice = GetProcAddress(d3d12dll, "D3D12CoreCreateLayeredDevice");
            m_D3D12CoreGetLayeredDeviceSize = GetProcAddress(d3d12dll, "D3D12CoreGetLayeredDeviceSize");
            m_D3D12CoreRegisterLayers = GetProcAddress(d3d12dll, "D3D12CoreRegisterLayers");
            m_D3D12CreateRootSignatureDeserializer = GetProcAddress(d3d12dll, "D3D12CreateRootSignatureDeserializer");
            m_D3D12CreateVersionedRootSignatureDeserializer = GetProcAddress(d3d12dll, "D3D12CreateVersionedRootSignatureDeserializer");
            m_D3D12DeviceRemovedExtendedData = GetProcAddress(d3d12dll, "D3D12DeviceRemovedExtendedData");
            m_D3D12EnableExperimentalFeatures = GetProcAddress(d3d12dll, "D3D12EnableExperimentalFeatures");
            m_D3D12GetInterface = GetProcAddress(d3d12dll, "D3D12GetInterface");
            m_D3D12PIXEventsReplaceBlock = GetProcAddress(d3d12dll, "D3D12PIXEventsReplaceBlock");
            m_D3D12PIXGetThreadInfo = GetProcAddress(d3d12dll, "D3D12PIXGetThreadInfo");
            m_D3D12PIXNotifyWakeFromFenceSignal = GetProcAddress(d3d12dll, "D3D12PIXNotifyWakeFromFenceSignal");
            m_D3D12PIXReportCounter = GetProcAddress(d3d12dll, "D3D12PIXReportCounter");
            m_D3D12SerializeRootSignature = GetProcAddress(d3d12dll, "D3D12SerializeRootSignature");
            m_D3D12SerializeVersionedRootSignature = GetProcAddress(d3d12dll, "D3D12SerializeVersionedRootSignature");
            m_GetBehaviorValue = GetProcAddress(d3d12dll, "GetBehaviorValue");

            break;
        }
        
        case DLL_PROCESS_DETACH: 
        {
            Logger::LogInfo() << "DLL PROCESS DETACH - Successfully" << std::endl;
            Logger::Shutdown();

            FreeLibrary(d3d12dll);
            break;
        }
    }
    return TRUE;
}

HRESULT Wrap_D3D12CreateDevice(IUnknown* pAdapter, D3D_FEATURE_LEVEL MinimumFeatureLevel, REFIID riid, void** ppDevice)
{
    HRESULT original = S_OK;

    D3D_FEATURE_LEVEL ModifiedFeatureLevel = D3D_FEATURE_LEVEL_12_1;

    if (m_D3D12CreateDevice) Logger::LogInfo() << "D3D12CreateDevice - Address Available" << std::endl;
    else {
                             Logger::LogError() << "D3D12CreateDevice - Address Unavailable" << std::endl;
                             return S_FALSE;
    }

    original = m_D3D12CreateDevice(pAdapter, ModifiedFeatureLevel, riid, ppDevice);
    if (original == S_OK) Logger::LogDebug() << "D3D12CreateDevice - Success" << std::endl;
    if (original == S_FALSE) Logger::LogDebug() << "D3D12CreateDevice - Failed" << std::endl;

    if(original >= 0x887A0000) Logger::LogError() << "Possible DXGI_ERROR code" << std::endl;

    WrappedD3D12Device* wrapped = new WrappedD3D12Device((ID3D12Device*)*ppDevice);
    *ppDevice = (ID3D12Device10*)wrapped;

    if (FAILED(original))
        Logger::LogError() << "D3D12CreateDevice failed with HRESULT: 0x" << std::hex << original << std::endl;

    return S_OK;
}

HRESULT Wrap_D3D12CreateRootSignatureDeserializer(LPCVOID pSrcData, SIZE_T  SrcDataSizeInBytes, REFIID  pRootSignatureDeserializerInterface, void** ppRootSignatureDeserializer)
{
    Logger::LogInfo() << "Wrap_D3D12CreateRootSignatureDeserializer" << std::endl;
    return ((PFN_D3D12CreateRootSignatureDeserializer)m_D3D12CreateRootSignatureDeserializer)(pSrcData, SrcDataSizeInBytes, pRootSignatureDeserializerInterface, ppRootSignatureDeserializer);
}

HRESULT Wrap_D3D12CreateVersionedRootSignatureDeserializer(LPCVOID pSrcData, SIZE_T SrcDataSizeInBytes, REFIID pRootSignatureDeserializerInterface, void** ppRootSignatureDeserializer)
{
    Logger::LogInfo() << "Wrap_D3D12CreateVersionedRootSignatureDeserializer" << std::endl;
    return ((PFN_D3D12CreateVersionedRootSignatureDeserializer)m_D3D12CreateVersionedRootSignatureDeserializer)(pSrcData, SrcDataSizeInBytes, pRootSignatureDeserializerInterface, ppRootSignatureDeserializer);
}

HRESULT Wrap_D3D12EnableExperimentalFeatures(UINT NumFeatures, const IID* pIIDs, void* pConfigurationStructs, UINT* pConfigurationStructSizes)
{
    Logger::LogInfo() << "Wrap_D3D12EnableExperimentalFeatures" << std::endl;
    return ((PFN_D3D12EnableExperimentalFeatures)m_D3D12EnableExperimentalFeatures)(NumFeatures, pIIDs, pConfigurationStructs, pConfigurationStructSizes);
}

HRESULT Wrap_D3D12GetDebugInterface(REFIID riid, void** ppvDebug)
{
    Logger::LogInfo() << "Wrap_D3D12GetDebugInterface" << std::endl;
    return ((PFN_D3D12GetDebugInterface)m_D3D12GetDebugInterface)(riid, ppvDebug);
}

HRESULT Wrap_D3D12GetInterface(REFCLSID rclsid, REFIID riid, void** ppvDebug)
{
    Logger::LogInfo() << "Wrap_D3D12GetInterface" << std::endl;
    return ((PFN_D3D12GetInterface)m_D3D12GetInterface)(rclsid, riid, ppvDebug);
}

HRESULT Wrap_D3D12SerializeRootSignature(const D3D12_ROOT_SIGNATURE_DESC* pRootSignature, D3D_ROOT_SIGNATURE_VERSION Version, ID3DBlob** ppBlob, ID3DBlob** ppErrorBlob)
{
    Logger::LogInfo() << "Wrap_D3D12SerializeRootSignature" << std::endl;
    return ((PFN_D3D12SerializeRootSignature)m_D3D12SerializeRootSignature)(pRootSignature, Version, ppBlob, ppErrorBlob);
}

HRESULT Wrap_D3D12SerializeVersionedRootSignature(const D3D12_VERSIONED_ROOT_SIGNATURE_DESC* pRootSignature, ID3DBlob** ppBlob, ID3DBlob** ppErrorBlob)
{
    //Logger::LogInfo() << "Wrap_D3D12SerializeVersionedRootSignature" << std::endl;
    return ((PFN_D3D12SerializeVersionedRootSignature)m_D3D12SerializeVersionedRootSignature)(pRootSignature, ppBlob, ppErrorBlob);
}

extern "C" {
    void Wrap_D3D12CoreCreateLayeredDevice()
    {
        Logger::LogInfo() << "Wrap_D3D12CoreCreateLayeredDevice" << std::endl;
        wrapPtr = m_D3D12CoreCreateLayeredDevice;
        call_original();
    }

    void Wrap_D3D12CoreGetLayeredDeviceSize()
    {
        Logger::LogInfo() << "Wrap_D3D12CoreGetLayeredDeviceSize" << std::endl;
        wrapPtr = m_D3D12CoreGetLayeredDeviceSize;
        call_original();
    }

    void Wrap_D3D12DeviceRemovedExtendedData()
    {
        Logger::LogInfo() << "Wrap_D3D12DeviceRemovedExtendedData" << std::endl;
        wrapPtr = m_D3D12DeviceRemovedExtendedData;
        call_original();
    }

    void Wrap_D3D12PIXEventsReplaceBlock()
    {
        Logger::LogInfo() << "Wrap_D3D12PIXEventsReplaceBlock" << std::endl;
        wrapPtr = m_D3D12PIXEventsReplaceBlock;
        call_original();
    }

    void Wrap_D3D12PIXGetThreadInfo()
    {
        Logger::LogInfo() << "Wrap_D3D12PIXGetThreadInfo" << std::endl;
        wrapPtr = m_D3D12PIXGetThreadInfo;
        call_original();
    }

    void Wrap_D3D12PIXNotifyWakeFromFenceSignal()
    {
        Logger::LogInfo() << "Wrap_D3D12PIXNotifyWakeFromFenceSignal" << std::endl;
        wrapPtr = m_D3D12PIXNotifyWakeFromFenceSignal;
        call_original();
    }

    void Wrap_D3D12PIXReportCounter()
    {
        Logger::LogInfo() << "Wrap_D3D12PIXReportCounter" << std::endl;
        wrapPtr = m_D3D12PIXReportCounter;
        call_original();
    }

    void Wrap_GetBehaviorValue()
    {
        Logger::LogInfo() << "Wrap_GetBehaviorValue" << std::endl;
        wrapPtr = m_GetBehaviorValue;
        call_original();
    }

    void Wrap_SetAppCompatStringPointer()
    {
        Logger::LogInfo() << "Wrap_SetAppCompatStringPointer" << std::endl;
        wrapPtr = m_SetAppCompatStringPointer;
        call_original();
    }

    void Wrap_D3D12CoreRegisterLayers()
    {
        Logger::LogInfo() << "Wrap_D3D12CoreRegisterLayers" << std::endl;
        wrapPtr = m_D3D12CoreRegisterLayers;
        call_original();
    }
}