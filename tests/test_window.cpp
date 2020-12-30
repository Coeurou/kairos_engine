#include <catch2/catch.hpp>

#include <core/types.h>
#include <window/window.h>

namespace kairos {

    struct dummy_big_window {
        float x = 0.f;
        float y = 0.f;
        int w = 0;
        int h = 0;
        uint32 id = 0;
        bool is_shown = false;
    };

    uint32 id(const dummy_big_window& w) { return w.id; }
    vec2i size(const dummy_big_window& w) { return vec2i(w.w, w.h); }
    void resize(dummy_big_window& w, const vec2i& size) {
        w.w = size.x;
        w.h = size.y;
    }
    void show(dummy_big_window& w) { w.is_shown = true; }
    bool is_shown(const dummy_big_window& w) { return w.is_shown; }
    void hide(dummy_big_window& w) { w.is_shown = false; }
    bool is_hidden(const dummy_big_window& w) { return !w.is_shown; }
    void destroy(dummy_big_window& w) {}

    struct dummy_small_window {
        dummy_big_window* flyweight;
    };

    uint32 id(const dummy_small_window& w) { return w.flyweight->id; }
    vec2i size(const dummy_small_window& w) { return vec2i(w.flyweight->w, w.flyweight->h); }
    void resize(dummy_small_window& w, const vec2i& size) {
        w.flyweight->w = size.x;
        w.flyweight->h = size.y;
    }
    void show(dummy_small_window& w) { w.flyweight->is_shown = true; }
    bool is_shown(const dummy_small_window& w) { return w.flyweight->is_shown; }
    void hide(dummy_small_window& w) { w.flyweight->is_shown = false; }
    bool is_hidden(const dummy_small_window& w) { return !w.flyweight->is_shown; }
    void destroy(dummy_small_window& w) {}
}

using namespace kairos;
namespace {
    dummy_big_window big{0.f, 0.f, 800, 600, 11, false};
    dummy_big_window data{0.f, 0.f, 800, 600, 10, false};
    dummy_small_window small{ &data };

    window small_window(small);
    window big_window(big);
}

TEST_CASE("window has an id", "[window]") {
    REQUIRE(id(small_window) == id(small));
    REQUIRE(id(big_window) == id(big));
}

TEST_CASE("window is copy constructible and move constructible", "[window]") {
    window copy(small_window);
    REQUIRE(id(small_window) == id(copy));
    REQUIRE(size(small_window) == size(copy));
    REQUIRE(is_shown(small_window) == is_shown(copy));
    REQUIRE(is_hidden(small_window) == is_hidden(copy));
}