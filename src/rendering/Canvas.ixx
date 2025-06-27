module;

#include <emscripten/val.h>
#include <utility>
#include <emscripten/em_asm.h>

#include "../../external/clay.h"

export module Canvas;

export struct Canvas2d {
    emscripten::val surface;

    static emscripten::val getSurface(const std::string &canvasId) {
        return emscripten::val::global("document").call<emscripten::val>("getElementById", canvasId);
    }

    static emscripten::val getSurface(const emscripten::val &context) {
        return context["canvas"];
    }

    explicit Canvas2d(emscripten::val surface) :  surface(std::move(surface)) {}

    [[nodiscard]] uint32_t getWidth() const {
        return surface["width"].as<uint32_t>();
    }

    [[nodiscard]] uint32_t getHeight() const {
        return surface["height"].as<uint32_t>();
    }

    void setSize(const uint32_t width, const uint32_t height) {
        surface.set("width", width);
        surface.set("height", height);
    }
};

export struct Context2d {
    emscripten::val ctx;
    const Canvas2d &canvas;

    explicit Context2d(const Canvas2d &canvas) : ctx(canvas.surface.call<emscripten::val>("getContext", emscripten::val("2d"))), canvas(canvas) {}

    // todo methods: arc, arcTo, bezierCurveTo, clearRect, clip, createConicGradient, createImageData, createLinearGradient
    // createPattern, createRadialGradient, drawFocusIfNeeded, drawImage, ellipse, fill, fillRect, fillText,
    // getContextAttributes, getImageData, getLineDash, getTransform, isContextLost, isPointInPath, isPointInStroke,
    // lineTo, measureText, moveTo, putImageData, quadraticCurveTo, reset, resetTransform, restore, rotate, roundRect,
    // save, setLineDash, setTransform, stroke, strokeRect, strokeText, transform, translate
    // todo accessors: direction, fillStyle, filter, font, font, globalAlpha, globalCompositeOperation,
    // imageSmoothingEnabled, imageSmoothingQuality, letterSpacing, lineCap, lineDashOffset, lineJoin,
    // lineWidth, miterLimit, shadowBlur, shadowColor, shadowOffsetX, shadowOffsetY, strokeStyle, textAlign, textBaseline,
    // wordSpacing

    void arc(const double x, const double y, const double radius, const double startAngle, const double endAngle, const bool counterclockwise = false) const {
        EM_ASM(({
            Emval.toValue($0).arc($1, $2, $3, $4, $5, !!$6);
        }), ctx.as_handle(), x, y, radius, startAngle, endAngle, counterclockwise);
    }

    void arcTo(const double x1, const double y1, const double x2, const double y2, const double radius) const {
        EM_ASM(({
            Emval.toValue($0).arcTo($1, $2, $3, $4, $5);
        }), ctx.as_handle(), x1, y1, x2, y2, radius);
    }

    void beginPath() const {
        EM_ASM(({
            Emval.toValue($0).beginPath();
        }), ctx.as_handle());
    }

    void closePath() const {
        EM_ASM(({
            Emval.toValue($0).closePath();
        }), ctx.as_handle());
    }

    void rect(const double x, const double y, const double width, const double height) const {
        EM_ASM(({
            Emval.toValue($0).rect($1, $2, $3, $4);
        }), ctx.as_handle(), x, y, width, height);
    }

    void bezierCurveTo(const double cp1x, const double cp1y, const double cp2x, const double cp2y, const double x, const double y) const {
        EM_ASM(({
            Emval.toValue($0).bezierCurveTo($1, $2, $3, $4, $5, $6);
        }), ctx.as_handle(), cp1x, cp1y, cp2x, cp2y, x, y);
    }

    void clearRect(const double x, const double y, const double width, const double height) const {
        EM_ASM(({
            Emval.toValue($0).clearRect($1, $2, $3, $4);
        }), ctx.as_handle(), x, y, width, height);
    }

    void clip() const {
        EM_ASM(({
            Emval.toValue($0).clip();
        }), ctx.as_handle());
    }

    void clip(const std::string& fillRule) const {
        EM_ASM(({
            Emval.toValue($0).clip(UTF8ToString($1));
        }), ctx.as_handle(), fillRule.c_str());
    }

