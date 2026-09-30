#pragma once

#include "root_generator.hpp"
#include "viewport.hpp"

struct RootOrderUiParams {
    float length = 0.5f;
    float branchiness = 0.5f;
    float spread = 0.5f;
    float randomness = 0.35f;
};

struct RootControls {
    RootOrderUiParams orders[3]{};
    int selectedOrder = 0;
    bool visible = false;
    bool newRootRequested = false;
};

void InitRootControls(RootControls& controls);
void UpdateRootControls(RootControls& controls);
bool RootControlsCaptureMouse(const RootControls& controls, const Viewport& viewport);
bool ConsumeNewRootRequest(RootControls& controls);
RootGenerationParams CreateRootGenerationParamsFromControls(const RootControls& controls);
void DrawRootControls(RootControls& controls, const Viewport& viewport);
