#include "RendererD3D11.h"

#include <SDL.h>
#include <SDL_syswm.h>

#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>

#include <cstdio>

template<typename T>
static void SafeRelease(T*& p) {
    if (p) { p->Release(); p = nullptr; }
}

static HWND GetHWNDFromSDL(SDL_Window* window) {
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    if (!SDL_GetWindowWMInfo(window, &wmInfo)) {
        return nullptr;
    }
    return wmInfo.info.win.window;
}

RendererD3D11::~RendererD3D11() {
    Shutdown();
}

bool RendererD3D11::Initialize(SDL_Window* sdlWindow, int width, int height) {
    m_width = width;
    m_height = height;

    HWND hwnd = GetHWNDFromSDL(sdlWindow);
    if (!hwnd) {
        std::printf("GetHWNDFromSDL failed\n");
        return false;
    }

    DXGI_SWAP_CHAIN_DESC scDesc = {};
    scDesc.BufferCount = 2;
    scDesc.BufferDesc.Width = (UINT)width;
    scDesc.BufferDesc.Height = (UINT)height;
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.OutputWindow = hwnd;
    scDesc.SampleDesc.Count = 1;
    scDesc.Windowed = TRUE;

    // DXGI_SWAP_EFFECT_DISCARD는 가장 단순한 경로(테스트 목적)
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createFlags = 0;
#if defined(_DEBUG)
    createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    const D3D_FEATURE_LEVEL fls[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0
    };
    D3D_FEATURE_LEVEL flOut = D3D_FEATURE_LEVEL_11_0;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createFlags,
        fls,
        (UINT)(sizeof(fls) / sizeof(fls[0])),
        D3D11_SDK_VERSION,
        &scDesc,
        &m_swapChain,
        &m_device,
        &flOut,
        &m_context
    );

    if (FAILED(hr)) {
        std::printf("D3D11CreateDeviceAndSwapChain failed (hr=0x%08X)\n", (unsigned)hr);
        Shutdown();
        return false;
    }

    if (!CreateRenderTarget()) {
        Shutdown();
        return false;
    }

    return true;
}

void RendererD3D11::Shutdown() {
    ReleaseRenderTarget();
    SafeRelease(m_swapChain);
    SafeRelease(m_context);
    SafeRelease(m_device);
}

bool RendererD3D11::CreateRenderTarget() {
    if (!m_swapChain || !m_device) return false;

    ID3D11Texture2D* backBuffer = nullptr;
    HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    if (FAILED(hr)) {
        std::printf("GetBuffer failed (hr=0x%08X)\n", (unsigned)hr);
        SafeRelease(backBuffer);
        return false;
    }

    hr = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_rtv);
    SafeRelease(backBuffer);

    if (FAILED(hr)) {
        std::printf("CreateRenderTargetView failed (hr=0x%08X)\n", (unsigned)hr);
        return false;
    }

    return true;
}

void RendererD3D11::ReleaseRenderTarget() {
    SafeRelease(m_rtv);
}

void RendererD3D11::Resize(int width, int height) {
    if (!m_swapChain || width <= 0 || height <= 0) return;

    // 동일 크기면 스킵
    if (width == m_width && height == m_height) return;

    m_width = width;
    m_height = height;

    ReleaseRenderTarget();

    // 0을 주면 기존 설정 유지(포맷 등). 버퍼 수만 0으로 두면 기존 유지됨.
    HRESULT hr = m_swapChain->ResizeBuffers(
        0,
        (UINT)width,
        (UINT)height,
        DXGI_FORMAT_UNKNOWN,
        0
    );

    if (FAILED(hr)) {
        std::printf("ResizeBuffers failed (hr=0x%08X)\n", (unsigned)hr);
        return;
    }

    CreateRenderTarget();
}

void RendererD3D11::BeginFrame(float r, float g, float b, float a) {
    if (!m_context || !m_rtv) return;

    const float clearColor[4] = { r, g, b, a };
    m_context->OMSetRenderTargets(1, &m_rtv, nullptr);
    m_context->ClearRenderTargetView(m_rtv, clearColor);
}

void RendererD3D11::EndFrame() {
    if (!m_swapChain) return;
    m_swapChain->Present(1, 0);
}
