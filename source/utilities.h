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