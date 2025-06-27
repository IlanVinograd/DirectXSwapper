#include "Logger.h"
#include <Windows.h>
#include <ctime>

std::ofstream Logger::file;
std::mutex Logger::queueMutex;
std::condition_variable Logger::cv;
std::queue<std::string> Logger::messageQueue;
std::thread Logger::workerThread;
std::atomic<bool> Logger::running = false;
bool Logger::initialized = false;

std::deque<std::string> Logger::inMemoryLog;
std::mutex Logger::memoryLogMutex;

void Logger::Init(const std::string& filename) {
    if (initialized) return;

    char path[MAX_PATH];
    HMODULE hm = NULL;
    GetModuleHandleExA(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCSTR>(&Logger::Init), &hm);
    GetModuleFileNameA(hm, path, MAX_PATH);

    strcpy_s(strrchr(path, '\\'), MAX_PATH - strlen(path), "\\d3d12.ini");
    int enableLogger = GetPrivateProfileIntA("UTILITIES", "EnableLogger", 0, path);
    if (!enableLogger) return;

    file.open("Log.txt", std::ios::out | std::ios::trunc);
    if (!file.is_open()) return;

    running = true;
    workerThread = std::thread(&Logger::ThreadMain);
    initialized = true;
}

void Logger::Shutdown() {
    if (!initialized) return;

    if (file.is_open()) {
        time_t now = time(nullptr);
        tm localTime;
        localtime_s(&localTime, &now);
        char timeBuf[32];
        strftime(timeBuf, sizeof(timeBuf), "[%H:%M:%S]", &localTime);
        file << timeBuf << " [INFO] DLL_PROCESS_DETACH" << std::endl;
        file.flush();
    }

    running = false;
    cv.notify_all();

    if (workerThread.joinable()) workerThread.join();
    if (file.is_open()) file.close();
    initialized = false;
}

Logger::LogStream::LogStream(Level lvl) : level(lvl) {}

Logger::LogStream::~LogStream() {
    Logger::Write(level, stream.str());
}

void Logger::Write(Level lvl, const std::string& msg) {
    if (!initialized) return;

    const char* levelStr = "";
    switch (lvl) {
    case Level::Debug:   levelStr = "[DEBUG]"; break;
    case Level::Info:    levelStr = "[INFO]"; break;
    case Level::Warning: levelStr = "[WARN]"; break;
    case Level::Error:   levelStr = "[ERROR]"; break;
    }

    time_t now = time(nullptr);
    tm localTime;
    localtime_s(&localTime, &now);
    char timeBuf[32];
    strftime(timeBuf, sizeof(timeBuf), "[%H:%M:%S]", &localTime);

    std::ostringstream fullMessage;
    fullMessage << timeBuf << " " << levelStr << " " << msg;
    std::string finalMsg = fullMessage.str();

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        messageQueue.push(finalMsg);
    }
    cv.notify_one();

    {
        std::lock_guard<std::mutex> lock(memoryLogMutex);
        inMemoryLog.push_back(finalMsg);
        if (inMemoryLog.size() > 1000)
            inMemoryLog.pop_front();
    }
}

void Logger::ThreadMain() {
    while (running || !messageQueue.empty()) {
        std::unique_lock<std::mutex> lock(queueMutex);
        cv.wait(lock, [] { return !running || !messageQueue.empty(); });

        while (!messageQueue.empty()) {
            file << messageQueue.front() << std::endl;
            messageQueue.pop();
        }
        file.flush();
    }
}

Logger::LogStream Logger::LogDebug() { return LogStream(Level::Debug); }
Logger::LogStream Logger::LogInfo() { return LogStream(Level::Info); }
Logger::LogStream Logger::LogWarning() { return LogStream(Level::Warning); }
Logger::LogStream Logger::LogError() { return LogStream(Level::Error); }

const std::deque<std::string>& Logger::GetLogBuffer() {
    return inMemoryLog;
}

std::vector<std::string> Logger::CopyLogBuffer() {
    std::lock_guard<std::mutex> lock(memoryLogMutex);
    return std::vector<std::string>(inMemoryLog.begin(), inMemoryLog.end());
}

void Logger::ClearLogBuffer() {
    std::lock_guard<std::mutex> lock(memoryLogMutex);
    inMemoryLog.clear();
}