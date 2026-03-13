#include "raylib.h"

#include "resource_dir.h"
#include "Program.hpp"
#include "PostProcessing/OverlayTexture.hpp"

int main ()
{
	SearchAndSetResourceDir("resources");
    InitAudioDevice();
	SetTargetFPS(60);

	raylib::Window window(1000, 1000, "Galaga", FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	Program Galaga;
	FontManager::Load();
	ImageManager::Load();
	SoundManager::Load();

	if (!IsWindowReady()) {
    TraceLog(LOG_ERROR, "Window failed to initialize!");
    return 1;
}

	raylib::RenderTexture2D target(GetRenderWidth(), GetRenderHeight());
	OverlayTexture crtEffect("shaders/crt.fs", (float)GetRenderWidth(), (float)GetRenderHeight());
	float scale = (float)GetRenderWidth() / (float)GetScreenWidth();

	while (!WindowShouldClose()) {
		printf("RenderTexture: %d x %d\n", target.texture.width, target.texture.height);
		printf("Screen: %d x %d\n", GetScreenWidth(), GetScreenHeight());
		Galaga.Update();

	target.BeginMode();
		ClearBackground(BLACK);
	
		rlPushMatrix();
			rlScalef(scale, scale, 1.0f);
			Galaga.Draw();
		rlPopMatrix();
		
	target.EndMode();

		BeginDrawing();
		ClearBackground(WHITE);
		crtEffect.Draw(target);		
		Galaga.KeyInputs();

		EndDrawing();
	}

	FontManager::Unload();
	ImageManager::Unload();
	SoundManager::Unload();
	CloseAudioDevice();
	window.Close();
	return 0;
}
