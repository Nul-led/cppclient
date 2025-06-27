#include <emscripten.h>
#include <iostream>
#include <emscripten/val.h>

import Color;
import World;
import Entity;
import Viewport;

void frame() {
    Viewport::frame(emscripten_get_now(), 40, 20);
}

int main() {
    emscripten_set_main_loop(frame, 0, false);

    Viewport::init();

    return 0;
}