    [[nodiscard]] emscripten::val createConicGradient(const double startAngle, const double x, const double y) const {
        return emscripten::val::take_ownership(static_cast<emscripten::EM_VAL>(EM_ASM_PTR(({
            return Emval.toHandle(Emval.toValue($0).createConicGradient($1, $2, $3));
            }), ctx.as_handle(), startAngle, x, y)));
    }

    [[nodiscard]] emscripten::val createImageData(const double width, const double height) const {
        return emscripten::val::take_ownership(static_cast<emscripten::EM_VAL>(EM_ASM_PTR(({
            return Emval.toHandle(Emval.toValue($0).createImageData($1, $2));
            }), ctx.as_handle(), width, height)));
    }

    [[nodiscard]] emscripten::val createImageData(const emscripten::val& imagedata) const {
        return emscripten::val::take_ownership(static_cast<emscripten::EM_VAL>(EM_ASM_PTR(({
            return Emval.toHandle(Emval.toValue($0).createImageData(Emval.toValue($1)));
            }), ctx.as_handle(), imagedata.as_handle())));
    }

    [[nodiscard]] emscripten::val createLinearGradient(const double x0, const double y0, const double x1, const double y1) const {
        return emscripten::val::take_ownership(static_cast<emscripten::EM_VAL>(EM_ASM_PTR(({
            return Emval.toHandle(Emval.toValue($0).createLinearGradient($1, $2, $3, $4));
            }), ctx.as_handle(), x0, y0, x1, y1)));
    }

    [[nodiscard]] emscripten::val createPattern(const emscripten::val& image, const std::string& repetition) const {
        return emscripten::val::take_ownership(static_cast<emscripten::EM_VAL>(EM_ASM_PTR(({
            return Emval.toHandle(Emval.toValue($0).createPattern(Emval.toValue($1), UTF8ToString($2)));
            }), ctx.as_handle(), image.as_handle(), repetition.c_str())));
    }

    [[nodiscard]] emscripten::val createRadialGradient(const double x0, const double y0, const double r0, const double x1, const double y1, const double r1) const {
        return emscripten::val::take_ownership(static_cast<emscripten::EM_VAL>(EM_ASM_PTR(({
            return Emval.toHandle(Emval.toValue($0).createRadialGradient($1, $2, $3, $4, $5, $6));
            }), ctx.as_handle(), x0, y0, r0, x1, y1, r1)));
    }

    void drawFocusIfNeeded(const emscripten::val& element) const {
        EM_ASM(({
            Emval.toValue($0).drawFocusIfNeeded(Emval.toValue($1));
        }), ctx.as_handle(), element.as_handle());
    }

    void drawImage(const emscripten::val& image, const double dx, const double dy) const {
        EM_ASM(({
            Emval.toValue($0).drawImage(Emval.toValue($1), $2, $3);
        }), ctx.as_handle(), image.as_handle(), dx, dy);
    }

    void drawImage(const emscripten::val& image, const double dx, const double dy, const double dWidth, const double dHeight) const {
        EM_ASM(({
            Emval.toValue($0).drawImage(Emval.toValue($1), $2, $3, $4, $5);
        }), ctx.as_handle(), image.as_handle(), dx, dy, dWidth, dHeight);
    }

    void drawImage(const emscripten::val& image, const double sx, const double sy, const double sWidth, const double sHeight, const double dx, const double dy, const double dWidth, const double dHeight) const {
        EM_ASM(({
            Emval.toValue($0).drawImage(Emval.toValue($1), $2, $3, $4, $5, $6, $7, $8, $9);
        }), ctx.as_handle(), image.as_handle(), sx, sy, sWidth, sHeight, dx, dy, dWidth, dHeight);
    }

    void ellipse(const double x, const double y, const double radiusX, const double radiusY, const double rotation, const double startAngle, const double endAngle, const bool counterclockwise = false) const {
        EM_ASM(({
            Emval.toValue($0).ellipse($1, $2, $3, $4, $5, $6, $7, !!$8);
        }), ctx.as_handle(), x, y, radiusX, radiusY, rotation, startAngle, endAngle, counterclockwise);
    }

