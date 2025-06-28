module;

#include <string>
#include <utility>
#include <emscripten/em_asm.h>
#include <emscripten/val.h>

export module Image;

export struct Image {
    emscripten::val element;
    const std::string src;

    explicit Image(std::string src) : src(std::move(src)) {
        element = emscripten::val::global("Image").call<emscripten::val>("new", emscripten::val(src));
        EM_ASM(({
            const element = Emval.toValue($0);
            element.onload = () => { element.loaded = true; };
        }), element.as_handle());
    }

    bool isLoaded() {
        if (_loaded) return true;
        const int loaded = EM_ASM_INT(({
            return Emval.toValue($0).loaded;
        }), element.as_handle());
        if (!loaded) return false;
        _loaded = true;
        return true;
    }

private:
    bool _loaded = false;
};
