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
	
	raylib::RenderTexture2D target(GetScreenWidth(), GetScreenHeight());
	OverlayTexture crtEffect("shaders/crt.fs");
	
	Program Galaga;
	FontManager::Load();
	ImageManager::Load();
	SoundManager::Load();

	if (!IsWindowReady()) {
    TraceLog(LOG_ERROR, "Window failed to initialize!");
    return 1;
}

	while (!WindowShouldClose()) {
		Galaga.Update();

		target.BeginMode();
			ClearBackground(BLACK);
			Galaga.Draw();
		target.EndMode();

		BeginDrawing();
		//ClearBackground(WHITE);
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
