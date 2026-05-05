#pragma once
#include <windows.h>
#include <atomic>

namespace recoil {

class FollowRCSOverlay {
public:
    FollowRCSOverlay();
    ~FollowRCSOverlay();
    
    bool Create(HINSTANCE hInstance);
    void Destroy();
    void Show();
    void Hide();
    void UpdatePosition(double x, double y);
    void SetActive(bool active);
    void SetDotSize(int size);
    void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    
private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void Render();
    
    HWND hwnd_ = nullptr;
    HDC hdc_ = nullptr;
    HBITMAP hbm_ = nullptr;
    
    std::atomic<double> pos_x_{0.0};
    std::atomic<double> pos_y_{0.0};
    std::atomic<bool> active_{false};
    
    int dot_size_ = 3;
    COLORREF dot_color_ = RGB(0, 0, 255);
    int window_size_ = 400;
};

} // namespace recoil
