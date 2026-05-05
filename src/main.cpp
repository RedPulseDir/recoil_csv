#include "recoil_core.h"
#include "logger.h"
#include "config_loader.h"
#include <windows.h>
#include <chrono>
#include <thread>

using namespace recoil;

// Глобальный инстанс
static RecoilCore& g_core = RecoilCore::Instance();

// Функции обратного вызова для GUI (здесь пока заглушки, потом заменим на реальный GUI)
void OnWeaponChanged(const std::string& weapon_name) {
    LOG_INFO("Weapon changed to: {}", weapon_name);
}

void OnPositionUpdate(double x, double y) {
    // Для оверлея Follow RCS
    static double last_x = 0, last_y = 0;
    if (std::abs(x - last_x) > 0.1 || std::abs(y - last_y) > 0.1) {
        last_x = x;
        last_y = y;
        LOG_DEBUG("Recoil position: ({:.2f}, {:.2f})", x, y);
    }
}

void OnStatusChange(bool active) {
    LOG_INFO("RCS status: {}", active ? "ACTIVE" : "INACTIVE");
}

void OnGSIUpdate(const GSIPlayerState& state) {
    LOG_DEBUG("GSI: weapon={}, ammo={}, alive={}", 
              state.active_weapon, state.ammo_clip, state.alive);
    
    if (state.alive && !state.active_weapon.empty()) {
        // Автоматическое переключение оружия если включен авто-детект
        if (g_core.IsAutoDetectionEnabled()) {
            g_core.SetActiveWeapon(state.active_weapon);
        }
    }
    
    // Авто-стоп если умер или нет патронов
    if (!state.alive || state.ammo_clip == 0) {
        if (g_core.IsActive()) {
            g_core.StopCompensation();
        }
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    
    // Инициализация логгера
    Logger::Initialize("recoil_rcs.log");
    LOG_INFO("=== Recoil RCS Starting ===");
    
    // Загрузка конфига
    auto& configLoader = ConfigLoader::Instance();
    if (!configLoader.LoadConfig()) {
        LOG_ERROR("Failed to load config.json, using defaults");
    }
    
    GlobalConfig config = configLoader.GetGlobalConfig();
    auto weapons = configLoader.GetAllWeapons();
    
    // Инициализация ядра
    if (!g_core.Initialize(config)) {
        LOG_CRITICAL("Failed to initialize RecoilCore");
        MessageBoxA(nullptr, "Failed to initialize Recoil RCS", "Error", MB_ICONERROR);
        return 1;
    }
    
    // Регистрация колбэков
    g_core.SetWeaponChangeCallback(OnWeaponChanged);
    g_core.SetPositionUpdateCallback(OnPositionUpdate);
    g_core.SetStatusChangeCallback(OnStatusChange);
    g_core.SetGSICallback(OnGSIUpdate);
    
    // Загрузка всех оружий
    for (const auto& weapon : weapons) {
        if (g_core.LoadWeapon(weapon)) {
            LOG_INFO("Loaded weapon: {}", weapon.display_name);
        } else {
            LOG_WARN("Failed to load weapon: {}", weapon.name);
        }
    }
    
    // Установка активного оружия (по умолчанию AK-47)
    g_core.SetActiveWeapon("ak47");
    
    // Включение GSI сервера
    if (config.gsi.enabled) {
        // GSI сервер запускается внутри RecoilCore::Initialize
        LOG_INFO("GSI server started on {}:{}", config.gsi.host, config.gsi.port);
    }
    
    LOG_INFO("System ready. Press INSERT to toggle RCS, END to exit.");
    
    // Главный цикл (ждем сигнала выхода)
    // TODO: В реальном приложении тут должен быть GUI или системный трей
    // Сейчас просто показываем консольное окно для отладки
    
    // Создаем консоль для отладки (можно убрать в релизе)
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);
    
    std::cout << "Recoil RCS Running. Commands:\n";
    std::cout << "  q - Quit\n";
    std::cout << "  t - Toggle RCS\n";
    std::cout << "  w - Show current weapon\n";
    std::cout << "  l - List loaded weapons\n";
    
    bool running = true;
    while (running) {
        if (GetAsyncKeyState('Q') & 1) {
            running = false;
            break;
        }
        if (GetAsyncKeyState('T') & 1) {
            g_core.ToggleCompensation();
        }
        if (GetAsyncKeyState('W') & 1) {
            std::cout << "Current weapon: " << g_core.GetActiveWeapon() << "\n";
        }
        if (GetAsyncKeyState('L') & 1) {
            std::cout << "Loaded weapons:\n";
            // TODO: вывести список
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    // Очистка
    g_core.Shutdown();
    Logger::Shutdown();
    
    if (f) fclose(f);
    FreeConsole();
    
    LOG_INFO("=== Recoil RCS Stopped ===");
    return 0;
}
