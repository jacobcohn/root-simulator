#include "raylib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
    #include <emscripten/html5.h>
#endif

#include <algorithm>
#include <cmath>

namespace {
int screenWidth = 1;
int screenHeight = 1;

void ResizeCanvasToViewport() {
#if defined(PLATFORM_WEB)
    double cssWidth = 0.0;
    double cssHeight = 0.0;
    emscripten_get_element_css_size("#canvas", &cssWidth, &cssHeight);

    const int width = std::max(1, static_cast<int>(std::lround(cssWidth)));
    const int height = std::max(1, static_cast<int>(std::lround(cssHeight)));

    if (width != screenWidth || height != screenHeight) {
        screenWidth = width;
        screenHeight = height;
        emscripten_set_canvas_element_size("#canvas", screenWidth, screenHeight);
        SetWindowSize(screenWidth, screenHeight);
    }
#else
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
#endif
}
} // namespace

void UpdateDrawFrame() {
    ResizeCanvasToViewport();

    const float time = static_cast<float>(GetTime());
    const Vector2 mouse = GetMousePosition();
    const bool mouseDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    BeginDrawing();
    ClearBackground(Color{10, 12, 18, 255});

    const float w = static_cast<float>(screenWidth);
    const float h = static_cast<float>(screenHeight);
    const Vector2 seed{w * 0.5f, h * 0.32f};
    const float rootTop = seed.y + 22.0f;
    const float rootBottom = h * 0.86f;
    const float sway = std::sin(time * 1.7f) * 10.0f;

    DrawText("Root Simulator", 28, 24, 32, RAYWHITE);
    DrawText("WebAssembly + raylib proof of concept", 30, 62, 18, Color{150, 160, 175, 255});

    DrawCircleV(seed, mouseDown ? 18.0f : 14.0f, Color{94, 190, 120, 255});
    DrawLineEx(Vector2{seed.x, seed.y + 10.0f}, Vector2{seed.x + sway, rootBottom}, 5.0f, Color{210, 184, 135, 255});

    for (int i = 0; i < 7; ++i) {
        const float t = static_cast<float>(i + 1) / 8.0f;
        const float y = rootTop + (rootBottom - rootTop) * t;
        const float direction = (i % 2 == 0) ? -1.0f : 1.0f;
        const float length = 45.0f + 16.0f * std::sin(time + static_cast<float>(i));
        const float x = seed.x + sway * t;
        DrawLineEx(Vector2{x, y}, Vector2{x + direction * length, y + 26.0f}, 3.0f, Color{176, 142, 96, 255});
    }

    DrawCircleV(mouse, mouseDown ? 13.0f : 8.0f, mouseDown ? Color{255, 220, 120, 255} : Color{110, 180, 255, 220});
    DrawText("Move/click the mouse; resize the browser window.", 30, screenHeight - 34, 18, Color{150, 160, 175, 255});

    EndDrawing();
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Root Simulator");
    ResizeCanvasToViewport();
    SetTargetFPS(60);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }
#endif

    CloseWindow();
    return 0;
}
