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

#define DEFAULT_LIVES 3
#define MAX_LIVES 5

#define DEFAULT_COOLDOWN 1080

class Program {
    private:
        Background background = Background();
        Player* player = new Player((GetScreenWidth() / 2) - 15, GetScreenHeight() * 0.75f);
        ScoreManager scoreManager;
        int respawnCooldownReset = DEFAULT_COOLDOWN;
        int respawnCooldown = respawnCooldownReset;
        int respawns = 0;
        int count = 0;
        int delay = 0;
        int lives = DEFAULT_LIVES;
        int pauseFrames = 0;

        int progWave = 1; // The wave number that enemies follow

        bool startup = true;
        bool paused = false;
        bool gameOver = false;
        bool inGame = false;

    public:
        Program();
        void StartingPositionEnemies();
        void Update();
        void Draw();
        void NewWave(void); // Respawn enemies for each wave cleared
        void RespawnEnemy(void); // Now the standalone function for respawning a single enemy
        void ManageEnemyRespawns();
        void DrawCurrentLives(void); // NOT an overlay, part of the heads-up display
        void DrawDebugVariables(void); // ^
        void DrawStartup();
        void DrawPauseScreen();
        void DrawGameOver();
        void KeyInputs();
        void PlayerReset();
        void Reset();
          
        ~Program() {}
};