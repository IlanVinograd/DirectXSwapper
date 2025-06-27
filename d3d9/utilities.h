#pragma once

#include <string>
#include <sstream>
#include <iomanip>

template<typename T>
std::string PointerToString(T* ptr) {
    std::ostringstream oss;
    oss << std::hex << std::uppercase << reinterpret_cast<uintptr_t>(ptr);
    return oss.str();
}

std::string FormatMatrixReadable(const D3DMATRIX& m) {
    std::ostringstream oss;
    oss << "[ " << m._11 << " " << m._12 << " " << m._13 << " " << m._14 << " | "
        << m._21 << " " << m._22 << " " << m._23 << " " << m._24 << " | "
        << m._31 << " " << m._32 << " " << m._33 << " " << m._34 << " | "
        << m._41 << " " << m._42 << " " << m._43 << " " << m._44 << " ]";
    return oss.str();
}

D3DMATRIX IdentityMatrix() {
    D3DMATRIX m = {};
    m._11 = m._22 = m._33 = m._44 = 1.0f;
    return m;
}

std::string MatrixToString(const D3DMATRIX& m) {
    std::ostringstream oss;
    oss << "[ "
        << m._11 << " " << m._12 << " " << m._13 << " " << m._14 << " | "
        << m._21 << " " << m._22 << " " << m._23 << " " << m._24 << " | "
        << m._31 << " " << m._32 << " " << m._33 << " " << m._34 << " | "
        << m._41 << " " << m._42 << " " << m._43 << " " << m._44 << " ]";
    return oss.str();
}