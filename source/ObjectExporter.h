#pragma once
#include <unordered_set>

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
	static size_t ComputeHash(const uint8_t* data, size_t size) {
		size_t h = 0;
		for (size_t i = 0; i < size; ++i) {
			h ^= std::hash<uint8_t>()(data[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
		}
		return h;
	}

	void EnqueueObject(ObjectDescriptor&& obj);
	void StartExportWorker();
	void StopExportWorker();

private:
	static std::unordered_set<size_t> seenVertexHashes;
	size_t dataHash = 0;
};