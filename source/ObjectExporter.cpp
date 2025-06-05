#include "ObjectExporter.h"

int logId = 0;
std::queue<ObjectDescriptor> ObjectExporter::objectQueue;
std::mutex ObjectExporter::queueMutex;
std::condition_variable ObjectExporter::cv;
bool ObjectExporter::initialized = false;
std::atomic<bool> ObjectExporter::running = false;
std::thread ObjectExporter::workerThread;
std::unordered_set<size_t> ObjectExporter::seenVertexHashes;

thread_local std::vector<uint8_t> tempVB;

void ObjectExporter::SaveToObj(ObjectDescriptor& obj, const std::string& path) {
	if (obj.vertexData.empty() || obj.indexData.empty() || obj.stride < 12) {
		Logger::LogInfo() << "[SaveToObj] Invalid or empty data.";
		return;
	}

	std::ofstream file(path);
	if (!file.is_open()) {
		Logger::LogInfo() << "[SaveToObj] Failed to open file: " << path;
		return;
	}

	const size_t vertexCount = obj.vertexData.size() / obj.stride;
	const size_t indexCount = obj.primitiveCount * 3;

	bool hasNormals = obj.stride >= 24;
	bool hasUVs = obj.stride >= 32;

	for (size_t i = 0; i < vertexCount; ++i) {
		const uint8_t* base = &obj.vertexData[i * obj.stride];
		const float* pos = reinterpret_cast<const float*>(base + 0);
		file << std::fixed << std::setprecision(6);
		file << "v " << pos[0] << " " << pos[1] << " " << pos[2] << "\n";

		if (hasNormals) {
			const float* norm = reinterpret_cast<const float*>(base + 12);
			file << "vn " << norm[0] << " " << norm[1] << " " << norm[2] << "\n";
		}
		if (hasUVs) {
			const float* uv = reinterpret_cast<const float*>(base + 24);
			file << "vt " << uv[0] << " " << uv[1] << "\n";
		}
	}

	auto writeFace = [&](uint32_t i0, uint32_t i1, uint32_t i2) {
		auto formatIndex = [&](uint32_t idx) -> std::string {
			idx += 1; // OBJ is 1-based
			if (hasNormals && hasUVs)    return std::to_string(idx) + "/" + std::to_string(idx) + "/" + std::to_string(idx);
			if (hasUVs)                  return std::to_string(idx) + "/" + std::to_string(idx);
			if (hasNormals)              return std::to_string(idx) + "//" + std::to_string(idx);
			return std::to_string(idx);
			};
		file << "f " << formatIndex(i0) << " " << formatIndex(i1) << " " << formatIndex(i2) << "\n";
		};

	if (obj.index32bit) {
		const uint32_t* indices = reinterpret_cast<const uint32_t*>(obj.indexData.data());
		for (size_t i = 0; i < indexCount; i += 3) {
			uint32_t i0 = indices[obj.startIndex + i + 0];
			uint32_t i1 = indices[obj.startIndex + i + 1];
			uint32_t i2 = indices[obj.startIndex + i + 2];
			writeFace(i0, i1, i2);
		}
	}
	else {
		const uint16_t* indices = reinterpret_cast<const uint16_t*>(obj.indexData.data());
		for (size_t i = 0; i < indexCount; i += 3) {
			uint32_t i0 = static_cast<uint32_t>(indices[obj.startIndex + i + 0]);
			uint32_t i1 = static_cast<uint32_t>(indices[obj.startIndex + i + 1]);
			uint32_t i2 = static_cast<uint32_t>(indices[obj.startIndex + i + 2]);
			writeFace(i0, i1, i2);
		}
	}
}

void ObjectExporter::LockAndFillVertexBuffer(ObjectDescriptor& obj, LPDIRECT3DVERTEXBUFFER9 vb, UINT stride) {
	if (!vb || stride == 0)
		return;

	D3DVERTEXBUFFER_DESC vbDesc = {};
	if (FAILED(vb->GetDesc(&vbDesc)) || vbDesc.Size == 0)
		return;

	void* vertexData = nullptr;
	DWORD vbLockFlags = (vbDesc.Pool == D3DPOOL_DEFAULT)
		? D3DLOCK_READONLY | D3DLOCK_NOOVERWRITE
		: 0;

	HRESULT hr = vb->Lock(0, 0, &vertexData, vbLockFlags);
	if (FAILED(hr) || !vertexData) {
		Logger::LogInfo() << "[Skip] vb->Lock failed. Pool = " << vbDesc.Pool << ", hr = 0x" << std::hex << hr << std::endl;
		return;
	}

	if (tempVB.size() < vbDesc.Size)
		tempVB.resize(vbDesc.Size);

	memcpy(tempVB.data(), vertexData, vbDesc.Size);
	vb->Unlock();

	obj.vertexData.assign(tempVB.begin(), tempVB.begin() + vbDesc.Size);
}

size_t ObjectExporter::ComputeHash(const ObjectDescriptor& obj) {
	size_t hash = 0;

	auto combine = [&](size_t value) {
		hash ^= std::hash<size_t>{}(value)+0x9e3779b9 + (hash << 6) + (hash >> 2);
		};

	std::unordered_set<uint32_t> uniqueIndices;
	const size_t indexCount = obj.primitiveCount * 3;

	if (obj.index32bit) {
		const uint32_t* indices = reinterpret_cast<const uint32_t*>(obj.indexData.data());
		for (size_t i = 0; i < indexCount; ++i) {
			size_t idx = obj.startIndex + i;
			if (idx < obj.indexData.size() / sizeof(uint32_t)) {
				uniqueIndices.insert(indices[idx]);
			}
		}
	}
	else {
		const uint16_t* indices = reinterpret_cast<const uint16_t*>(obj.indexData.data());
		for (size_t i = 0; i < indexCount; ++i) {
			size_t idx = obj.startIndex + i;
			if (idx < obj.indexData.size() / sizeof(uint16_t)) {
				uniqueIndices.insert(static_cast<uint32_t>(indices[idx]));
			}
		}
	}

	std::vector<uint32_t> sortedIndices(uniqueIndices.begin(), uniqueIndices.end());
	std::sort(sortedIndices.begin(), sortedIndices.end());

	combine(std::hash<std::string_view>{}(
		std::string_view(reinterpret_cast<const char*>(sortedIndices.data()), sortedIndices.size() * sizeof(uint32_t))));

	combine(obj.stride);
	combine(obj.baseVertexIndex);
	combine(obj.startIndex);
	combine(obj.primitiveCount);
	combine(obj.primitiveType);
	combine(obj.index32bit ? 1 : 0);

	return hash;
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

				if (Button_2) {
					Logger::LogInfo() << "OBJ EXPORT";
					std::string folder = "exported/";
					std::filesystem::create_directories(folder);
					SaveToObj(obj, folder + std::to_string(obj.hash) + "_model.obj");
				}

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

void ObjectExporter::resetHashes() {
	seenVertexHashes.clear();
}