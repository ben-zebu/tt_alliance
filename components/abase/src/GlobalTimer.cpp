#include "GlobalTimer.h"
#include <stdexcept>
#include <iostream>

using namespace abase;

namespace abase {
    GlobalTimer globalTimer;
}

void GlobalTimer::start(const std::string& name) {
    auto& timer = timers[name];
    if (timer.running) {
        throw std::runtime_error("Timer " + name + " already running");
    }
    timer.wallStart = WallClock::now();
    timer.cpuStart = std::chrono::steady_clock::now();
    timer.running = true;
}

void GlobalTimer::stop(const std::string& name) {
    auto it = timers.find(name);
    if (it == timers.end() || !it->second.running) {
        throw std::runtime_error("Timer " + name + " not running");
    }
    
    auto& timer = it->second;
    timer.wallTotal += WallClock::now() - timer.wallStart;
    timer.cpuTotal += std::chrono::steady_clock::now() - timer.cpuStart;
    timer.running = false;
}

void GlobalTimer::stop_all() {
    for (auto& timer : timers) {
        if (timer.second.running) stop(timer.first);
    }
}

double GlobalTimer::getWallTime(const std::string& name) const {
    auto it = timers.find(name);
    if (it == timers.end()) {
        return 0.0;
    }
    return std::chrono::duration_cast<Duration>(it->second.wallTotal).count();
}

double GlobalTimer::getCPUTime(const std::string& name) const {
    auto it = timers.find(name);
    if (it == timers.end()) {
        return 0.0;
    }
    return std::chrono::duration_cast<std::chrono::duration<double>>(it->second.cpuTotal).count();
}

void GlobalTimer::print(const std::string& name) {
    auto it = timers.find(name);
    if (it == timers.end()) {
        return;
    }

    std::string cpu_message = "cpu time(" + name + ") = " + std::to_string(getCPUTime(name)) + " s" ;
    std::string elapsed_message = "elasped time(" + name + ") = " + std::to_string(getWallTime(name)) + " s" ;
    
    std::cout << cpu_message << std::endl;
    std::cout << elapsed_message << std::endl;
}

void GlobalTimer::reset(const std::string& name) {
    auto it = timers.find(name);
    if (it != timers.end()) {
        it->second = TimerData{};
    }
}

void GlobalTimer::resetAll() {
    timers.clear();
}

std::pair<std::string, std::string> GlobalTimer::get_timer(const std::string& name) {
    std::string cpu_time = str::to_string(getCPUTime(name), abase::TIME_PRECISION);
    std::string wall_time = str::to_string(getWallTime(name), abase::TIME_PRECISION);
    return std::make_pair(cpu_time, wall_time);
}

std::unordered_map<std::string, std::pair<std::string, std::string>> GlobalTimer::get_all_timers() {
    std::unordered_map<std::string, std::pair<std::string, std::string>> result;
    for (const auto& timer : timers) {
        result[timer.first] = get_timer(timer.first);
    }
    return result;
}