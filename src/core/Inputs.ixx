module;

#include <set>
#include <string>
#include <vector>

#include "emscripten.h"

export module Inputs;

export namespace Inputs {
    float screenMouseX = 0.0f;
    float screenMouseY = 0.0f;
    bool isLeftMouseButtonDown = false;
    bool isRightMouseButtonDown = false;
    bool isMiddleMouseButtonDown = false;
    float mouseWheelDeltaX = 0.0f;
    float mouseWheelDeltaY = 0.0f;

    std::vector keydown(256, false);

    void init() {
        EM_ASM(({
            document.addEventListener("mousemove", e => {
                HEAPF32[$0 >> 2] = e.clientX;
                HEAPF32[$1 >> 2] = e.clientY;
            });

            document.addEventListener("mousedown", e => {
                switch (e.button) {
                    case 0: // Left mouse button
                        HEAPU8[$2 >> 0] = true;
                        break;
                    case 1: // Middle mouse button
                        HEAPU8[$4 >> 0] = true;
                        break;
                    case 2: // Right mouse button
                        HEAPU8[$3 >> 0] = true;
                        break;
                }
            });

            document.addEventListener("mouseup", e => {
                switch (e.button) {
                    case 0: // Left mouse button
                        HEAPU8[$2 >> 0] = false;
                        break;
                    case 1: // Middle mouse button
                        HEAPU8[$4 >> 0] = false;
                        break;
                    case 2: // Right mouse button
                        HEAPU8[$3 >> 0] = false;
                        break;
                }
            });

            document.addEventListener("wheel", e => {
                HEAPF32[$5 >> 2] = e.deltaX * -0.1;
                HEAPF32[$6 >> 2] = e.deltaY * -0.1;
            });

            document.addEventListener("keydown", e => {
                HEAPU8[$7 + e.keyCode] = true;
            });

            document.addEventListener("keyup", e => {
                HEAPU8[$7 + e.keyCode] = false;
            });
        }), &screenMouseX, &screenMouseY, &isLeftMouseButtonDown, &isRightMouseButtonDown, &isMiddleMouseButtonDown, &mouseWheelDeltaX, &mouseWheelDeltaY, &keydown);
    }

    void clear() {
        mouseWheelDeltaX = 0.0f;
        mouseWheelDeltaY = 0.0f;
    }
}



