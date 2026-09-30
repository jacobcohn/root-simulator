#include "root_controls.hpp"

#include "raylib.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>

namespace {
constexpr Color kPanelBackground{18, 22, 32, 226};
constexpr Color kPanelBorder{62, 72, 90, 255};
constexpr Color kTitleText{238, 242, 248, 255};
constexpr Color kBodyText{174, 184, 198, 255};
constexpr Color kMutedText{116, 126, 142, 255};
constexpr Color kControlFill{37, 45, 60, 255};
constexpr Color kControlHover{49, 59, 78, 255};
constexpr Color kAccent{113, 173, 119, 255};
constexpr Color kAccentHover{139, 202, 145, 255};

const char* kOrderNames[] = {"Primary", "Laterals", "Fine Roots"};

float Lerp(float minValue, float maxValue, float amount) {
    return minValue + (maxValue - minValue) * std::clamp(amount, 0.0f, 1.0f);
}

int LerpInt(int minValue, int maxValue, float amount) {
    return static_cast<int>(std::lround(Lerp(static_cast<float>(minValue), static_cast<float>(maxValue), amount)));
}

unsigned int CreateRandomSeed() {
    const auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    return static_cast<unsigned int>(now);
}

bool PointInRect(Vector2 point, Rectangle rect) {
    return point.x >= rect.x && point.x <= rect.x + rect.width &&
        point.y >= rect.y && point.y <= rect.y + rect.height;
}

bool Button(Rectangle bounds, const char* label, bool selected = false) {
    const Vector2 mouse = GetMousePosition();
    const bool hovered = PointInRect(mouse, bounds);
    const bool pressed = hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    const Color fill = selected ? kAccent : (hovered ? kControlHover : kControlFill);
    const Color text = selected ? Color{8, 12, 10, 255} : (hovered ? kTitleText : kBodyText);

    DrawRectangleRounded(bounds, 0.22f, 8, fill);
    DrawRectangleRoundedLines(bounds, 0.22f, 8, hovered || selected ? kAccentHover : kPanelBorder);

    const int fontSize = 16;
    const int textWidth = MeasureText(label, fontSize);
    DrawText(label,
        static_cast<int>(bounds.x + (bounds.width - static_cast<float>(textWidth)) * 0.5f),
        static_cast<int>(bounds.y + (bounds.height - static_cast<float>(fontSize)) * 0.5f),
        fontSize,
        text);

    return pressed;
}

float Slider(Rectangle trackBounds, float value) {
    const Vector2 mouse = GetMousePosition();
    const bool hovered = PointInRect(mouse, Rectangle{trackBounds.x - 8.0f, trackBounds.y - 8.0f, trackBounds.width + 16.0f, trackBounds.height + 16.0f});
    const bool active = hovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    if (active) {
        value = std::clamp((mouse.x - trackBounds.x) / trackBounds.width, 0.0f, 1.0f);
    }

    const float knobX = trackBounds.x + value * trackBounds.width;
    DrawRectangleRounded(trackBounds, 0.45f, 8, kControlFill);
    DrawRectangleRounded(Rectangle{trackBounds.x, trackBounds.y, knobX - trackBounds.x, trackBounds.height}, 0.45f, 8, active ? kAccentHover : kAccent);
    DrawCircleV(Vector2{knobX, trackBounds.y + trackBounds.height * 0.5f}, hovered || active ? 8.0f : 6.0f, active ? kAccentHover : kTitleText);

    return value;
}

void DrawSliderRow(const char* label, float& value, Rectangle row, const char* valueOverride = nullptr) {
    DrawText(label, static_cast<int>(row.x), static_cast<int>(row.y), 18, kBodyText);

    char valueText[24]{};
    if (valueOverride != nullptr) {
        std::snprintf(valueText, sizeof(valueText), "%s", valueOverride);
    } else {
        std::snprintf(valueText, sizeof(valueText), "%d%%", static_cast<int>(value * 100.0f + 0.5f));
    }
    DrawText(valueText, static_cast<int>(row.x + row.width - 58.0f), static_cast<int>(row.y), 18, kMutedText);

    value = Slider(Rectangle{row.x + 140.0f, row.y + 9.0f, row.width - 220.0f, 8.0f}, value);
}

Rectangle GetPanelBounds(const Viewport& viewport) {
    const float margin = 24.0f;
    return Rectangle{
        margin,
        margin,
        std::max(320.0f, static_cast<float>(viewport.width) - margin * 2.0f),
        std::max(520.0f, static_cast<float>(viewport.height) - margin * 2.0f)
    };
}

void ResetRootControlsToDefault(RootControls& controls) {
    controls.orders[0] = RootOrderUiParams{0.46f, 0.92f, 0.34f, 0.30f};
    controls.orders[1] = RootOrderUiParams{0.46f, 0.84f, 0.42f, 0.38f};
    controls.orders[2] = RootOrderUiParams{0.52f, 0.62f, 0.62f, 0.42f};
    controls.selectedOrder = 0;
}

void DrawOrderTabs(RootControls& controls, float x, float y) {
    float buttonX = x;
    for (int i = 0; i < 3; ++i) {
        const float width = i == 2 ? 124.0f : 104.0f;
        if (Button(Rectangle{buttonX, y, width, 36.0f}, kOrderNames[i], controls.selectedOrder == i)) {
            controls.selectedOrder = i;
        }
        buttonX += width + 10.0f;
    }
}

RootOrderParams CreateOrderParams(
    const RootOrderUiParams& ui,
    Color color,
    float minLength,
    float maxLength,
    float baseRadius,
    float taper,
    int minSegmentBase,
    int maxSegmentBase,
    int maxBranches,
    float branchStartMin,
    float branchStartMax,
    bool canCreateChildren,
    float childBranchAmount
) {
    const float length = Lerp(minLength, maxLength, ui.length);
    const float angle = Lerp(36.0f, 92.0f, ui.spread);
    const float angleRandomness = Lerp(12.0f, 65.0f, ui.randomness);

    RootOrderParams params;
    params.color = color;
    params.meanLength = length;
    params.lengthVariation = Lerp(0.10f, 0.85f, ui.randomness);
    params.meanStartRadius = baseRadius * Lerp(0.82f, 1.22f, ui.length);
    params.radiusVariation = Lerp(0.06f, 0.42f, ui.randomness);
    params.taper = taper;
    params.minSegments = std::max(1, LerpInt(minSegmentBase, minSegmentBase + 4, ui.length));
    params.maxSegments = std::max(params.minSegments, LerpInt(maxSegmentBase, maxSegmentBase + 8, ui.length));
    const int branchCount = canCreateChildren ? LerpInt(0, maxBranches, childBranchAmount) : 0;
    params.branchProbability = branchCount > 0 ? 1.0f : 0.0f;
    params.minBranchesPerRoot = branchCount;
    params.maxBranchesPerRoot = branchCount;
    params.branchStartMin = branchStartMin;
    params.branchStartMax = branchStartMax;
    params.branchAngleMinDegrees = std::clamp(angle - angleRandomness, 5.0f, 130.0f);
    params.branchAngleMaxDegrees = std::clamp(angle + angleRandomness, params.branchAngleMinDegrees, 140.0f);
    params.downwardBias = Lerp(0.78f, 0.08f, ui.spread);
    params.outwardBias = Lerp(0.04f, 0.68f, ui.spread);
    params.randomness = Lerp(0.05f, 0.72f, ui.randomness);
    return params;
}

void DrawParameterPanel(RootControls& controls, const Viewport& viewport) {
    DrawRectangle(0, 0, viewport.width, viewport.height, Color{5, 7, 11, 218});

    const Rectangle panel = GetPanelBounds(viewport);
    DrawRectangleRounded(panel, 0.025f, 12, kPanelBackground);
    DrawRectangleRoundedLines(panel, 0.025f, 12, kPanelBorder);

    const float x = panel.x + 26.0f;
    float y = panel.y + 22.0f;
    const float rowWidth = std::min(720.0f, panel.width - 52.0f);

    DrawText("Root Parameters", static_cast<int>(x), static_cast<int>(y), 28, kTitleText);
    if (Button(Rectangle{panel.x + panel.width - 54.0f, panel.y + 18.0f, 32.0f, 32.0f}, "X")) {
        controls.visible = false;
    }
    y += 46.0f;

    DrawText("Order", static_cast<int>(x), static_cast<int>(y), 18, kMutedText);
    y += 28.0f;
    DrawOrderTabs(controls, x, y);
    y += 64.0f;

    DrawText(kOrderNames[controls.selectedOrder], static_cast<int>(x), static_cast<int>(y), 24, kTitleText);
    y += 44.0f;

    RootOrderUiParams& order = controls.orders[controls.selectedOrder];
    DrawSliderRow("Length", order.length, Rectangle{x, y, rowWidth, 38.0f}); y += 46.0f;

    if (controls.selectedOrder != 0) {
        char branchCountText[24]{};
        const int branchEstimate = controls.selectedOrder == 1
            ? LerpInt(0, 40, order.branchiness)
            : LerpInt(0, 12, order.branchiness);
        std::snprintf(branchCountText, sizeof(branchCountText), "%d", branchEstimate);

        DrawSliderRow("Branches", order.branchiness, Rectangle{x, y, rowWidth, 38.0f}, branchCountText); y += 46.0f;
        DrawSliderRow("Angle", order.spread, Rectangle{x, y, rowWidth, 38.0f}); y += 46.0f;
    }

    DrawSliderRow("Randomness", order.randomness, Rectangle{x, y, rowWidth, 38.0f}); y += 66.0f;

    if (Button(Rectangle{x, y, 116.0f, 36.0f}, "New Root")) {
        controls.newRootRequested = true;
        controls.visible = false;
    }
    if (Button(Rectangle{x + 130.0f, y, 96.0f, 36.0f}, "Reset")) {
        ResetRootControlsToDefault(controls);
    }

    DrawText("N: new root  |  R: reset params  |  P: hide parameters", static_cast<int>(x), static_cast<int>(panel.y + panel.height - 34.0f), 14, kMutedText);
}
}

