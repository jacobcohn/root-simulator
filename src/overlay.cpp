#include "overlay.hpp"

#include "raylib.h"

#include <cstdio>

namespace {
constexpr Color kOverlayTextColor{150, 160, 175, 255};
}

void DrawOverlay(const Viewport& viewport, const OrbitCamera& orbitCamera) {
    (void)viewport;

    char controlsText[180]{};
    std::snprintf(
        controlsText,
        sizeof(controlsText),
        "Drag: rotate  |  Wheel: zoom  |  R: reset  |  P: parameters  |  A: %s",
        orbitCamera.autoOrbit ? "no orbit" : "orbit"
    );

    DrawText("Root Simulator", 28, 24, 32, RAYWHITE);
    DrawText(controlsText, 30, 62, 18, kOverlayTextColor);
}
