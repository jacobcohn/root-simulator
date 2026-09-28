#include "root_scene.hpp"

#include "raylib.h"

namespace {
void DrawRootSegment(Vector3 start, Vector3 end, float radiusStart, float radiusEnd, Color color) {
    DrawCylinderEx(start, end, radiusStart, radiusEnd, 12, color);
    DrawSphere(end, radiusEnd * 1.15f, color);
}
}

void DrawPlaceholderRoot() {
    constexpr Color mainRoot{205, 171, 118, 255};
    constexpr Color sideRoot{156, 119, 78, 255};
    constexpr Color joint{222, 196, 145, 255};
    constexpr Color sprout{93, 174, 113, 255};

    DrawSphere(Vector3{0.0f, 0.06f, 0.0f}, 0.18f, sprout);

    const Vector3 p0{0.0f, -0.05f, 0.0f};
    const Vector3 p1{0.08f, -0.85f, 0.05f};
    const Vector3 p2{-0.07f, -1.65f, -0.03f};
    const Vector3 p3{0.05f, -2.45f, 0.06f};
    const Vector3 p4{0.0f, -3.35f, 0.0f};

    DrawRootSegment(p0, p1, 0.085f, 0.072f, mainRoot);
    DrawRootSegment(p1, p2, 0.072f, 0.055f, mainRoot);
    DrawRootSegment(p2, p3, 0.055f, 0.038f, mainRoot);
    DrawRootSegment(p3, p4, 0.038f, 0.018f, mainRoot);

    DrawSphere(p1, 0.083f, joint);
    DrawSphere(p2, 0.064f, joint);
    DrawSphere(p3, 0.046f, joint);

    DrawRootSegment(p1, Vector3{-0.82f, -1.18f, 0.34f}, 0.035f, 0.014f, sideRoot);
    DrawRootSegment(p1, Vector3{0.72f, -1.08f, -0.42f}, 0.032f, 0.012f, sideRoot);
    DrawRootSegment(p2, Vector3{-0.64f, -2.02f, -0.52f}, 0.028f, 0.010f, sideRoot);
    DrawRootSegment(p2, Vector3{0.93f, -2.15f, 0.22f}, 0.028f, 0.010f, sideRoot);
    DrawRootSegment(p3, Vector3{-0.42f, -2.82f, 0.26f}, 0.020f, 0.008f, sideRoot);
    DrawRootSegment(p3, Vector3{0.48f, -2.95f, -0.28f}, 0.018f, 0.006f, sideRoot);
}