    void fill() const {
        EM_ASM(({
            Emval.toValue($0).fill();
        }), ctx.as_handle());
    }

    void fill(const std::string& fillRule) const {
        EM_ASM(({
            Emval.toValue($0).fill(UTF8ToString($1));
        }), ctx.as_handle(), fillRule.c_str());
    }

    void fillRect(const double x, const double y, const double width, const double height) const {
        EM_ASM(({
            Emval.toValue($0).fillRect($1, $2, $3, $4);
        }), ctx.as_handle(), x, y, width, height);
    }

    void fillText(const std::string& text, const double x, const double y) const {
        EM_ASM(({
            Emval.toValue($0).fillText(UTF8ToString($1), $2, $3);
        }), ctx.as_handle(), text.c_str(), x, y);
    }

    void fillText(const std::string& text, const double x, const double y, const double maxWidth) const {
        EM_ASM(({
            Emval.toValue($0).fillText(UTF8ToString($1), $2, $3, $4);
        }), ctx.as_handle(), text.c_str(), x, y, maxWidth);
    }

    [[nodiscard]] emscripten::val getContextAttributes() const {
        return emscripten::val::take_ownership(static_cast<emscripten::EM_VAL>(EM_ASM_PTR(({
            return Emval.toHandle(Emval.toValue($0).getContextAttributes());
            }), ctx.as_handle())));
    }

    [[nodiscard]] emscripten::val getImageData(const double sx, const double sy, const double sw, const double sh) const {
        return emscripten::val::take_ownership(static_cast<emscripten::EM_VAL>(EM_ASM_PTR(({
            return Emval.toHandle(Emval.toValue($0).getImageData($1, $2, $3, $4));
            }), ctx.as_handle(), sx, sy, sw, sh)));
    }

    [[nodiscard]] std::vector<double> getLineDash() const {
        auto* result_ptr = reinterpret_cast<double*>(EM_ASM_INT({
            const js_array = Emval.toValue($0).getLineDash();
            const size = js_array.length;
            const buffer = _malloc((size + 1) * 8); // Allocate space for size + N doubles
            HEAPF64[buffer >> 3] = size;
            for (let i = 0; i < size; i++) {
                HEAPF64[(buffer >> 3) + 1 + i] = js_array[i];
            }
            return buffer;
        }, ctx.as_handle()));

        const int size = static_cast<int>(result_ptr[0]);
        std::vector<double> result;
        if (size > 0) {
            result.resize(size);
            std::copy_n(result_ptr + 1, size, result.begin());
        }
        free(result_ptr);
        return result;
    }

    [[nodiscard]] emscripten::val getTransform() const {
        return emscripten::val::take_ownership(static_cast<emscripten::EM_VAL>(EM_ASM_PTR(({
            return Emval.toHandle(Emval.toValue($0).getTransform());
            }), ctx.as_handle())));
    }

    [[nodiscard]] bool isContextLost() const {
        return EM_ASM_INT({
            return Emval.toValue($0).isContextLost();
        }, ctx.as_handle());
    }

    [[nodiscard]] bool isPointInPath(const double x, const double y) const {
        return EM_ASM_INT({
            return Emval.toValue($0).isPointInPath($1, $2);
        }, ctx.as_handle(), x, y);
    }

    [[nodiscard]] bool isPointInPath(const double x, const double y, const std::string& fillRule) const {
        return EM_ASM_INT({
            return Emval.toValue($0).isPointInPath($1, $2, UTF8ToString($3));
        }, ctx.as_handle(), x, y, fillRule.c_str());
    }

    [[nodiscard]] bool isPointInStroke(const double x, const double y) const {
        return EM_ASM_INT({
            return Emval.toValue($0).isPointInStroke($1, $2);
        }, ctx.as_handle(), x, y);
    }

    void lineTo(const double x, const double y) const {
        EM_ASM(({
            Emval.toValue($0).lineTo($1, $2);
        }), ctx.as_handle(), x, y);
    }

    [[nodiscard]] emscripten::val measureText(const std::string& text) const {
        return emscripten::val::take_ownership(static_cast<emscripten::EM_VAL>(EM_ASM_PTR(({
            return Emval.toHandle(Emval.toValue($0).measureText(UTF8ToString($1)));
            }), ctx.as_handle(), text.c_str())));
    }

