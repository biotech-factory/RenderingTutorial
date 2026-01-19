#pragma once
#include <cstdint>

struct SDL_Window;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;

class RendererD3D11 {
public:
    RendererD3D11() = default;
    ~RendererD3D11();

    RendererD3D11(const RendererD3D11&) = delete;
    RendererD3D11& operator=(const RendererD3D11&) = delete;

    bool Initialize(SDL_Window* sdlWindow, int width, int height);
    void Shutdown();

    void Resize(int width, int height);

    void BeginFrame(float r, float g, float b, float a);
    void EndFrame();

private:
    bool CreateRenderTarget();
    void ReleaseRenderTarget();

    // 내부 포인터
    ID3D11Device* m_device = nullptr;
    ID3D11DeviceContext* m_context = nullptr;
    IDXGISwapChain* m_swapChain = nullptr;
    ID3D11RenderTargetView* m_rtv = nullptr;

    int m_width = 0;
    int m_height = 0;
};
