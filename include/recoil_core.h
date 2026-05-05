#pragma once
#include "types.h"
#include <functional>

namespace recoil {

// Колбэки
using WeaponChangeCallback = std::function<void(const std::string& weapon_name)>;
using PositionUpdateCallback = std::function<void(double x, double y)>;
using StatusChangeCallback = std::function<void(bool active)>;
using GSICallback = std::function<void(const GSIPlayerState& state)>;

class RecoilCore {
public:
    static RecoilCore& Instance();
    
    // Инициализация/завершение
    bool Initialize(const GlobalConfig& config);
    void Shutdown();
    
    // Колбэки
    void SetWeaponChangeCallback(WeaponChangeCallback cb);
    void SetPositionUpdateCallback(PositionUpdateCallback cb);
    void SetStatusChangeCallback(StatusChangeCallback cb);
    void SetGSICallback(GSICallback cb);
    
    // Управление оружием
    bool LoadWeapon(const WeaponProfile& weapon);
    bool UnloadWeapon(const std::string& name);
    void SetActiveWeapon(const std::string& name);
    std::string GetActiveWeapon() const;
    const WeaponProfile* GetWeaponProfile(const std::string& name) const;
    
    // RCS управление
    bool StartCompensation();
    bool StopCompensation();
    bool IsActive() const;
    void ToggleCompensation();  // для хоткея
    
    // Авто-детект через GSI
    void EnableAutoDetection();
    void DisableAutoDetection();
    bool IsAutoDetectionEnabled() const;
    
    // GSI (вызывается из GSI сервера)
    void ProcessGSIData(const GSIPlayerState& state);
    
    // Обновление конфига на лету
    void UpdateGlobalConfig(const GlobalConfig& config);
    void UpdateWeaponConfig(const WeaponProfile& weapon);
    
private:
    RecoilCore() = default;
    ~RecoilCore() = default;
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace recoil
