#include "viewport.hpp"

#include "raylib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/html5.h>
#endif

#include <algorithm>
#include <cmath>

void ResizeCanvasToViewport(Viewport& viewport) {
#if defined(PLATFORM_WEB)
    double cssWidth = 0.0;
    double cssHeight = 0.0;
    emscripten_get_element_css_size("#canvas", &cssWidth, &cssHeight);

    const int width = std::max(1, static_cast<int>(std::lround(cssWidth)));
    const int height = std::max(1, static_cast<int>(std::lround(cssHeight)));

    if (width != viewport.width || height != viewport.height) {
        viewport.width = width;
        viewport.height = height;
        emscripten_set_canvas_element_size("#canvas", viewport.width, viewport.height);
        SetWindowSize(viewport.width, viewport.height);
    }
#else
    viewport.width = GetScreenWidth();
    viewport.height = GetScreenHeight();
#endif
}
