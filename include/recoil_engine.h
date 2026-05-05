#pragma once
#include "types.h"
#include <atomic>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <random>

namespace recoil {

class RecoilEngine {
public:
    RecoilEngine();
    ~RecoilEngine();
    
    bool Initialize();
    void Shutdown();
    
    void LoadWeapon(const WeaponProfile& profile);
    void UnloadWeapon(const std::string& name);
    void SetActiveWeapon(const std::string& name);
    std::string GetActiveWeapon() const;
    const WeaponProfile* GetWeapon(const std::string& name) const;
    
    void Start();
    void Stop();
    bool IsActive() const;
    void Toggle();
    
    void UpdatePatternPosition(int bullet_index, double dx, double dy);
    
private:
    void WorkerLoop();
    void PrecomputePattern(WeaponProfile& profile);
    void ExecuteSpray(const WeaponProfile& weapon);
    void ApplyJitter(double& dx, double& dy, double& delay, double movement_jitter_percent, double timing_jitter_ms);
    
    struct LoadedWeapon {
        WeaponProfile profile;
        std::vector<RecoilPoint> computed_pattern;
    };
    
    std::unordered_map<std::string, LoadedWeapon> weapons_;
    std::string active_weapon_;
    mutable std::mutex mutex_;
    
    std::atomic<bool> active_{false};
    std::atomic<bool> stop_requested_{false};
    std::thread worker_thread_;
    
    // аккумуляторы для дробной части (sub-pixel movement)
    double accum_x_ = 0.0;
    double accum_y_ = 0.0;
    double raw_x_ = 0.0;  // для оверлея
    double raw_y_ = 0.0;
    
    // рандом для джиттера
    std::mt19937 rng_{std::random_device{}()};
    
    // внешние сервисы
    class InputService* input_ = nullptr;
    class TimingService* timing_ = nullptr;
};

} // namespace recoil
