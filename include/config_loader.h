#pragma once
#include "types.h"
#include <string>

namespace recoil {

class ConfigLoader {
public:
    static ConfigLoader& Instance();
    
    bool LoadConfig(const std::string& config_path = "config.json");
    bool SaveConfig(const std::string& config_path = "config.json");
    
    GlobalConfig GetGlobalConfig() const;
    std::vector<WeaponProfile> GetAllWeapons() const;
    WeaponProfile LoadWeaponPattern(const std::string& weapon_name, const std::string& patterns_dir = "patterns/");
    
    void SetGlobalConfig(const GlobalConfig& config);
    void SetWeaponProfile(const WeaponProfile& profile);
    
private:
    ConfigLoader() = default;
    void ParseWeaponCSV(const std::string& csv_path, WeaponProfile& profile, double sensitivity);
    
    GlobalConfig config_;
    std::unordered_map<std::string, WeaponProfile> weapons_;
    mutable std::mutex mutex_;
};

} // namespace recoil
