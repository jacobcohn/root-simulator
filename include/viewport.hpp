#pragma once

struct Viewport {
    int width = 1;
    int height = 1;
};

void ResizeCanvasToViewport(Viewport& viewport);
