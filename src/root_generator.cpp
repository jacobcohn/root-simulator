#include "root_generator.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <random>
#include <vector>

namespace {
constexpr float kPi = 3.14159265358979323846f;
constexpr float kTwoPi = 2.0f * kPi;

struct BranchSpawn {
    Vector3 point{};
    Vector3 direction{0.0f, -1.0f, 0.0f};
    int parentPointIndex = 0;
};

float Dot(Vector3 a, Vector3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Cross(Vector3 a, Vector3 b) {
    return Vector3{
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

Vector3 Add(Vector3 a, Vector3 b) {
    return Vector3{a.x + b.x, a.y + b.y, a.z + b.z};
}

Vector3 Scale(Vector3 value, float amount) {
    return Vector3{value.x * amount, value.y * amount, value.z * amount};
}

Vector3 Lerp(Vector3 start, Vector3 end, float amount) {
    return Add(Scale(start, 1.0f - amount), Scale(end, amount));
}

float Length(Vector3 value) {
    return std::sqrt(Dot(value, value));
}

Vector3 Normalize(Vector3 value, Vector3 fallback = Vector3{0.0f, -1.0f, 0.0f}) {
    const float length = Length(value);
    if (length <= 0.00001f) {
        return fallback;
    }

    return Scale(value, 1.0f / length);
}

float RandomFloat(std::mt19937& random, float minValue, float maxValue) {
    if (maxValue < minValue) {
        std::swap(minValue, maxValue);
    }

    return std::uniform_real_distribution<float>(minValue, maxValue)(random);
}

int RandomInt(std::mt19937& random, int minValue, int maxValue) {
    if (maxValue < minValue) {
        std::swap(minValue, maxValue);
    }

    return std::uniform_int_distribution<int>(minValue, maxValue)(random);
}

bool RandomChance(std::mt19937& random, float probability) {
    return RandomFloat(random, 0.0f, 1.0f) <= std::clamp(probability, 0.0f, 1.0f);
}

float RandomVariation(std::mt19937& random, float mean, float variation) {
    const float safeVariation = std::max(0.0f, variation);
    return std::max(0.0f, RandomFloat(random, mean * (1.0f - safeVariation), mean * (1.0f + safeVariation)));
}

Vector3 RandomUnitVector(std::mt19937& random) {
    const float y = RandomFloat(random, -1.0f, 1.0f);
    const float angle = RandomFloat(random, 0.0f, kTwoPi);
    const float radius = std::sqrt(std::max(0.0f, 1.0f - y * y));

    return Vector3{radius * std::cos(angle), y, radius * std::sin(angle)};
}

Vector3 RandomHorizontalUnitVector(std::mt19937& random) {
    const float angle = RandomFloat(random, 0.0f, kTwoPi);
    return Vector3{std::cos(angle), 0.0f, std::sin(angle)};
}

Vector3 LocalOutwardDirection(Vector3 direction, std::mt19937& random) {
    const Vector3 horizontalDirection{direction.x, 0.0f, direction.z};
    if (Length(horizontalDirection) <= 0.00001f) {
        return RandomHorizontalUnitVector(random);
    }

    return Normalize(horizontalDirection, Vector3{0.0f, 0.0f, 0.0f});
}

Vector3 BranchDirection(Vector3 parentDirection, float angleDegrees, float rotationRadians) {
    const Vector3 forward = Normalize(parentDirection);
    const Vector3 reference = std::fabs(Dot(forward, Vector3{0.0f, 1.0f, 0.0f})) < 0.95f
        ? Vector3{0.0f, 1.0f, 0.0f}
        : Vector3{1.0f, 0.0f, 0.0f};

    const Vector3 right = Normalize(Cross(reference, forward), Vector3{1.0f, 0.0f, 0.0f});
    const Vector3 up = Normalize(Cross(forward, right), Vector3{0.0f, 1.0f, 0.0f});
    const Vector3 radial = Add(Scale(right, std::cos(rotationRadians)), Scale(up, std::sin(rotationRadians)));

    const float angleRadians = std::clamp(angleDegrees, 0.0f, 150.0f) * kPi / 180.0f;
    return Normalize(Add(Scale(forward, std::cos(angleRadians)), Scale(radial, std::sin(angleRadians))));
}

BranchSpawn BranchSpawnAt(const Root& root, float position) {
    const int segmentCount = static_cast<int>(root.points.size()) - 1;
    const float exactIndex = std::clamp(position, 0.0f, 1.0f) * static_cast<float>(segmentCount);
    const int segmentIndex = std::clamp(static_cast<int>(std::floor(exactIndex)), 0, segmentCount - 1);
    const float segmentAmount = exactIndex - static_cast<float>(segmentIndex);

    const Vector3 start = root.points[segmentIndex];
    const Vector3 end = root.points[segmentIndex + 1];

    return BranchSpawn{
        .point = Lerp(start, end, segmentAmount),
        .direction = Normalize(Add(end, Scale(start, -1.0f))),
        .parentPointIndex = std::clamp(static_cast<int>(std::lround(exactIndex)), 0, segmentCount)
    };
}

std::vector<BranchSpawn> ChooseBranchSpawns(
    const Root& root,
    const RootOrderParams& orderParams,
    std::mt19937& random
) {
    if (root.points.size() < 2 || orderParams.maxBranchesPerRoot <= 0) {
        return {};
    }

    if (!RandomChance(random, orderParams.branchProbability)) {
        return {};
    }

    const int branchCount = RandomInt(
        random,
        std::max(0, orderParams.minBranchesPerRoot),
        std::max(0, orderParams.maxBranchesPerRoot)
    );

    float start = std::clamp(orderParams.branchStartMin, 0.0f, 1.0f);
    float end = std::clamp(orderParams.branchStartMax, 0.0f, 1.0f);
    if (end < start) {
        std::swap(start, end);
    }

    std::vector<float> positions;
    positions.reserve(static_cast<std::size_t>(branchCount));
    for (int i = 0; i < branchCount; ++i) {
        positions.push_back(RandomFloat(random, start, end));
    }
    std::sort(positions.begin(), positions.end());

    std::vector<BranchSpawn> spawns;
    spawns.reserve(positions.size());
    for (float position : positions) {
        spawns.push_back(BranchSpawnAt(root, position));
    }

    return spawns;
}

Root MakeRoot(
    const RootOrderParams& orderParams,
    int order,
    Vector3 start,
    Vector3 direction,
    int parentRootIndex,
    int parentPointIndex,
    std::mt19937& random
) {
    const float length = std::max(0.01f, RandomVariation(random, orderParams.meanLength, orderParams.lengthVariation));
    const float startRadius = std::max(0.0005f, RandomVariation(random, orderParams.meanStartRadius, orderParams.radiusVariation));
    const int segmentCount = std::max(1, RandomInt(random, orderParams.minSegments, orderParams.maxSegments));
    const float segmentLength = length / static_cast<float>(segmentCount);

    Root root;
    root.color = orderParams.color;
    root.parentRootIndex = parentRootIndex;
    root.parentPointIndex = parentPointIndex;
    root.order = order;
    root.growthProgress = std::clamp(orderParams.growthProgress, 0.0f, 1.0f);
    root.points.reserve(static_cast<std::size_t>(segmentCount + 1));
    root.radii.reserve(static_cast<std::size_t>(segmentCount + 1));
    root.points.push_back(start);
    root.radii.push_back(startRadius);

    direction = Normalize(direction);

    const Vector3 initialDirection = direction;
    const float directionPersistence = order == 0 ? 0.25f : 0.9f;
    const float downwardBias = order == 0 ? orderParams.downwardBias : orderParams.downwardBias * 0.12f;
    const float outwardBias = order == 0 ? 0.0f : std::max(0.0f, orderParams.outwardBias) * 0.25f;
    const Vector3 outwardDirection = outwardBias > 0.0f
        ? LocalOutwardDirection(direction, random)
        : Vector3{0.0f, 0.0f, 0.0f};

    for (int segment = 1; segment <= segmentCount; ++segment) {
        const Vector3 point = root.points.back();
        const Vector3 desiredDirection = Normalize(Add(
            Add(
                Add(direction, Scale(initialDirection, directionPersistence)),
                Scale(Vector3{0.0f, -1.0f, 0.0f}, downwardBias)
            ),
            Add(
                Scale(outwardDirection, outwardBias),
                Scale(RandomUnitVector(random), orderParams.randomness)
            )
        ));

        direction = desiredDirection;

        const float amount = static_cast<float>(segment) / static_cast<float>(segmentCount);
        const float radius = startRadius * std::max(0.04f, 1.0f - std::clamp(orderParams.taper, 0.0f, 0.98f) * amount);

        root.points.push_back(Add(point, Scale(direction, segmentLength)));
        root.radii.push_back(radius);
    }

    return root;
}

void GenerateRoot(
    RootSystem& rootSystem,
    const RootGenerationParams& params,
    std::mt19937& random,
    int order,
    Vector3 start,
    Vector3 direction,
    int parentRootIndex,
    int parentPointIndex
) {
    if (static_cast<int>(rootSystem.roots.size()) >= params.maxRoots) {
        return;
    }

    const RootOrderParams& orderParams = params.orders[static_cast<std::size_t>(order)];
    Root root = MakeRoot(orderParams, order, start, direction, parentRootIndex, parentPointIndex, random);

    const int childOrder = order + 1;
    const bool canBranch = static_cast<std::size_t>(childOrder) < params.orders.size();
    const std::vector<BranchSpawn> branchSpawns = canBranch
        ? ChooseBranchSpawns(root, orderParams, random)
        : std::vector<BranchSpawn>{};

    const int rootIndex = static_cast<int>(rootSystem.roots.size());
    rootSystem.roots.push_back(std::move(root));

    std::vector<float> branchRotations;
    branchRotations.reserve(branchSpawns.size());

    for (std::size_t i = 0; i < branchSpawns.size(); ++i) {
        branchRotations.push_back(RandomFloat(random, 0.0f, kTwoPi));
    }

    for (std::size_t i = 0; i < branchSpawns.size(); ++i) {
        if (static_cast<int>(rootSystem.roots.size()) >= params.maxRoots) {
            return;
        }

        const BranchSpawn& spawn = branchSpawns[i];
        const RootOrderParams& childOrderParams = params.orders[static_cast<std::size_t>(childOrder)];
        const Vector3 childDirection = BranchDirection(
            spawn.direction,
            RandomFloat(random, childOrderParams.branchAngleMinDegrees, childOrderParams.branchAngleMaxDegrees),
            branchRotations[i]
        );

        GenerateRoot(rootSystem, params, random, childOrder, spawn.point, childDirection, rootIndex, spawn.parentPointIndex);
    }
}
}

RootSystem GenerateRootSystem(const RootGenerationParams& params) {
    RootSystem rootSystem;
    rootSystem.crownColor = params.crownColor;

    if (params.orders.empty() || params.maxRoots <= 0) {
        return rootSystem;
    }

    std::mt19937 random(params.seed);
    const int primaryRootCount = RandomInt(
        random,
        std::max(1, params.minPrimaryRoots),
        std::max(1, params.maxPrimaryRoots)
    );

    for (int i = 0; i < primaryRootCount; ++i) {
        const Vector3 direction = primaryRootCount == 1
            ? Vector3{0.0f, -1.0f, 0.0f}
            : BranchDirection(
                Vector3{0.0f, -1.0f, 0.0f},
                RandomFloat(random, 4.0f, 22.0f),
                RandomFloat(random, 0.0f, kTwoPi)
            );

        GenerateRoot(
            rootSystem,
            params,
            random,
            0,
            Vector3{0.0f, 0.0f, 0.0f},
            direction,
            -1,
            -1
        );
    }

    return rootSystem;
}
