#pragma once
#include <string>
#include <chrono>
#include <unordered_map>
#include <limits>

#include "String.h"

namespace abase {

    /// @brief The precision of the timer output (string representation).
    const std::size_t TIME_PRECISION = 3;

    /// @class GlobalTimer
    /// @brief A class to manage and measure wall-clock and CPU time for named timers.
    class GlobalTimer {
    private:
        using WallClock = std::chrono::high_resolution_clock;
        using TimePoint = WallClock::time_point;
        using Duration = std::chrono::duration<double>;

        struct TimerData {
            TimePoint wallStart; ///< The start time of the wall-clock timer.
            std::chrono::time_point<std::chrono::steady_clock> cpuStart; ///< The start time of the CPU timer.
            Duration wallTotal{0}; ///< The total wall-clock time accumulated.
            Duration cpuTotal{0}; ///< The total CPU time accumulated.
            bool running{false}; ///< Indicates whether the timer is currently running.
        };

        std::unordered_map<std::string, TimerData> timers; ///< A map of timer names to their data.

    public:
        /// @brief Starts the timer with the given name.
        /// @param name The name of the timer to start.
        void start(const std::string& name);
        /// @brief Stops the timer with the given name.
        /// @param name The name of the timer to stop.
        void stop(const std::string& name);
        /// @brief Stops all timers.
        void stop_all();

        /// @brief Gets the total wall-clock time for the timer with the given name.
        /// @param name The name of the timer.
        /// @return The total wall-clock time in seconds.
        double getWallTime(const std::string& name) const;
        /// @brief Gets the total CPU time for the timer with the given name.
        /// @param name The name of the timer.
        /// @return The total CPU time in seconds.
        double getCPUTime(const std::string& name) const;

        /// @brief Resets the timer with the given name.
        /// @param name The name of the timer to reset.
        void reset(const std::string& name);
        /// @brief Resets all timers.
        void resetAll();

        /// @brief Prints the timing information for the timer with the given name.
        /// @param name The name of the timer to print.
        void print(const std::string& name);
        /// @brief Gets the timing information for the timer with the given name.
        /// @param name The name of the timer to get.
        /// @return A pair of strings representing the CPU and wall-clock times.
        std::pair<std::string, std::string> get_timer(const std::string& name);
        /// @brief Gets the timing information for all timers.
        /// @return A map of pairs of strings representing the CPU and wall-clock times.
        std::unordered_map<std::string, std::pair<std::string, std::string>> get_all_timers();

    };

    
    /// @brief Global instance of GlobalTimer.
    extern GlobalTimer globalTimer;

} // namespace abase


//
// Short aliases for timer functions
//

inline void start_timer(const std::string& name) {
    abase::globalTimer.start(name);
}

inline void stop_timer(const std::string& name) {
    abase::globalTimer.stop(name);
}

inline void stop_all_timers() {
    return abase::globalTimer.stop_all();
}

inline void print_timer(const std::string& name) {
    abase::globalTimer.print(name);
}

inline std::pair<std::string, std::string> get_timer(const std::string& name) {
    return abase::globalTimer.get_timer(name);
}

inline std::unordered_map<std::string, std::pair<std::string, std::string>> get_all_timers() {
    return abase::globalTimer.get_all_timers();
}
