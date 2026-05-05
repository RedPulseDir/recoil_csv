#pragma once
#include <windows.h>
#include <functional>
#include <unordered_map>
#include <thread>
#include <atomic>

namespace recoil {

class HotkeyService {
public:
    using HotkeyCallback = std::function<void()>;
    
    static HotkeyService& Instance();
    
    bool Initialize();
    void Shutdown();
    
    void RegisterHotkey(const std::string& name, int vk_code, HotkeyCallback cb);
    void UnregisterHotkey(const std::string& name);
    void SetWeaponHotkey(const std::string& weapon_name, int vk_code, HotkeyCallback cb);
    
private:
    void PollingLoop();
    bool IsKeyPressed(int vk_code) const;
    
    struct HotkeyInfo {
        int vk_code;
        HotkeyCallback callback;
    };
    
    std::unordered_map<std::string, HotkeyInfo> hotkeys_;
    std::atomic<bool> running_{false};
    std::thread polling_thread_;
};

} // namespace recoil
