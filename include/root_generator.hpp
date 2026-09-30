#pragma once

#include "root_scene.hpp"

#include "raylib.h"

#include <vector>

struct RootOrderParams {
    Color color{122, 88, 58, 255};

    float meanLength = 1.0f;
    float lengthVariation = 0.25f;

    float meanStartRadius = 0.03f;
    float radiusVariation = 0.15f;
    float taper = 0.75f;

    int minSegments = 6;
    int maxSegments = 10;

    float branchProbability = 0.25f;
    int minBranchesPerRoot = 1;
    int maxBranchesPerRoot = 3;

    float branchStartMin = 0.2f;
    float branchStartMax = 0.85f;
    float branchAngleMinDegrees = 35.0f;
    float branchAngleMaxDegrees = 70.0f;

    float downwardBias = 0.35f;
    float outwardBias = 0.25f;
    float randomness = 0.18f;

    float growthProgress = 1.0f;
};

struct RootGenerationParams {
    unsigned int seed = 1;
    int maxRoots = 250;
    int minPrimaryRoots = 1;
    int maxPrimaryRoots = 1;
    Color crownColor{218, 184, 128, 255};
    std::vector<RootOrderParams> orders;
};

RootSystem GenerateRootSystem(const RootGenerationParams& params);
