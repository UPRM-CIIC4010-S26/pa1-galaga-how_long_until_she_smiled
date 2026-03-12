#pragma once

#include <iostream>
#include <string>
#include "raylib-cpp/raylib-cpp.hpp"
#include "Background.hpp"
#include "Player.hpp"
#include "Score.hpp"
#include "Loop.hpp"
#include "StdEnemy.hpp"
#include "SpEnemy.hpp"
#include "StEnemy.hpp"
#include "DyEnemy.hpp"

#ifdef DEBUG
constexpr bool DEBUG_MODE = true;
#else
constexpr bool DEBUG_MODE = false;
#endif

#define DEFAULT_LIVES 3
#define MAX_LIVES 5

#define DEFAULT_COOLDOWN 1080

class Program {
    private:
        enum class stages{DEFAULT, SECOND};
        Background background = Background();
        Player* player = new Player((GetScreenWidth() / 2) - 15, GetScreenHeight() * 0.75f);
        Loop stageManager;
        ScoreManager scoreManager;
        int respawnCooldownReset = DEFAULT_COOLDOWN;
        int respawnCooldown = respawnCooldownReset;
        int respawns = 0;
        int count = 0;
        int delay = 0;
        int lives = DEFAULT_LIVES;
        int pauseFrames = 0;

        int progWave = 1; // The wave number that enemies follow
        
        float bootTimer = 0.0f;
        int bootFrameCounter = 0;


        bool stageTransition = false;
        bool startup = false;
        bool paused = false;
        bool gameOver = false;
        bool inGame = false;
        bool isBooting = true;
        bool stagesActive = false;
        bool stageCompleted = false;


        //BOOTUP VARIABLES
        struct Cell { // Creating structures for each cell for the 2D List each list
            bool isLetter;   
            char letter;     
            Color color;    
        };
        std::vector<std::vector<Cell>> bootGrid;/*There is no other way to do a "Garbage CRT RAM" effect without a 2D List having all the values of the letter
                                                and having basically roating. And if there are, is too dificult to implement without changing a 
                                                SIGNIFICANT part of the code base*/


    public:
        Program();
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
        void NextStage();
        void Reset(bool next_stage=false);

        //BOOTUP FUNCTIONS
        void DrawBootup();
        void InitializeGrid(); //Intializes the grid of colors of RANDOM colors or letters
        void UpdateGrid(); // Shift downs to look like rolling shutter
          
        ~Program() {}
};