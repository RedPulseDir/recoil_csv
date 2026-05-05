#pragma once
#include <windows.h>

namespace recoil {

class TimingService {
public:
    static TimingService& Instance();
    
    void Initialize();
    void Shutdown();
    
    double GetTimeMs() const;           // через QPC
    void SleepMs(double ms);            // адаптивный сон
    void SleepUntil(double target_ms, double start_ms); // абсолютный
    
private:
    TimingService() = default;
    void BusyWait(double ms) const;
    
    LARGE_INTEGER frequency_;
    double freq_inv_ms_;
    bool high_res_available_ = false;
    UINT original_timer_res_ = 0;
};

} // namespace recoil
