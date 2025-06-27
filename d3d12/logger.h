#pragma once
#include <fstream>
#include <mutex>
#include <sstream>
#include <string>
#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>

class Logger {
public:
    enum class Level { Debug, Info, Warning, Error };

    class LogStream {
    public:
        explicit LogStream(Level lvl);
        ~LogStream();

        LogStream(const LogStream&) = delete;
        LogStream& operator=(const LogStream&) = delete;

        LogStream(LogStream&&) noexcept = default;
        LogStream& operator=(LogStream&&) noexcept = default;

        template <typename T>
        LogStream& operator<<(const T& val) {
            stream << val;
            return *this;
        }

        LogStream& operator<<(std::ostream& (*manip)(std::ostream&)) {
            stream << manip;
            return *this;
        }

    private:
        Level level;
        std::ostringstream stream;
    };

    static void Init(const std::string& filename = "log.txt");
    static void Shutdown();

    static LogStream LogDebug();
    static LogStream LogInfo();
    static LogStream LogWarning();
    static LogStream LogError();

    static const std::deque<std::string>& GetLogBuffer();
    static std::vector<std::string> CopyLogBuffer();
    static void ClearLogBuffer();
private:
    static void Write(Level lvl, const std::string& msg);
    static void ThreadMain();

    static std::ofstream file;
    static std::mutex queueMutex;
    static std::condition_variable cv;
    static std::queue<std::string> messageQueue;
    static std::thread workerThread;
    static std::atomic<bool> running;
    static bool initialized;

    static std::deque<std::string> inMemoryLog;
    static std::mutex memoryLogMutex;
};