    void moveTo(const double x, const double y) const {
        EM_ASM(({
            Emval.toValue($0).moveTo($1, $2);
        }), ctx.as_handle(), x, y);
    }

    void putImageData(const emscripten::val& imageData, const double dx, const double dy) const {
        EM_ASM(({
            Emval.toValue($0).putImageData(Emval.toValue($1), $2, $3);
        }), ctx.as_handle(), imageData.as_handle(), dx, dy);
    }

    void quadraticCurveTo(const double cpx, const double cpy, const double x, const double y) const {
        EM_ASM(({
            Emval.toValue($0).quadraticCurveTo($1, $2, $3, $4);
        }), ctx.as_handle(), cpx, cpy, x, y);
    }

    void reset() const {
        EM_ASM(({
            if (Emval.toValue($0).reset) { // Not supported in all browsers
                Emval.toValue($0).reset();
            }
        }), ctx.as_handle());
    }

    // resetTransform is deprecated, use setTransform(1,0,0,1,0,0) instead. Included for completeness.
    void resetTransform() const {
        EM_ASM(({
            Emval.toValue($0).resetTransform();
        }), ctx.as_handle());
    }

    void restore() const {
        EM_ASM(({
            Emval.toValue($0).restore();
        }), ctx.as_handle());
    }

    void rotate(const double angle) const {
        EM_ASM(({
            Emval.toValue($0).rotate($1);
        }), ctx.as_handle(), angle);
    }

    void roundRect(const double x, const double y, const double w, const double h, const double radius) const {
        EM_ASM(({
            Emval.toValue($0).roundRect($1, $2, $3, $4, $5);
        }), ctx.as_handle(), x, y, w, h, radius);
    }

    void roundRect(const double x, const double y, const double w, const double h, const std::vector<double>& radii) const {
        EM_ASM_({
            const radii_ptr = $5;
            const radii_size = $6;
            const js_array = [];
            for (let i = 0; i < radii_size; i++) {
                js_array.push(HEAPF64[(radii_ptr>>3) + i]);
            }
            Emval.toValue($0).roundRect($1, $2, $3, $4, js_array);
        }, ctx.as_handle(), x, y, w, h, radii.data(), radii.size());
    }

    void save() const {
        EM_ASM(({
            Emval.toValue($0).save();
        }), ctx.as_handle());
    }

    void setLineDash(const std::vector<double>& segments) const {
        EM_ASM_({
            const segments_ptr = $1;
            const segments_size = $2;
            const js_array = [];
            for (let i = 0; i < segments_size; i++) {
                js_array.push(HEAPF64[(segments_ptr>>3) + i]);
            }
            Emval.toValue($0).setLineDash(js_array);
        }, ctx.as_handle(), segments.data(), segments.size());
    }

    void setTransform(const double a, const double b, const double c, const double d, const double e, const double f) const {
        EM_ASM(({
            Emval.toValue($0).setTransform($1, $2, $3, $4, $5, $6);
        }), ctx.as_handle(), a, b, c, d, e, f);
    }

    void setTransform(const emscripten::val& matrix) const {
        EM_ASM(({
            Emval.toValue($0).setTransform(Emval.toValue($1));
        }), ctx.as_handle(), matrix.as_handle());
    }

    void stroke() const {
        EM_ASM(({
            Emval.toValue($0).stroke();
        }), ctx.as_handle());
    }

    void strokeRect(const double x, const double y, const double width, const double height) const {
        EM_ASM(({
            Emval.toValue($0).strokeRect($1, $2, $3, $4);
        }), ctx.as_handle(), x, y, width, height);
    }

    void strokeText(const std::string& text, const double x, const double y) const {
        EM_ASM(({
            Emval.toValue($0).strokeText(UTF8ToString($1), $2, $3);
        }), ctx.as_handle(), text.c_str(), x, y);
    }

    void strokeText(const std::string& text, const double x, const double y, const double maxWidth) const {
        EM_ASM(({
            Emval.toValue($0).strokeText(UTF8ToString($1), $2, $3, $4);
        }), ctx.as_handle(), text.c_str(), x, y, maxWidth);
    }

