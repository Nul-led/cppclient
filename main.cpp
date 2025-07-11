#include <emscripten.h>
#include <iostream>
#include <emscripten/val.h>

import Color;
import UI;
import Viewport;
import Inputs;

void frame() {
    Viewport::updateScreen();
    Viewport::updateTime(emscripten_get_now(), 40, 20);

    UI::layout();
    Viewport::ctx->drawImage(UI::canvas->surface, 0, 0);

    Inputs::clear();
}

int main() {
    emscripten_set_main_loop(frame, 0, false);

    Viewport::init();
    Inputs::init();
    UI::init();

    return 0;
}