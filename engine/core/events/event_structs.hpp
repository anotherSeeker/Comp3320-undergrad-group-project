#pragma once

struct SK_MOUSE_MOVE_EVENT{
    double mouseX;
    double mouseY;
    double deltaX;
    double deltaY;
};

struct SK_WINDOW_RESIZE_EVENT{
    int width;
    int height;
};
