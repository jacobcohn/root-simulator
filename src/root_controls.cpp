#include "root_controls.hpp"

#include "raylib.h"

#include <algorithm>
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

void DrawSliderRow(const char* label, float& value, Rectangle row) {
    DrawText(label, static_cast<int>(row.x), static_cast<int>(row.y), 18, kBodyText);

    char valueText[16]{};
    std::snprintf(valueText, sizeof(valueText), "%d%%", static_cast<int>(value * 100.0f + 0.5f));
    DrawText(valueText, static_cast<int>(row.x + row.width - 48.0f), static_cast<int>(row.y), 18, kMutedText);

    value = Slider(Rectangle{row.x + 140.0f, row.y + 9.0f, row.width - 210.0f, 8.0f}, value);
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

void ApplyPreset(RootControls& controls, RootPreset preset) {
    controls.selectedPreset = preset;

    switch (preset) {
    case RootPreset::Default:
        controls.orders[0] = RootOrderUiParams{0.62f, 0.72f, 0.34f, 0.30f};
        controls.orders[1] = RootOrderUiParams{0.46f, 0.60f, 0.58f, 0.38f};
        controls.orders[2] = RootOrderUiParams{0.28f, 0.48f, 0.62f, 0.42f};
        break;
    case RootPreset::Deep:
        controls.orders[0] = RootOrderUiParams{0.88f, 0.50f, 0.18f, 0.22f};
        controls.orders[1] = RootOrderUiParams{0.36f, 0.42f, 0.34f, 0.28f};
        controls.orders[2] = RootOrderUiParams{0.22f, 0.35f, 0.42f, 0.30f};
        break;
    case RootPreset::Wide:
        controls.orders[0] = RootOrderUiParams{0.46f, 0.64f, 0.62f, 0.30f};
        controls.orders[1] = RootOrderUiParams{0.72f, 0.60f, 0.82f, 0.34f};
        controls.orders[2] = RootOrderUiParams{0.32f, 0.46f, 0.78f, 0.38f};
        break;
    case RootPreset::Dense:
        controls.orders[0] = RootOrderUiParams{0.58f, 0.92f, 0.46f, 0.28f};
        controls.orders[1] = RootOrderUiParams{0.50f, 0.88f, 0.60f, 0.34f};
        controls.orders[2] = RootOrderUiParams{0.34f, 0.82f, 0.68f, 0.36f};
        break;
    case RootPreset::Sparse:
        controls.orders[0] = RootOrderUiParams{0.68f, 0.28f, 0.34f, 0.18f};
        controls.orders[1] = RootOrderUiParams{0.44f, 0.22f, 0.52f, 0.22f};
        controls.orders[2] = RootOrderUiParams{0.22f, 0.16f, 0.58f, 0.24f};
        break;
    case RootPreset::Wild:
        controls.orders[0] = RootOrderUiParams{0.64f, 0.72f, 0.52f, 0.78f};
        controls.orders[1] = RootOrderUiParams{0.56f, 0.68f, 0.72f, 0.86f};
        controls.orders[2] = RootOrderUiParams{0.36f, 0.56f, 0.80f, 0.92f};
        break;
    }
}

void DrawPresetButtons(RootControls& controls, float x, float y) {
    struct PresetButton { const char* label; RootPreset preset; };
    const PresetButton presets[] = {
        {"Default", RootPreset::Default},
        {"Deep", RootPreset::Deep},
        {"Wide", RootPreset::Wide},
        {"Dense", RootPreset::Dense},
        {"Sparse", RootPreset::Sparse},
        {"Wild", RootPreset::Wild}
    };

    float buttonX = x;
    for (const PresetButton& preset : presets) {
        const float width = static_cast<float>(MeasureText(preset.label, 16)) + 28.0f;
        if (Button(Rectangle{buttonX, y, width, 34.0f}, preset.label, controls.selectedPreset == preset.preset)) {
            ApplyPreset(controls, preset.preset);
        }
        buttonX += width + 10.0f;
    }
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

    DrawText("Preset", static_cast<int>(x), static_cast<int>(y), 18, kMutedText);
    y += 28.0f;
    DrawPresetButtons(controls, x, y);
    y += 58.0f;

    DrawText("Order", static_cast<int>(x), static_cast<int>(y), 18, kMutedText);
    y += 28.0f;
    DrawOrderTabs(controls, x, y);
    y += 64.0f;

    DrawText(kOrderNames[controls.selectedOrder], static_cast<int>(x), static_cast<int>(y), 24, kTitleText);
    y += 44.0f;

    RootOrderUiParams& order = controls.orders[controls.selectedOrder];
    DrawSliderRow("Length", order.length, Rectangle{x, y, rowWidth, 38.0f}); y += 46.0f;
    DrawSliderRow("Branches", order.branchiness, Rectangle{x, y, rowWidth, 38.0f}); y += 46.0f;
    DrawSliderRow("Angle", order.spread, Rectangle{x, y, rowWidth, 38.0f}); y += 46.0f;
    DrawSliderRow("Randomness", order.randomness, Rectangle{x, y, rowWidth, 38.0f}); y += 54.0f;

    DrawText("Length: root size  |  Branches: child roots  |  Angle: outward spread  |  Randomness: irregularity", static_cast<int>(x), static_cast<int>(y), 14, kMutedText);
    y += 46.0f;

    if (Button(Rectangle{x, y, 116.0f, 36.0f}, "New Root")) {
        controls.newRootRequested = true;
    }
    if (Button(Rectangle{x + 130.0f, y, 96.0f, 36.0f}, "Reset")) {
        ApplyPreset(controls, RootPreset::Default);
        controls.selectedOrder = 0;
    }

    DrawText("P: hide parameters", static_cast<int>(x), static_cast<int>(panel.y + panel.height - 34.0f), 14, kMutedText);
}
}

void InitRootControls(RootControls& controls) {
    controls = RootControls{};
    ApplyPreset(controls, RootPreset::Default);
}

void UpdateRootControls(RootControls& controls) {
    if (IsKeyPressed(KEY_P)) {
        controls.visible = !controls.visible;
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

void DrawRootControls(RootControls& controls, const Viewport& viewport) {
    if (!controls.visible) {
        return;
    }

    DrawParameterPanel(controls, viewport);
}
