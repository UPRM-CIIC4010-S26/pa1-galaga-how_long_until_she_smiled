#pragma once

#include <iostream>
#include <string>
#include "raylib-cpp/raylib-cpp.hpp"
#include "Background.hpp"
#include "Player.hpp"
#include "Score.hpp"
#include "StdEnemy.hpp"
#include "SpEnemy.hpp"
#include "StEnemy.hpp"
#include "DyEnemy.hpp"

#define RETRO_FONT "resources/fonts/PressStart2P-vaV7.ttf"
class Program {
    private:
        Background background = Background();
        Player* player = new Player((GetScreenWidth() / 2) - 15, GetScreenHeight() * 0.75f);
        ScoreManager scoreManager;
        int respawnCooldown = 1080;
        int respawns = 0;
        int count = 0;
        int delay = 0;
        int lives = 3;
        int pauseFrames = 0;

        bool startup = true;
        bool paused = false;
        bool gameOver = false;

        Font pixelFontHead = LoadFontEx(RETRO_FONT, 40, 0, 0);
        Font pixelFontBody = LoadFontEx(RETRO_FONT, 30, 0, 0);

    public:
        Program();
        void StartingPositionEnemies();
        void Update();
        void Draw();
        void ManageEnemyRespawns();
        void DrawStartup();
        void DrawPauseScreen();
        void DrawGameOver();
        void KeyInputs();
        void PlayerReset();
        void Reset();
          
        ~Program() {}
};