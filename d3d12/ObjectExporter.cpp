#include "ObjectExporter.h"

void RunAsyncExport(void* vbData, UINT vbSize, UINT stride,
    void* ibData, UINT ibSize, DXGI_FORMAT format)
{
    std::vector<char> vbCopy((char*)vbData, (char*)vbData + vbSize);
    std::vector<char> ibCopy((char*)ibData, (char*)ibData + ibSize);

    std::thread([vbCopy = std::move(vbCopy), ibCopy = std::move(ibCopy), stride, format]() {
        ExportToObj(vbCopy.data(), (UINT)vbCopy.size(), stride, ibCopy.data(), (UINT)ibCopy.size(), format);
        }).detach();
}

void ExportToObj(const void* vbData, UINT vbSize, UINT stride,
    const void* ibData, UINT ibSize, DXGI_FORMAT format) {

    if (stride < sizeof(float) * 3) {
        Logger::LogError() << "[ExportToObj] Invalid stride: too small for position data.";
        return;
    }

    static int counter = 0;
    std::ostringstream nameStream;
    nameStream << "Exported/obj_" << std::setw(4) << std::setfill('0') << counter++ << ".obj";
    std::filesystem::create_directories("Exported");

    std::ofstream obj(nameStream.str());
    if (!obj) return;

    const char* vb = reinterpret_cast<const char*>(vbData);
    UINT vertexCount = vbSize / stride;

    for (UINT i = 0; i < vertexCount; ++i) {
        const float* pos = reinterpret_cast<const float*>(vb + i * stride);

        if (!std::isfinite(pos[0]) || !std::isfinite(pos[1]) || !std::isfinite(pos[2])) {
            Logger::LogWarning() << "[ExportToObj] Skipping vertex with invalid position at index " << i;
            obj << "v 0 0 0\n";
            continue;
        }

        float x = pos[0];
        float y = pos[1];
        float z = pos[2];

        obj << "v " << x << " " << z << " " << -y << "\n";
    }

    const char* ib = reinterpret_cast<const char*>(ibData);
    UINT indexCount = 0;

    if (format == DXGI_FORMAT_R16_UINT)
        indexCount = ibSize / sizeof(uint16_t);
    else if (format == DXGI_FORMAT_R32_UINT)
        indexCount = ibSize / sizeof(uint32_t);
    else {
        Logger::LogError() << "[ExportToObj] Unsupported index format: " << format;
        return;
    }

    for (UINT i = 0; i + 2 < indexCount; i += 3) {
        UINT i0, i1, i2;

        if (format == DXGI_FORMAT_R16_UINT) {
            const uint16_t* indices = reinterpret_cast<const uint16_t*>(ib);
            i0 = indices[i];
            i1 = indices[i + 1];
            i2 = indices[i + 2];
        }
        else {
            const uint32_t* indices = reinterpret_cast<const uint32_t*>(ib);
            i0 = indices[i];
            i1 = indices[i + 1];
            i2 = indices[i + 2];
        }

        if (i0 >= vertexCount || i1 >= vertexCount || i2 >= vertexCount)
        {
            Logger::LogWarning() << "[ExportToObj] Skipping face with invalid index: "
                << i0 << ", " << i1 << ", " << i2;
            continue;
        }

        obj << "f " << (i0 + 1) << " " << (i1 + 1) << " " << (i2 + 1) << "\n";
    }

    obj.close();
}

size_t HashBuffers(const void* vbData, size_t vbSize, const void* ibData, size_t ibSize)
{
    const uint8_t* vbBytes = reinterpret_cast<const uint8_t*>(vbData);
    const uint8_t* ibBytes = reinterpret_cast<const uint8_t*>(ibData);

    size_t hash = 14695981039346656037ull; // FNV-1a 64-bit offset basis
    for (size_t i = 0; i < vbSize; ++i)
        hash = (hash ^ vbBytes[i]) * 1099511628211ull;
    for (size_t i = 0; i < ibSize; ++i)
        hash = (hash ^ ibBytes[i]) * 1099511628211ull;
    return hash;
}