void InitRootControls(RootControls& controls) {
    controls = RootControls{};
    ResetRootControlsToDefault(controls);
}

void UpdateRootControls(RootControls& controls) {
    if (IsKeyPressed(KEY_P)) {
        controls.visible = !controls.visible;
    }

    if (controls.visible && IsKeyPressed(KEY_N)) {
        controls.newRootRequested = true;
        controls.visible = false;
    }

    if (controls.visible && IsKeyPressed(KEY_R)) {
        ResetRootControlsToDefault(controls);
    }
}

bool RootControlsCaptureMouse(const RootControls& controls, const Viewport& viewport) {
    (void)viewport;
    return controls.visible;
}

bool ConsumeNewRootRequest(RootControls& controls) {
    const bool requested = controls.newRootRequested;
    controls.newRootRequested = false;
    return requested;
}

RootGenerationParams CreateRootGenerationParamsFromControls(const RootControls& controls) {
    const RootOrderUiParams& primary = controls.orders[0];
    const RootOrderUiParams& lateral = controls.orders[1];
    const RootOrderUiParams& fine = controls.orders[2];

    RootGenerationParams params;
    params.seed = CreateRandomSeed();
    params.crownColor = Color{184, 142, 92, 255};

    params.minPrimaryRoots = 1;
    params.maxPrimaryRoots = 1;

    const float density = (primary.branchiness + lateral.branchiness + fine.branchiness) / 3.0f;
    params.maxRoots = LerpInt(140, 700, density);

    RootOrderUiParams primaryParams = primary;
    primaryParams.spread = 0.25f;

    params.orders = {
        CreateOrderParams(
            primaryParams,
            Color{174, 134, 88, 255},
            0.8f,
            3.8f,
            0.086f,
            0.88f,
            8,
            14,
            40,
            0.05f,
            0.92f,
            true,
            lateral.branchiness
        ),
        CreateOrderParams(
            lateral,
            Color{156, 119, 78, 255},
            0.35f,
            2.8f,
            0.030f,
            0.78f,
            3,
            7,
            12,
            0.22f,
            0.95f,
            true,
            fine.branchiness
        ),
        CreateOrderParams(
            fine,
            Color{122, 88, 58, 255},
            0.08f,
            0.75f,
            0.008f,
            0.70f,
            2,
            4,
            0,
            0.35f,
            0.95f,
            false,
            0.0f
        )
    };

    return params;
}

void DrawRootControls(RootControls& controls, const Viewport& viewport) {
    if (!controls.visible) {
        return;
    }

    DrawParameterPanel(controls, viewport);
}
