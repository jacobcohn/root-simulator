#pragma once

#include "raylib.h"

#include <vector>

struct Root {
    std::vector<Vector3> points;
    std::vector<float> radii;
    Color color{122, 88, 58, 255};
    int parentRootIndex = -1;
    int parentPointIndex = -1;
    int order = 0;
    float growthProgress = 1.0f;
};

struct RootSystem {
    std::vector<Root> roots;
    Color crownColor{218, 184, 128, 255};
};

void DrawRootSystem(const RootSystem& rootSystem);
