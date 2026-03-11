#include "raylib-cpp/raylib-cpp.hpp"

#define GLSL_VERSION 330

class OverlayTexture {
    private:
        raylib::Color colorOver = raylib::Color(0, 0, 0, 0);
        raylib::Shader curShader;

    public:
        void Draw(const raylib::RenderTexture &target) {
            // Render generated texture using selected postprocessing shader
            BeginShaderMode(this->curShader);
                // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
                DrawTextureRec(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2){ 0, 0 }, WHITE);
            EndShaderMode();

        }

        OverlayTexture(const char *texturePath) {
            this->curShader = raylib::Shader(texturePath, texturePath);
        }
};