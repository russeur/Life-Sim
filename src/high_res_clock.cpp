#include "high_res_clock.hpp"




time_point now(){
    return std::chrono::high_resolution_clock::now();
}

long long get_delta(time_point start, time_point end){
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
}



