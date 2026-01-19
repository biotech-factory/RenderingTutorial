#include "WindowSDL.h"
#include "RendererD3D11.h"

int main(int, char**) {
    WindowSDL window;
    if (!window.Create("RenderingTutorial", 1280, 720)) return 1;

    RendererD3D11 renderer;
    if (!renderer.Initialize(window.GetSDLWindow(), window.GetWidth(), window.GetHeight())) return 1;

    bool running = true;
    while (running) {
        window.PollEvents(running);

        int w = 0, h = 0;
        if (window.ConsumeResize(w, h)) {
            renderer.Resize(w, h);
        }

        renderer.BeginFrame(0.08f, 0.08f, 0.10f, 1.0f);
        renderer.EndFrame();
    }
    return 0;
}
