#pragma once
#include "raylib-cpp/raylib-cpp.hpp"

#define RETRO_FONT "fonts/PressStart2P-vaV7.ttf"

class FontManager {
    public:
        inline static raylib::Font PixelFontHead;
        inline static raylib::Font PixelFontBody;

        static void Load() {
            PixelFontHead = raylib::Font(RETRO_FONT, 30);
            PixelFontBody = raylib::Font(RETRO_FONT, 20);
        }

        static void Unload() {
            PixelFontHead.Unload();
            PixelFontBody.Unload();
        }
};