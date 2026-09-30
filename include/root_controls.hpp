#pragma once

#include "viewport.hpp"

struct RootOrderUiParams {
    float length = 0.5f;
    float branchiness = 0.5f;
    float spread = 0.5f;
    float randomness = 0.35f;
};

enum class RootPreset {
    Default,
    Deep,
    Wide,
    Dense,
    Sparse,
    Wild
};

struct RootControls {
    RootOrderUiParams orders[3]{};
    int selectedOrder = 0;
    RootPreset selectedPreset = RootPreset::Default;
    bool visible = false;
    bool newRootRequested = false;
};

void InitRootControls(RootControls& controls);
void UpdateRootControls(RootControls& controls);
bool RootControlsCaptureMouse(const RootControls& controls, const Viewport& viewport);
bool ConsumeNewRootRequest(RootControls& controls);
void DrawRootControls(RootControls& controls, const Viewport& viewport);
