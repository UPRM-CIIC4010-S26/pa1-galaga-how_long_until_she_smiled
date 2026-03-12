#include "raylib-cpp/raylib-cpp.hpp"
#include <filesystem>

#define GLSL_VERSION 330

class OverlayTexture {
    private:
        raylib::Color colorOver = raylib::Color(0, 0, 0, 0);
        raylib::Shader curShader;

    public:
        void Draw(raylib::RenderTexture &target) {
            // Render generated texture using selected postprocessing shader
            this->curShader.BeginMode();
                // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
                target.GetTexture().Draw(raylib::Rectangle( 0, 0, (float)target.texture.width, (float)-target.texture.height ), raylib::Vector2(0,0), BLACK);
                //DrawTextureRec(target.texture, raylib::Rectangle( 0, 0, (float)target.texture.width, (float)-target.texture.height ), raylib::Vector2(0,0), WHITE);
            this->curShader.EndMode();
            
        }

        OverlayTexture(const char *texturePath) {
            if (std::filesystem::exists(texturePath)) 
                this->curShader.Load(texturePath, texturePath);
            else 
                throw 67;
        }
};