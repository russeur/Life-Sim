#include <chrono>

typedef std::chrono::time_point<std::chrono::high_resolution_clock> time_point;


time_point now();

long long get_delta(time_point start, time_point end);

constexpr int64_t  ticks_per_sec = std::chrono::high_resolution_clock::period::den;;


