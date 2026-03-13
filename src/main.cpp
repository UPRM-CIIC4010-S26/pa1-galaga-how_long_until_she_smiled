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
	float scale = (float)GetRenderWidth() / (float)GetScreenWidth();//(1)

	/*
	So because we arent using RayLib normal shader, that means that GetScreenHeight() or GetScreenWidth() will give us the logical correct dimensions but not 
	the actual render dimensions (if for example a iMac M1 with a 4k screen will not draw it correctly and actually, will draw like 2000x2000 bc of the highdp 
	setting so it will be in the half) we need to get the render dimensions of what are we drawing and scale it up to the screen that we want (1000x1000).
	*/

	while (!WindowShouldClose()) {
		Galaga.Update();

	target.BeginMode();
		ClearBackground(BLACK);

		/*
		As we said previously, we need to scale it. We first calculate the scale by (float) GetRender___()/(float)GetScreen___() (___ being height or width (we
		chosed Width))(1). Then, we take the State of OpenGl (or the Transforming Matrix) and take to memory specificly the Matrix Stack (2). After we scale the matrix (3),
		we draw with every x and y scaled (4), and finally get rid of the matrix that we took the Matrix Stack(5). (as u can see this whole shaders stuff was painful)
		*/
	
		rlPushMatrix();//(2)
			rlScalef(scale, scale, 1.0f); //(3)
			Galaga.Draw();//(4)
		rlPopMatrix();//(5)
		
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
