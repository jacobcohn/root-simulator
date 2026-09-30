#include "root_scene.hpp"

#include "raylib.h"

#include <algorithm>
#include <cmath>
#include <cstddef>

namespace {
constexpr Color kTaprootColor{205, 171, 118, 255};
constexpr Color kLateralColor{156, 119, 78, 255};
constexpr Color kFineRootColor{122, 88, 58, 255};
constexpr Color kJointColor{222, 196, 145, 255};
constexpr Color kCrownColor{218, 184, 128, 255};

Vector3 Lerp(Vector3 start, Vector3 end, float amount) {
    return Vector3{
        start.x + (end.x - start.x) * amount,
        start.y + (end.y - start.y) * amount,
        start.z + (end.z - start.z) * amount
    };
}

float Lerp(float start, float end, float amount) {
    return start + (end - start) * amount;
}

Color ColorForRootOrder(int order) {
    if (order == 0) {
        return kTaprootColor;
    }

    if (order == 1) {
        return kLateralColor;
    }

    return kFineRootColor;
}

void DrawRootSegment(Vector3 start, Vector3 end, float radiusStart, float radiusEnd, Color color) {
    DrawCylinderEx(start, end, radiusStart, radiusEnd, 12, color);
    DrawSphere(end, radiusEnd * 1.08f, color);
}

bool IsDrawable(const Root& root) {
    return root.points.size() >= 2 && root.points.size() == root.radii.size() && root.growthProgress > 0.0f;
}

void DrawRoot(const Root& root) {
    if (!IsDrawable(root)) {
        return;
    }

    const Color color = ColorForRootOrder(root.order);
    const float progress = std::clamp(root.growthProgress, 0.0f, 1.0f);
    const float visibleSegmentCount = static_cast<float>(root.points.size() - 1) * progress;
    const auto fullSegmentCount = static_cast<std::size_t>(std::floor(visibleSegmentCount));
    const float partialSegmentAmount = visibleSegmentCount - static_cast<float>(fullSegmentCount);

    for (std::size_t i = 0; i < fullSegmentCount; ++i) {
        DrawRootSegment(root.points[i], root.points[i + 1], root.radii[i], root.radii[i + 1], color);
    }

    if (fullSegmentCount + 1 < root.points.size() && partialSegmentAmount > 0.0f) {
        const Vector3 end = Lerp(root.points[fullSegmentCount], root.points[fullSegmentCount + 1], partialSegmentAmount);
        const float radiusEnd = Lerp(root.radii[fullSegmentCount], root.radii[fullSegmentCount + 1], partialSegmentAmount);
        DrawRootSegment(root.points[fullSegmentCount], end, root.radii[fullSegmentCount], radiusEnd, color);
    }
}
}

RootSystem CreateSampleDicotRootSystem() {
    RootSystem rootSystem;

    rootSystem.roots.push_back(Root{
        .points = {
            Vector3{0.0f, 0.0f, 0.0f},
            Vector3{0.03f, -0.35f, 0.02f},
            Vector3{0.08f, -0.78f, 0.05f},
            Vector3{-0.02f, -1.18f, 0.02f},
            Vector3{-0.08f, -1.62f, -0.04f},
            Vector3{0.03f, -2.08f, 0.02f},
            Vector3{0.08f, -2.52f, 0.06f},
            Vector3{0.02f, -2.96f, 0.02f},
            Vector3{0.0f, -3.38f, 0.0f}
        },
        .radii = {0.092f, 0.084f, 0.074f, 0.064f, 0.052f, 0.040f, 0.030f, 0.022f, 0.012f},
        .order = 0
    });

    rootSystem.roots.push_back(Root{
        .points = {
            rootSystem.roots[0].points[2],
            Vector3{-0.28f, -0.92f, 0.20f},
            Vector3{-0.62f, -1.08f, 0.35f},
            Vector3{-0.98f, -1.27f, 0.45f}
        },
        .radii = {0.036f, 0.027f, 0.019f, 0.010f},
        .parentRootIndex = 0,
        .parentPointIndex = 2,
        .order = 1
    });

    rootSystem.roots.push_back(Root{
        .points = {
            rootSystem.roots[0].points[3],
            Vector3{0.34f, -1.34f, -0.20f},
            Vector3{0.74f, -1.52f, -0.36f},
            Vector3{1.08f, -1.76f, -0.44f}
        },
        .radii = {0.032f, 0.024f, 0.016f, 0.008f},
        .parentRootIndex = 0,
        .parentPointIndex = 3,
        .order = 1
    });

    rootSystem.roots.push_back(Root{
        .points = {
            rootSystem.roots[0].points[4],
            Vector3{-0.32f, -1.84f, -0.25f},
            Vector3{-0.66f, -2.05f, -0.46f},
            Vector3{-0.86f, -2.32f, -0.58f}
        },
        .radii = {0.027f, 0.020f, 0.013f, 0.007f},
        .parentRootIndex = 0,
        .parentPointIndex = 4,
        .order = 1
    });

    rootSystem.roots.push_back(Root{
        .points = {
            rootSystem.roots[0].points[5],
            Vector3{0.37f, -2.28f, 0.20f},
            Vector3{0.73f, -2.50f, 0.33f},
            Vector3{0.95f, -2.78f, 0.38f}
        },
        .radii = {0.022f, 0.016f, 0.010f, 0.005f},
        .parentRootIndex = 0,
        .parentPointIndex = 5,
        .order = 1
    });

    rootSystem.roots.push_back(Root{
        .points = {
            rootSystem.roots[1].points[2],
            Vector3{-0.70f, -1.28f, 0.55f},
            Vector3{-0.84f, -1.46f, 0.70f}
        },
        .radii = {0.010f, 0.007f, 0.0035f},
        .parentRootIndex = 1,
        .parentPointIndex = 2,
        .order = 2
    });

    rootSystem.roots.push_back(Root{
        .points = {
            rootSystem.roots[2].points[2],
            Vector3{0.84f, -1.72f, -0.54f},
            Vector3{0.98f, -1.94f, -0.68f}
        },
        .radii = {0.008f, 0.0055f, 0.003f},
        .parentRootIndex = 2,
        .parentPointIndex = 2,
        .order = 2
    });

    return rootSystem;
}

void DrawRootSystem(const RootSystem& rootSystem) {
    DrawSphere(Vector3{0.0f, 0.02f, 0.0f}, 0.14f, kCrownColor);

    for (const Root& root : rootSystem.roots) {
        DrawRoot(root);
    }

    for (const Root& root : rootSystem.roots) {
        if (root.parentRootIndex >= 0 && !root.points.empty() && !root.radii.empty()) {
            DrawSphere(root.points.front(), root.radii.front() * 1.35f, kJointColor);
        }
    }
}
