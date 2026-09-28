#pragma once

#include "raylib.h"

struct OrbitCamera {
    Camera3D camera{};
    Vector3 target{0.0f, -1.4f, 0.0f};
    float azimuth = 0.65f;
    float elevation = 0.36f;
    float distance = 6.5f;
    bool autoOrbit = false;
};

void ResetCameraOrbit(OrbitCamera& orbitCamera);
void UpdateCameraFromOrbit(OrbitCamera& orbitCamera);
void HandleCameraInput(OrbitCamera& orbitCamera);
