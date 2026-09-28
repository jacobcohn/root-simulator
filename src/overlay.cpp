#include "overlay.hpp"

#include "raylib.h"

namespace {
constexpr Color kOverlayTextColor{150, 160, 175, 255};
}

void DrawOverlay(const Viewport& viewport, const OrbitCamera& orbitCamera) {
    DrawText("Root Simulator", 28, 24, 32, RAYWHITE);
    DrawText("Drag: orbit  |  Wheel: zoom  |  A: auto-orbit  |  R: reset", 30, 62, 18, kOverlayTextColor);
    DrawText(
        orbitCamera.autoOrbit ? "Auto-orbit: on" : "Auto-orbit: off",
        30,
        viewport.height - 34,
        18,
        kOverlayTextColor
    );
}
