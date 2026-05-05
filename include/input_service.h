#pragma once
#include <windows.h>
#include <unordered_map>
#include <string>

namespace recoil {

class InputService {
public:
    static InputService& Instance();
    
    bool Initialize();
    void Shutdown();
    
    void MouseMove(int dx, int dy);
    void MouseClick(int button);  // 0=left,1=right,2=middle
    bool IsKeyPressed(int vk_code) const;
    int GetVirtualKeyCode(const std::string& key_name) const;
    
private:
    InputService() = default;
    void BuildKeyMap();
    void SendRawInput(const INPUT& input) const;
    
    std::unordered_map<std::string, int> key_map_;
    bool initialized_ = false;
};

} // namespace recoil
