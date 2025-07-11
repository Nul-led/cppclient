module;

#include <emscripten/emscripten.h>
#include <emscripten/val.h>

export module Viewport;

import Canvas;

export namespace Viewport {
    uint32_t screenWidth = 1920;
    uint32_t screenHeight = 1080;

    uint64_t frameCount = 0;
    double deltaTime = 1000.0f / 60.0f;
    double now = 0.0f;
    double renderTime = 0.0f;

    Canvas2d* canvas;
    Context2d* ctx;

    void init() {
        canvas = new Canvas2d(Canvas2d::getSurface("canvas"));
        ctx = new Context2d(*canvas);
    }

    void updateTime(const double time, const double mspt, const double latency) {
        deltaTime = time - now;
        renderTime = time - (mspt / 2.0f + latency);
        now = time;
    }

    void updateScreen() {
        screenWidth = emscripten::val::global("innerWidth").as<int>() ;
        screenHeight = emscripten::val::global("innerHeight").as<int>();
        canvas->setSize(screenWidth, screenHeight);
        canvas->setStyleSize(screenWidth, screenHeight);
    }
}