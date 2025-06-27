#pragma once
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <Windows.h>
#include "d3dx12.h"
#include <thread>
#include <vector>
#include <unordered_set>
#include "logger.h"

const UINT64 kMinSaveSize = 2 * 1024;
const UINT64 kMaxSaveSize = 1024ull * 1024 * 1024;

void RunAsyncExport(void* vbData, UINT vbSize, UINT stride,
    void* ibData, UINT ibSize, DXGI_FORMAT format);

void ExportToObj(const void* vbData, UINT vbSize, UINT stride,
    const void* ibData, UINT ibSize, DXGI_FORMAT format);

size_t HashBuffers(const void* vbData, size_t vbSize, const void* ibData, size_t ibSize);