    void transform(const double a, const double b, const double c, const double d, const double e, const double f) const {
        EM_ASM(({
            Emval.toValue($0).transform($1, $2, $3, $4, $5, $6);
        }), ctx.as_handle(), a, b, c, d, e, f);
    }

    void translate(const double x, const double y) const {
        EM_ASM(({
            Emval.toValue($0).translate($1, $2);
        }), ctx.as_handle(), x, y);
    }

    [[nodiscard]] std::string getDirection() const { return getStringProperty("direction"); }
    void setDirection(const std::string& value) const { EM_ASM(({ Emval.toValue($0).direction = UTF8ToString($1); }), ctx.as_handle(), value.c_str()); }

    [[nodiscard]] std::string getFillStyleStr() const { return getStringProperty("fillStyle"); }
    void setFillStyle(const std::string& value) const { EM_ASM(({ Emval.toValue($0).fillStyle = UTF8ToString($1); }), ctx.as_handle(), value.c_str()); }
    void setFillStyle(const emscripten::val& value) const { EM_ASM(({ Emval.toValue($0).fillStyle = Emval.toValue($1); }), ctx.as_handle(), value.as_handle()); }

    [[nodiscard]] std::string getFilter() const { return getStringProperty("filter"); }
    void setFilter(const std::string& value) const { EM_ASM(({ Emval.toValue($0).filter = UTF8ToString($1); }), ctx.as_handle(), value.c_str()); }

    [[nodiscard]] std::string getFont() const { return getStringProperty("font"); }
    void setFont(const std::string& value) const { EM_ASM(({ Emval.toValue($0).font = UTF8ToString($1); }), ctx.as_handle(), value.c_str()); }

    [[nodiscard]] double getGlobalAlpha() const { return EM_ASM_DOUBLE({ return Emval.toValue($0).globalAlpha; }, ctx.as_handle()); }
    void setGlobalAlpha(const double value) const { EM_ASM(({ Emval.toValue($0).globalAlpha = $1; }), ctx.as_handle(), value); }

    [[nodiscard]] std::string getGlobalCompositeOperation() const { return getStringProperty("globalCompositeOperation"); }
    void setGlobalCompositeOperation(const std::string& value) const { EM_ASM(({ Emval.toValue($0).globalCompositeOperation = UTF8ToString($1); }), ctx.as_handle(), value.c_str()); }

    [[nodiscard]] bool getImageSmoothingEnabled() const { return EM_ASM_INT({ return Emval.toValue($0).imageSmoothingEnabled; }, ctx.as_handle()); }
    void setImageSmoothingEnabled(const bool value) const { EM_ASM(({ Emval.toValue($0).imageSmoothingEnabled = !!$1; }), ctx.as_handle(), value); }

    [[nodiscard]] std::string getImageSmoothingQuality() const { return getStringProperty("imageSmoothingQuality"); }
    void setImageSmoothingQuality(const std::string& value) const { EM_ASM(({ Emval.toValue($0).imageSmoothingQuality = UTF8ToString($1); }), ctx.as_handle(), value.c_str()); }

    [[nodiscard]] std::string getLetterSpacing() const { return getStringProperty("letterSpacing"); }
    void setLetterSpacing(const std::string& value) const { EM_ASM(({ Emval.toValue($0).letterSpacing = UTF8ToString($1); }), ctx.as_handle(), value.c_str()); }

    [[nodiscard]] std::string getLineCap() const { return getStringProperty("lineCap"); }
    void setLineCap(const std::string& value) const { EM_ASM(({ Emval.toValue($0).lineCap = UTF8ToString($1); }), ctx.as_handle(), value.c_str()); }

    [[nodiscard]] double getLineDashOffset() const { return EM_ASM_DOUBLE({ return Emval.toValue($0).lineDashOffset; }, ctx.as_handle()); }
    void setLineDashOffset(const double value) const { EM_ASM(({ Emval.toValue($0).lineDashOffset = $1; }), ctx.as_handle(), value); }

