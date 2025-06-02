#include "ObjectExporter.h"

int logId = 0;
std::queue<ObjectDescriptor> ObjectExporter::objectQueue;
std::mutex ObjectExporter::queueMutex;
std::condition_variable ObjectExporter::cv;
bool ObjectExporter::initialized = false;
std::atomic<bool> ObjectExporter::running = false;
std::thread ObjectExporter::workerThread;
std::unordered_set<size_t> ObjectExporter::seenVertexHashes;

size_t ObjectExporter::ComputeHash(const ObjectDescriptor& obj) {
    size_t hash = 0;

    auto combine = [&](size_t value) {
        hash ^= std::hash<size_t>{}(value)+0x9e3779b9 + (hash << 6) + (hash >> 2);
        };

    std::vector<uint32_t> usedIndices;
    const size_t indexCount = obj.primitiveCount * 3;

    if (obj.index32bit) {
        const uint32_t* indices = reinterpret_cast<const uint32_t*>(obj.indexData.data());
        for (size_t i = 0; i < indexCount; ++i) {
            size_t idx = obj.startIndex + i;
            if (idx < obj.indexData.size() / sizeof(uint32_t)) {
                usedIndices.push_back(indices[idx]);
            }
        }
    }
    else {
        const uint16_t* indices = reinterpret_cast<const uint16_t*>(obj.indexData.data());
        for (size_t i = 0; i < indexCount; ++i) {
            size_t idx = obj.startIndex + i;
            if (idx < obj.indexData.size() / sizeof(uint16_t)) {
                usedIndices.push_back(static_cast<uint32_t>(indices[idx]));
            }
        }
    }

    combine(std::hash<std::string_view>{}(
        std::string_view(reinterpret_cast<const char*>(usedIndices.data()), usedIndices.size() * sizeof(uint32_t))));

    std::unordered_set<uint32_t> uniqueVertexIndices(usedIndices.begin(), usedIndices.end());

    std::vector<std::byte> usedVertexData;
    for (uint32_t idx : uniqueVertexIndices) {
        size_t vertexOffset = (obj.baseVertexIndex + idx) * obj.stride;
        if (vertexOffset + obj.stride <= obj.vertexData.size()) {
            const std::byte* vertexPtr = reinterpret_cast<const std::byte*>(obj.vertexData.data()) + vertexOffset;
            usedVertexData.insert(usedVertexData.end(), vertexPtr, vertexPtr + obj.stride);
        }
    }

    combine(std::hash<std::string_view>{}(
        std::string_view(reinterpret_cast<const char*>(usedVertexData.data()), usedVertexData.size())));

    combine(obj.stride);
    combine(obj.baseVertexIndex);
    combine(obj.startIndex);
    combine(obj.primitiveCount);
    combine(obj.primitiveType);
    combine(obj.index32bit ? 1 : 0);

    return hash;
}

void SaveToObj(ObjectDescriptor& obj) {

}

void ObjectExporter::EnqueueObject(ObjectDescriptor&& obj) {
	{
		std::lock_guard<std::mutex> lock(queueMutex);
		ObjectExporter::objectQueue.push(std::move(obj));
	}
	cv.notify_one();
}

void ObjectExporter::StartExportWorker() {
	if (initialized) return;

	running = true;
	workerThread = std::thread(ObjectExporter::ThreadMain);
	initialized = true;
}

void ObjectExporter::StopExportWorker() {
	if (!initialized) return;

	running = false;
	cv.notify_all();

	if (workerThread.joinable()) workerThread.join();
	initialized = false;
}

void ObjectExporter::ThreadMain() {
	while (running || !objectQueue.empty()) {
		std::unique_lock<std::mutex> lock(queueMutex);
		cv.wait(lock, [] { return !running || !objectQueue.empty(); });

		while (!objectQueue.empty()) {
			ObjectDescriptor obj = std::move(objectQueue.front());
			objectQueue.pop();
			lock.unlock();

			size_t estimatedSize = obj.primitiveCount * 3 * obj.stride;

			if (estimatedSize < Filter) {
				lock.lock();
				continue;
			}

			obj.hash = ComputeHash(obj);

			if (!seenVertexHashes.contains(obj.hash)) {
				seenVertexHashes.insert(obj.hash);
				// Save obj...

				Logger::LogInfo()  << "======== Object #" << logId++ << " ========";
				Logger::LogInfo() << "Hash: " << obj.hash;
				Logger::LogInfo() << "VertexData Size: " << obj.vertexData.size() << " bytes";
				Logger::LogInfo() << "IndexData Size: " << obj.indexData.size() << " bytes";
				Logger::LogInfo() << "Stride: " << obj.stride;
				Logger::LogInfo() << "BaseVertexIndex: " << obj.baseVertexIndex;
				Logger::LogInfo() << "StartIndex: " << obj.startIndex;
				Logger::LogInfo() << "PrimitiveCount: " << obj.primitiveCount;
				Logger::LogInfo() << "Index Format: " << (obj.index32bit ? "32-bit" : "16-bit");

				const char* primType = "UNKNOWN";
				switch (obj.primitiveType) {
				case D3DPT_POINTLIST:     primType = "POINTLIST"; break;
				case D3DPT_LINELIST:      primType = "LINELIST"; break;
				case D3DPT_LINESTRIP:     primType = "LINESTRIP"; break;
				case D3DPT_TRIANGLELIST:  primType = "TRIANGLELIST"; break;
				case D3DPT_TRIANGLESTRIP: primType = "TRIANGLESTRIP"; break;
				case D3DPT_TRIANGLEFAN:   primType = "TRIANGLEFAN"; break;
				}
				Logger::LogInfo() << "Primitive Type: " << primType << "\n";
			}

			lock.lock();
		}
	}
}