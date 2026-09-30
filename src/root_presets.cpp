#include "root_presets.hpp"

#include <chrono>

namespace {
unsigned int CreateRandomSeed() {
    const auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    return static_cast<unsigned int>(now);
}
}

RootGenerationParams CreateDefaultRootGenerationParams() {
    RootGenerationParams params;
    params.seed = CreateRandomSeed();
    params.maxRoots = 1500;
    params.crownColor = Color{184, 142, 92, 255};
    params.orders = {
        RootOrderParams{
            .color = Color{174, 134, 88, 255},
            .meanLength = 3.4f,
            .lengthVariation = 0.08f,
            .meanStartRadius = 0.092f,
            .radiusVariation = 0.08f,
            .taper = 0.88f,
            .minSegments = 24,
            .maxSegments = 30,
            .branchProbability = 1.0f,
            .minBranchesPerRoot = 18,
            .maxBranchesPerRoot = 26,
            .branchStartMin = 0.04f,
            .branchStartMax = 0.92f,
            .branchAngleMinDegrees = 42.0f,
            .branchAngleMaxDegrees = 82.0f,
            .downwardBias = 0.68f,
            .outwardBias = 0.07f,
            .randomness = 0.12f
        },
        RootOrderParams{
            .color = Color{156, 119, 78, 255},
            .meanLength = 1.85f,
            .lengthVariation = 0.32f,
            .meanStartRadius = 0.032f,
            .radiusVariation = 0.22f,
            .taper = 0.78f,
            .minSegments = 8,
            .maxSegments = 13,
            .branchProbability = 0.98f,
            .minBranchesPerRoot = 4,
            .maxBranchesPerRoot = 8,
            .branchStartMin = 0.25f,
            .branchStartMax = 0.95f,
            .branchAngleMinDegrees = 35.0f,
            .branchAngleMaxDegrees = 78.0f,
            .downwardBias = 0.18f,
            .outwardBias = 0.44f,
            .randomness = 0.2f
        },
        RootOrderParams{
            .color = Color{122, 88, 58, 255},
            .meanLength = 0.55f,
            .lengthVariation = 0.45f,
            .meanStartRadius = 0.009f,
            .radiusVariation = 0.28f,
            .taper = 0.72f,
            .minSegments = 4,
            .maxSegments = 7,
            .branchProbability = 0.82f,
            .minBranchesPerRoot = 2,
            .maxBranchesPerRoot = 4,
            .branchStartMin = 0.35f,
            .branchStartMax = 0.95f,
            .branchAngleMinDegrees = 28.0f,
            .branchAngleMaxDegrees = 72.0f,
            .downwardBias = 0.14f,
            .outwardBias = 0.48f,
            .randomness = 0.24f
        },
        RootOrderParams{
            .color = Color{98, 70, 45, 255},
            .meanLength = 0.24f,
            .lengthVariation = 0.5f,
            .meanStartRadius = 0.0035f,
            .radiusVariation = 0.3f,
            .taper = 0.65f,
            .minSegments = 2,
            .maxSegments = 4,
            .branchProbability = 0.25f,
            .minBranchesPerRoot = 1,
            .maxBranchesPerRoot = 2,
            .downwardBias = 0.1f,
            .outwardBias = 0.52f,
            .randomness = 0.28f
        }
    };

    return params;
}
