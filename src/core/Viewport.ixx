module;

#include <emscripten/emscripten.h>
#include <emscripten/val.h>

export module Viewport;

import Canvas;

export struct Viewport {
    static inline uint32_t screenWidth = 1920;
    static inline uint32_t screenHeight = 1080;
    static inline double devicePixelRatio = 1.0f;
    static inline uint64_t frameCount = 0;

    static inline double deltaTime = 1000.0f / 60.0f;
    static inline double now = 0.0f;
    static inline double renderTime = 0.0f;

    static inline Canvas2d* canvas;
    static inline Context2d* ctx;

    static void init() {
        canvas = new Canvas2d(Canvas2d::getSurface("canvas"));
        ctx = new Context2d(*canvas);
    }

    // mspt and latency are to be smoothed, latency is one-way latency not rtt
    static void frame(const double now, const double mspt, const double latency) {
        deltaTime = now - Viewport::now;
        renderTime = now - (mspt / 2.0f + latency);
        Viewport::now = now;

        devicePixelRatio = emscripten::val::global("devicePixelRatio").as<double>();
        screenWidth = emscripten::val::global("innerWidth").as<int>() ;
        screenHeight = emscripten::val::global("innerHeight").as<int>();
    }
};