#include "root_scene.hpp"

#include "raylib.h"

#include <algorithm>
#include <cmath>
#include <cstddef>

namespace {
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

void DrawRootSegment(Vector3 start, Vector3 end, float radiusStart, float radiusEnd, Color color) {
    DrawCylinderEx(start, end, radiusStart, radiusEnd, 12, color);
    DrawSphere(end, radiusEnd * 0.82f, color);
}

bool IsDrawable(const Root& root) {
    return root.points.size() >= 2 && root.points.size() == root.radii.size() && root.growthProgress > 0.0f;
}

void DrawRoot(const Root& root) {
    if (!IsDrawable(root)) {
        return;
    }

    const float progress = std::clamp(root.growthProgress, 0.0f, 1.0f);
    const float visibleSegmentCount = static_cast<float>(root.points.size() - 1) * progress;
    const auto fullSegmentCount = static_cast<std::size_t>(std::floor(visibleSegmentCount));
    const float partialSegmentAmount = visibleSegmentCount - static_cast<float>(fullSegmentCount);

    for (std::size_t i = 0; i < fullSegmentCount; ++i) {
        DrawRootSegment(root.points[i], root.points[i + 1], root.radii[i], root.radii[i + 1], root.color);
    }

    if (fullSegmentCount + 1 < root.points.size() && partialSegmentAmount > 0.0f) {
        const Vector3 end = Lerp(root.points[fullSegmentCount], root.points[fullSegmentCount + 1], partialSegmentAmount);
        const float radiusEnd = Lerp(root.radii[fullSegmentCount], root.radii[fullSegmentCount + 1], partialSegmentAmount);
        DrawRootSegment(root.points[fullSegmentCount], end, root.radii[fullSegmentCount], radiusEnd, root.color);
    }
}
}

void DrawRootSystem(const RootSystem& rootSystem) {
    DrawSphere(Vector3{0.0f, 0.02f, 0.0f}, 0.105f, rootSystem.crownColor);

    for (const Root& root : rootSystem.roots) {
        DrawRoot(root);
    }

    for (const Root& root : rootSystem.roots) {
        if (root.parentRootIndex >= 0 && !root.points.empty() && !root.radii.empty()) {
            const bool hasParent = static_cast<std::size_t>(root.parentRootIndex) < rootSystem.roots.size();
            const Color branchColor = hasParent ? rootSystem.roots[root.parentRootIndex].color : root.color;
            DrawSphere(root.points.front(), root.radii.front() * 0.9f, branchColor);
        }
    }
}
