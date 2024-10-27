#pragma once
#include <string>
#include <chrono>
#include <unordered_map>
#include <limits>

class GlobalTimer {
private:
    using WallClock = std::chrono::high_resolution_clock;
    using TimePoint = WallClock::time_point;
    using Duration = std::chrono::duration<double>;

    struct TimerData {
        TimePoint wallStart;
        std::chrono::time_point<std::chrono::steady_clock> cpuStart;
        Duration wallTotal{0};
        Duration cpuTotal{0};
        bool running{false};
    };

    std::unordered_map<std::string, TimerData> timers;

public:
    void start(const std::string& name);
    void stop(const std::string& name);
    double getWallTime(const std::string& name) const;
    double getCPUTime(const std::string& name) const;
    void reset(const std::string& name);
    void resetAll();
    void print(const std::string& name);
};

// Global instance of GlobalTimer
extern GlobalTimer globalTimer;

void start_timer(const std::string& name) {
    globalTimer.start(name);
}

void stop_timer(const std::string& name) {
    globalTimer.stop(name);
}

void print_timer(const std::string& name) {
    globalTimer.print(name);
}