    [[nodiscard]] std::string getLineJoin() const { return getStringProperty("lineJoin"); }
    void setLineJoin(const std::string& value) const { EM_ASM(({ Emval.toValue($0).lineJoin = UTF8ToString($1); }), ctx.as_handle(), value.c_str()); }

    [[nodiscard]] double getLineWidth() const { return EM_ASM_DOUBLE({ return Emval.toValue($0).lineWidth; }, ctx.as_handle()); }
    void setLineWidth(const double value) const { EM_ASM(({ Emval.toValue($0).lineWidth = $1; }), ctx.as_handle(), value); }

    [[nodiscard]] double getMiterLimit() const { return EM_ASM_DOUBLE({ return Emval.toValue($0).miterLimit; }, ctx.as_handle()); }
    void setMiterLimit(const double value) const { EM_ASM(({ Emval.toValue($0).miterLimit = $1; }), ctx.as_handle(), value); }

    [[nodiscard]] double getShadowBlur() const { return EM_ASM_DOUBLE({ return Emval.toValue($0).shadowBlur; }, ctx.as_handle()); }
    void setShadowBlur(const double value) const { EM_ASM(({ Emval.toValue($0).shadowBlur = $1; }), ctx.as_handle(), value); }

    [[nodiscard]] std::string getShadowColor() const { return getStringProperty("shadowColor"); }
    void setShadowColor(const std::string& value) const { EM_ASM(({ Emval.toValue($0).shadowColor = UTF8ToString($1); }), ctx.as_handle(), value.c_str()); }

    [[nodiscard]] double getShadowOffsetX() const { return EM_ASM_DOUBLE({ return Emval.toValue($0).shadowOffsetX; }, ctx.as_handle()); }
    void setShadowOffsetX(const double value) const { EM_ASM(({ Emval.toValue($0).shadowOffsetX = $1; }), ctx.as_handle(), value); }

    [[nodiscard]] double getShadowOffsetY() const { return EM_ASM_DOUBLE({ return Emval.toValue($0).shadowOffsetY; }, ctx.as_handle()); }
    void setShadowOffsetY(const double value) const { EM_ASM(({ Emval.toValue($0).shadowOffsetY = $1; }), ctx.as_handle(), value); }

    [[nodiscard]] std::string getStrokeStyleStr() const { return getStringProperty("strokeStyle"); }
    void setStrokeStyle(const std::string& value) const { EM_ASM(({ Emval.toValue($0).strokeStyle = UTF8ToString($1); }), ctx.as_handle(), value.c_str()); }
    void setStrokeStyle(const emscripten::val& value) const { EM_ASM(({ Emval.toValue($0).strokeStyle = Emval.toValue($1); }), ctx.as_handle(), value.as_handle()); }

    [[nodiscard]] std::string getTextAlign() const { return getStringProperty("textAlign"); }
    void setTextAlign(const std::string& value) const { EM_ASM(({ Emval.toValue($0).textAlign = UTF8ToString($1); }), ctx.as_handle(), value.c_str()); }

    [[nodiscard]] std::string getTextBaseline() const { return getStringProperty("textBaseline"); }
    void setTextBaseline(const std::string& value) const { EM_ASM(({ Emval.toValue($0).textBaseline = UTF8ToString($1); }), ctx.as_handle(), value.c_str()); }

    [[nodiscard]] std::string getWordSpacing() const { return getStringProperty("wordSpacing"); }
    void setWordSpacing(const std::string& value) const { EM_ASM(({ Emval.toValue($0).wordSpacing = UTF8ToString($1); }), ctx.as_handle(), value.c_str()); }

private:
    // Helper for getting string properties without using val::as<string>
    [[nodiscard]] std::string getStringProperty(const char* propertyName) const {
        const auto *s = reinterpret_cast<const char*>(EM_ASM_INT({
            const propName = UTF8ToString($1);
            const text = Emval.toValue($0)[propName];
            if (typeof text !== 'string') return 0;
            const len = lengthBytesUTF8(text) + 1;
            const buffer = _malloc(len);
            stringToUTF8(text, buffer, len);
            return buffer;
        }, ctx.as_handle(), propertyName));
        if (!s) return "";
        std::string result(s);
        delete s;
        return result;
    }
};