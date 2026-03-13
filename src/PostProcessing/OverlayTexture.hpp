#include "raylib-cpp/raylib-cpp.hpp"
#include <filesystem>

#define GLSL_VERSION 330

class OverlayTexture {
    private:
        raylib::Color colorOver = raylib::Color(0, 0, 0, 0);
        Shader curShader;

    public:
        void Draw(raylib::RenderTexture2D &target) {
            // Render generated texture using selected postprocessing shader
            BeginShaderMode(curShader);
                // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
                target.GetTexture().Draw(raylib::Rectangle( 0, 0, (float)target.texture.width, (float)-target.texture.height ), raylib::Vector2(0,0), WHITE);
            EndShaderMode();

        }

        OverlayTexture(const char *shaderPath) {
            if (std::filesystem::exists(shaderPath)) 
                curShader = LoadShader(0, shaderPath);
            else 
                throw 67;

            int iResolutionLoc = GetShaderLocation(this->curShader, "iResolution");
            float widthHeight[] = {GetScreenWidth(), GetScreenHeight()};

            int kBgColorLoc = GetShaderLocation(this->curShader, "kBackgroundColor");
            int backgroundColor[] = {0, 0, 0};

            SetShaderValue(this->curShader, iResolutionLoc, &widthHeight, SHADER_UNIFORM_VEC2);
            SetShaderValue(this->curShader, kBgColorLoc, &backgroundColor, SHADER_UNIFORM_VEC3);
        }
};