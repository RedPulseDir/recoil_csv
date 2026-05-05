#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace recoil {

#pragma pack(push, 1)
struct RecoilPoint {
    double dx;
    double dy;
    double delay_ms;
};

struct WeaponProfile {
    std::string name;
    std::string display_name;
    std::vector<RecoilPoint> raw_pattern;
    int length = 30;
    int multiple = 6;
    double sleep_divider = 6.0;
    double sleep_suber = 0.0;
    double jitter_timing_ms = 0.0;
    double jitter_movement_percent = 0.0;
    int max_bullets = 0;  // 0 = весь паттерн
};

struct GlobalConfig {
    double game_sensitivity = 1.0;
    int key_trigger = 0x01;  // VK_LBUTTON
    bool follow_rcs_enabled = false;
    struct {
        int dot_size = 3;
        uint8_t color[4] = {0, 0, 255, 255};
    } follow_rcs;
    struct {
        bool enabled = true;
        int port = 59873;
        std::string host = "127.0.0.1";
        std::string auth_token;
    } gsi;
    struct {
        std::string exit = "END";
        std::string toggle_recoil = "INSERT";
        std::string toggle_weapon_detection = "HOME";
        std::unordered_map<std::string, std::string> weapons;
    } hotkeys;
};

struct GSIPlayerState {
    int health = 0;
    int armor = 0;
    bool alive = false;
    std::string active_weapon;
    int ammo_clip = 0;
    bool bomb_planted = false;
    bool has_defuse_kit = false;
    double timestamp = 0.0;
};
#pragma pack(pop)

} // namespace recoil
