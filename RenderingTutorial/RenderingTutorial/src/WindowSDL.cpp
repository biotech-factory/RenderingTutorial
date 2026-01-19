#include "WindowSDL.h"
#include <cstdio>


WindowSDL::~WindowSDL() {
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    SDL_Quit();
}

bool WindowSDL::Create(const std::string& title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::printf("SDL_Init failed: %s\n", SDL_GetError());
        return false;
    }

    // 리사이즈까지 대비해서 SDL_WINDOW_RESIZABLE 포함(원치 않으면 빼도 됨)
    m_window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!m_window) {
        std::printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        return false;
    }

    m_width = width;
    m_height = height;
    return true;
}

void WindowSDL::PollEvents(bool& running) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT) {
            running = false;
        }
        if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE) {
            running = false;
        }

        if (ev.type == SDL_WINDOWEVENT) {
            if (ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                m_resized = true;
                m_pendingW = ev.window.data1;
                m_pendingH = ev.window.data2;
            }
        }
    }
}

bool WindowSDL::ConsumeResize(int& outW, int& outH) {
    if (!m_resized) return false;
    m_resized = false;

    m_width = m_pendingW;
    m_height = m_pendingH;
    outW = m_width;
    outH = m_height;
    return true;
}
