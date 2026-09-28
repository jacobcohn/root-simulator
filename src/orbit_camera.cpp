#include "orbit_camera.hpp"

#include <algorithm>
#include <cmath>

namespace {
constexpr float kPi = 3.14159265358979323846f;
}

void ResetCameraOrbit(OrbitCamera& orbitCamera) {
    orbitCamera.target = Vector3{0.0f, -1.4f, 0.0f};
    orbitCamera.azimuth = 0.65f;
    orbitCamera.elevation = 0.36f;
    orbitCamera.distance = 6.5f;
}

void UpdateCameraFromOrbit(OrbitCamera& orbitCamera) {
    const float horizontalDistance = std::cos(orbitCamera.elevation) * orbitCamera.distance;

    orbitCamera.camera.position = Vector3{
        orbitCamera.target.x + std::sin(orbitCamera.azimuth) * horizontalDistance,
        orbitCamera.target.y + std::sin(orbitCamera.elevation) * orbitCamera.distance,
        orbitCamera.target.z + std::cos(orbitCamera.azimuth) * horizontalDistance
    };
    orbitCamera.camera.target = orbitCamera.target;
    orbitCamera.camera.up = Vector3{0.0f, 1.0f, 0.0f};
    orbitCamera.camera.fovy = 45.0f;
    orbitCamera.camera.projection = CAMERA_PERSPECTIVE;
}

void HandleCameraInput(OrbitCamera& orbitCamera) {
    if (IsKeyPressed(KEY_A)) {
        orbitCamera.autoOrbit = !orbitCamera.autoOrbit;
    }

    if (IsKeyPressed(KEY_R)) {
        ResetCameraOrbit(orbitCamera);
    }

    const bool isDragging = IsMouseButtonDown(MOUSE_BUTTON_LEFT) ||
        IsMouseButtonDown(MOUSE_BUTTON_RIGHT) ||
        IsMouseButtonDown(MOUSE_BUTTON_MIDDLE);

    if (isDragging) {
        const Vector2 delta = GetMouseDelta();
        orbitCamera.azimuth -= delta.x * 0.008f;
        orbitCamera.elevation += delta.y * 0.006f;
        orbitCamera.elevation = std::clamp(orbitCamera.elevation, -0.25f * kPi, 0.45f * kPi);
    }

    const float wheel = GetMouseWheelMove();
    if (std::fabs(wheel) > 0.0f) {
        orbitCamera.distance = std::clamp(orbitCamera.distance - wheel * 0.55f, 2.2f, 14.0f);
    }

    if (orbitCamera.autoOrbit && !isDragging) {
        orbitCamera.azimuth += GetFrameTime() * 0.35f;
    }

    UpdateCameraFromOrbit(orbitCamera);
}
