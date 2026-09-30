#include "app.hpp"

#include "orbit_camera.hpp"
#include "overlay.hpp"
#include "root_controls.hpp"
#include "root_generator.hpp"
#include "root_scene.hpp"
#include "viewport.hpp"

#include "raylib.h"

namespace {
constexpr Color kBackgroundColor{10, 12, 18, 255};

Viewport viewport;
OrbitCamera orbitCamera;
RootSystem rootSystem;
RootControls rootControls;

void GenerateNewRandomRootSystem() {
    rootSystem = GenerateRootSystem(CreateRootGenerationParamsFromControls(rootControls));
}

void DrawScene3D() {
    BeginMode3D(orbitCamera.camera);
    DrawRootSystem(rootSystem);
    EndMode3D();
}
}

void InitApp() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(viewport.width, viewport.height, "Root Simulator");

    ResizeCanvasToViewport(viewport);
    InitRootControls(rootControls);
    GenerateNewRandomRootSystem();
    ResetCameraOrbit(orbitCamera);
    UpdateCameraFromOrbit(orbitCamera);
    SetTargetFPS(60);
}

void UpdateDrawFrame() {
    ResizeCanvasToViewport(viewport);
    UpdateRootControls(rootControls);
    if (ConsumeNewRootRequest(rootControls)) {
        GenerateNewRandomRootSystem();
    }

    if (!RootControlsCaptureMouse(rootControls, viewport)) {
        HandleCameraInput(orbitCamera);
    } else {
        UpdateCameraFromOrbit(orbitCamera);
    }

    BeginDrawing();
    ClearBackground(kBackgroundColor);
    DrawScene3D();
    DrawOverlay(viewport, orbitCamera);
    DrawRootControls(rootControls, viewport);
    EndDrawing();
}

void ShutdownApp() {
    CloseWindow();
}
