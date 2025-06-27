#include "WrappedCommandQueue.h"
#include "WrappedCommandList.h"
#include "logger.h"

void STDMETHODCALLTYPE WrappedCommandQueue::ExecuteCommandLists(UINT NumCommandLists, ID3D12CommandList* const* ppCommandLists)
{
    std::vector<ID3D12CommandList*> realLists;
    realLists.reserve(NumCommandLists);

    for (UINT i = 0; i < NumCommandLists; ++i)
    {
        auto wrapped = dynamic_cast<WrappedCommandList*>(ppCommandLists[i]);
        if (wrapped)
        {
            //Logger::LogInfo() << "Unwrapping WrappedCommandList at index: " << i << std::endl;
            realLists.push_back(wrapped->GetBase());
        }
        else
        {
            //Logger::LogWarning() << "Non-wrapped CommandList at index: " << i << std::endl;
            realLists.push_back(ppCommandLists[i]);
        }
    }

    m_real->ExecuteCommandLists(NumCommandLists, realLists.data());
}