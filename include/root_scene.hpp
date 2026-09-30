#pragma once

#include "raylib.h"

#include <vector>

struct Root {
    std::vector<Vector3> points;
    std::vector<float> radii;
    int parentRootIndex = -1;
    int parentPointIndex = -1;
    int order = 0;
    float growthProgress = 1.0f;
};

struct RootSystem {
    std::vector<Root> roots;
};

RootSystem CreateSampleDicotRootSystem();
void DrawRootSystem(const RootSystem& rootSystem);
