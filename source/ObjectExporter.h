#pragma once
#include <unordered_set>
#include <queue>
#include "d3d9.h"
#include "d3dx9.h"
#include "logger.h"
#include <functional>
#include "OverlayUI.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>

#include <condition_variable>
#include <mutex>
#include <atomic>
#include <thread>

struct ObjectDescriptor {
	size_t hash;
	std::vector<uint8_t> vertexData;
	std::vector<uint8_t> indexData;
	UINT stride;
	D3DPRIMITIVETYPE primitiveType;
	INT baseVertexIndex;
	UINT startIndex;
	UINT primitiveCount;
	bool index32bit;
};

class ObjectExporter {
public:
	static void SaveToObj(ObjectDescriptor& obj, const std::string& path);
	static void LockAndFillVertexBuffer(ObjectDescriptor& obj, LPDIRECT3DVERTEXBUFFER9 vb, UINT stride);

	static size_t ComputeHash(const ObjectDescriptor& obj);
	static void StartExportWorker();
	static void StopExportWorker();

	static void EnqueueObject(ObjectDescriptor&& obj);
	static void ThreadMain();

private:

	static std::unordered_set<size_t> seenVertexHashes;

	static std::queue<ObjectDescriptor> objectQueue;
	static std::mutex queueMutex;
	static std::condition_variable cv;
	static std::atomic<bool> running;
	static bool initialized;
	static std::thread workerThread;
};