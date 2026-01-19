#pragma once
#include <SDL.h>
#include <string>

class WindowSDL {
public:
    WindowSDL() = default;
    ~WindowSDL();

    WindowSDL(const WindowSDL&) = delete;
    WindowSDL& operator=(const WindowSDL&) = delete;

    bool Create(const std::string& title, int width, int height);
    void PollEvents(bool& running);

    SDL_Window* GetSDLWindow() const { return m_window; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

    // 리사이즈 이벤트가 발생했는지 + 최신 크기
    bool ConsumeResize(int& outW, int& outH);

private:
    SDL_Window* m_window = nullptr;
    int m_width = 0;
    int m_height = 0;

    bool m_resized = false;
    int m_pendingW = 0;
    int m_pendingH = 0;
};
