#include "Program.hpp"

/* DEPRECATED OFFSETS
#define RATIONAL_OFFSET 25
#define EXPONENTIAL_OFFSET 1000
#define MIN_VALUE 500
*/

Program::Program() {
    InitializeGrid();
    Background::sideWalls = std::pair<HitBox, HitBox>{ 
        HitBox(0, 0, 10, GetScreenHeight()), 
        HitBox(GetScreenWidth() - 10, 0, 10, GetScreenHeight())
    };

    //Aligns the enemies in the starting fase
    stageManager.StartingPositionEnemies();
}

void Program::Update() {
    if (isBooting) {
        bootTimer += GetFrameTime();

        if (bootTimer >= 10.0f) {
            isBooting = false;
            startup = true;
        }
        return;
    }

    for (Animation& a : Animation::animations) a.update();
    for (int i = 0; i < Animation::animations.size(); i++) {
        if (Animation::animations[i].done) Animation::animations.erase(Animation::animations.begin() + i);
    }
    pauseFrames = std::max(pauseFrames - 1, 0);

    if (pauseFrames <= 0) {
        stageTransition = false;
    }

    if (!startup && !paused && !gameOver && pauseFrames <= 0) {

        Enemy::ManageEnemies(player->hitBox, scoreManager, lives);
        StdEnemy::attackReset();
        ManageEnemyRespawns();
        player->update();

        for (std::pair<std::pair<float, float>, Enemy*> p : Enemy::enemies) {
            if (p.second && HitBox::Collision(player->hitBox, p.second->hitBox)) {
                Animation::animations.push_back(
                    Animation(player->position.first, player->position.second, 16, 0, 33, 34, 30 ,30, 3, ImageManager::SpriteSheet)
                );

                PlaySound(SoundManager::gameOver);
                Projectile::projectiles.clear();
                player->position.first = GetScreenWidth() / 2 - 15;
                p.second->health = 0;
                pauseFrames = 120;
                lives--;
            }
        }
        
        //Collision detection, checks if a bullet that isnt the player hits the player, and if, reset the player.
        for (Projectile& p : Projectile::projectiles) { 
            if (HitBox::Collision(player->hitBox,p.getHitBox()) && p.ID){
                PlayerReset();
            };
            p.update(); 

        }

        if (lives <= 0 && pauseFrames <= 0) gameOver = true;
        Projectile::CleanProjectiles();
        Projectile::ProjectileCollision();


        if (stagesActive && progWave >= 3 && Enemy::aliveEnemies == 0 && !stageTransition) {
            stageTransition = true;
            progWave = 1;
            Reset(true);
        }
    }
}

void Program::Draw() {
    if (isBooting){
        UpdateGrid();
        DrawBootup();
        return;
    }

    // Background
    background.Draw();

    // Entities
    if (pauseFrames <= 0 && !gameOver) player->draw();
    for (Animation& a : Animation::animations) a.draw();

    for (int i = 0; i < lives; i++) {
         DrawTexturePro(ImageManager::SpriteSheet, Rectangle{0, 0, 17, 18}, 
                   Rectangle{10.0f + i * 30, GetScreenHeight() - 30.0f, 20, 20}, 
                   Vector2{0, 0}, 0, WHITE);
    }


    for (Projectile p : Projectile::projectiles) p.draw();
    for (std::pair<std::pair<float, float>, Enemy*>& p : Enemy::enemies) if (p.second) p.second->draw();


    if (pauseFrames > 0 && stageTransition) {
        if (Loop::stagesActive) DrawText(TextFormat("STAGE %01i", stageManager.getStage()+1), GetScreenWidth()/2 - 100, GetScreenHeight()/2, 30, RED);
        else DrawText("STAGE NaN", GetScreenWidth()/2 - 100, GetScreenHeight()/2, 30, RED);
    }

    // HUD
    scoreManager.draw({20, 20}, progWave);
    this->DrawCurrentLives();

    // Debug HUD
    if (DEBUG_MODE) this->DrawDebugVariables();

    // Overlays
    if (startup) DrawStartup();
    if (paused) DrawPauseScreen();
    if (gameOver) DrawGameOver();
}

// Respawn 20 enemies all at once
void Program::NewWave(void) {
    this->respawnCooldownReset = DEFAULT_COOLDOWN;
    this->respawnCooldown = this->respawnCooldownReset;

    for (int i = 0; i < 20; i++) {
        RespawnEnemy();
    }
}

// Respawn enemy in position
void Program::RespawnEnemy(void) {
    for (std::pair<std::pair<float, float>, Enemy*>& p : Enemy::enemies) {
        if (!p.second && p.first.second != 150) {
            int eType = GetRandomValue(1, 3); //eType = enemyType

            if (eType == 1) {
                p.second = new StEnemy(GetScreenWidth() / 2 - 15, 0, true);
                respawnCooldown /= 2;
            } else {
                p.second = new StdEnemy(GetScreenWidth() / 2 - 15, 0, true);
            }

            Enemy::aliveEnemies++;
            respawns++;
            break;
        } else if (!p.second && p.first.second == 150) {
            p.second = new SpEnemy(GetScreenWidth() / 2 - 15, 0, true);
            respawns++;
            Enemy::aliveEnemies++;
            break;
        }
    }
}

void Program::ManageEnemyRespawns() {
    delay = std::max(delay - 1, 0);

    respawnCooldown -= 1;
    if (respawnCooldown <= 0) {
        if (progWave < scoreManager.getWave()) {
            for (int i = 0; i < scoreManager.getWave() - progWave; i++) {
                if (stagesActive){
                    if (progWave < 3){
                        NewWave();
                        progWave++;
                    }
                }else{
                    NewWave();
                    progWave++;
                }
            }
        } else {
                int offsetScore = scoreManager.getScore() - (scoreManager.getWaveRate() * (scoreManager.getWave() - 1));
                respawnCooldownReset = DEFAULT_COOLDOWN * std::exp(-std::pow(offsetScore, 2) / 3000000);
                                                /* Original equation: ((DEFAULT_COOLDOWN - MIN_VALUE) * RATIONAL_OFFSET)   \
                                                            / (std::exp(scoreDistance / EXPONENTIAL_OFFSET) + RATIONAL_OFFSET) \
                                                                                                                            + MIN_VALUE;*/
                respawnCooldown = respawnCooldownReset;

                if (stagesActive){
                    if (progWave < 3){
                        RespawnEnemy();
                    }
                }else RespawnEnemy();
            
        }
    }

    if(respawns >= 4) {
        count = 4;
        respawns = 0;
    }

    if (count > 0 && delay <= 0) {
        Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
            std::pair<float, float>{0, 0}, 
            new DyEnemy(GetScreenWidth(), 300)
        });
        Enemy::aliveEnemies++; 
        count--;
        delay = 20;
    }
}

// Who invited DrawCurrentLives to the overlay draw calls 😂😂😂 bro thinks he's on the team 💀💀
void Program::DrawCurrentLives(void) {
    int baseSize = FontManager::PixelFontBody.GetBaseSize();
    FontManager::PixelFontBody.DrawText(TextFormat("%01i-UP", this->lives), {150, 20}, baseSize, 0.1, RED);
}
void Program::DrawDebugVariables(void) {
    int baseSize = FontManager::PixelFontBody.GetBaseSize();
    FontManager::PixelFontBody.DrawText(TextFormat("Alive: %d", Enemy::aliveEnemies),{10, 200}, baseSize, 0.1 , RED);
    FontManager::PixelFontBody.DrawText(TextFormat("Respawn cooldown reset: %01i", this->respawnCooldownReset), {150, 400}, baseSize, 0.1, RED);
    FontManager::PixelFontBody.DrawText(TextFormat("Respawn cooldown: %01i", this->respawnCooldown), {150, 500}, baseSize, 0.1, RED);
}

void Program::InitializeGrid() {
        bootGrid.resize(20); //Creates 20 lists empty in the outside list ( called y)
        for (int y = 0; y < 20; y++){
            bootGrid[y].resize(40); //Creates 40 empty list inside each of those 20 lists ( called x)
            for (int x = 0; x < 40; x++) {
                bool chanceOfLetter = GetRandomValue(0, 100) < 30; // If the random value is less than 30, it will be a letter, else is a block of color
                bootGrid[y][x].isLetter = chanceOfLetter; //the var in the struc cell will be now that bool that we created thtat said if its a letter
                if (chanceOfLetter) bootGrid[y][x].letter = 'A' + GetRandomValue(0,25); //if we got letter, then we have to asigned a random letter
                            bootGrid[y][x].color = {(unsigned char)GetRandomValue(50,255), // for everything random colors
                                                    (unsigned char)GetRandomValue(50,255),
                                                    (unsigned char)GetRandomValue(50,255),
                                                    255 };
            };
        };
    }


void Program::UpdateGrid(){ 
        for (int y = 19; y > 0; y--) { //So basically what this does is put everything down a row, so we can a CRT Effect
            bootGrid[y] = bootGrid[y-1]; 
        }

        for (int x = 0; x < 40; x++) { //Generates the first row
        //Basically repeats the same code that we did when Initializing the Grid
        bool letter = GetRandomValue(0, 100) < 30; 
        bootGrid[0][x].isLetter = letter;
        if (letter) bootGrid[0][x].letter = 'A' + GetRandomValue(0,25);
        bootGrid[0][x].color = { (unsigned char)GetRandomValue(50,255),
                                 (unsigned char)GetRandomValue(50,255),
                                 (unsigned char)GetRandomValue(50,255),
                                 255 };
    }
    }

void Program::DrawBootup() {
    //Debuging
    if(DEBUG_MODE)DrawText(TextFormat("BOOT TIMER: %.2f", bootTimer), 10, 10, 20, RAYWHITE);

    int centerX = GetScreenWidth() / 2 - 80;
    int centerY = GetScreenHeight() / 2;

    //if we are between the 0 and 5 in the boot timer, then start the "cleaning the memory" (check galaga arcade bootup)
    if (bootTimer > 0.0f && bootTimer < 5.0f) {
        int cellWidth  = GetScreenWidth() / 40; 
        int cellHeight = GetScreenHeight() / 20;

        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 40; x++) {
                Cell &c = bootGrid[y][x]; //instead of copying the cell, we are just rawdogging and taking the actual values for letters (technically more efficient) 

                float px = x * cellWidth;
                float py = y * cellHeight;

                if (c.isLetter) {
                    DrawText(&c.letter, px, py, cellHeight, c.color); 
                } else {
                    DrawRectangle(px, py, cellWidth, cellHeight, c.color); 
                }
                }
            }
        }

    if (bootTimer > 5.0f && bootTimer < 10.0f) {
        DrawText("MEMORY CLEANED", centerX, centerY - 60, 20, GREEN);

        if(bootTimer > 7.5){
        DrawText("RAM OK", centerX, centerY - 20, 20, GREEN);
        }
    
        if (bootTimer > 8.5f) {
            DrawText("ROM OK", centerX, centerY + 20, 20, GREEN);
        }
    }
    
}

void Program::DrawStartup() {
    DrawRectangle(0, 0, (float)GetScreenWidth(), (float)GetScreenHeight(), Color{0, 0, 0, 125});
    DrawText("Galaga", (GetScreenWidth() / 2 - 237), 75, 144, WHITE);
    DrawText("Press Enter", (GetScreenWidth() / 2) - 75, GetScreenHeight() / 2, 24, GRAY);

    float t = GetTime();
    bool flash = ((int)(t * 2) % 2) == 0; 
    Color textColor = flash ? YELLOW : GRAY;
    if (!stagesActive){
        DrawText("Endless", (GetScreenWidth() / 2) - 75, (GetScreenHeight() / 2)+40, 24, textColor);
        DrawText("Stages", (GetScreenWidth() / 2) - 75, (GetScreenHeight() / 2)+80, 24, GRAY);
    }else{
        DrawText("Endless", (GetScreenWidth() / 2) - 75, (GetScreenHeight() / 2)+40, 24, GRAY);
        DrawText("Stages", (GetScreenWidth() / 2) - 75, (GetScreenHeight() / 2)+80, 24, textColor);
    }
}

void Program::DrawPauseScreen() {
    DrawRectangle(0, 0, (float)GetScreenWidth(), (float)GetScreenHeight(), Color{0, 0, 0, 125});
    DrawText("Paused", (GetScreenWidth() / 2) - 85, GetScreenHeight() / 2 - 60, 48, WHITE);
    DrawText("Press Enter", (GetScreenWidth() / 2) - 75, GetScreenHeight() / 2, 24, GRAY);
}

void Program::DrawGameOver() {
    DrawRectangle(0, 0, (float)GetScreenWidth(), (float)GetScreenHeight(), Color{0, 0, 0, 125});
    DrawText("Game Over", (GetScreenWidth() / 2) - 380, 50, 144, WHITE);
    DrawText("Press Enter", (GetScreenWidth() / 2) - 75, GetScreenHeight() / 2, 24, GRAY);
}

void Program::KeyInputs() {
    if ((!gameOver && !startup && IsKeyPressed('P')) || (paused && IsKeyPressed(KEY_ENTER))) paused = !paused;
    if (!paused && !startup && IsKeyPressed('O')) gameOver = !gameOver;
    if (!gameOver && !paused && IsKeyPressed('I')) startup = !startup;
    if (IsKeyPressed('H')) HitBox::drawHitbox = !HitBox::drawHitbox;

    // Custom debug function below, potentially comment out on release build
    if (IsKeyPressed('K')) {
        this->lives += scoreManager.addToScore(500, 1000);
        if (this->lives > 5) {
            this->lives = 5;
        }
    }
    
    if (gameOver && IsKeyPressed(KEY_ENTER)) {
        gameOver = false;
        Reset();
    }

    if (startup && (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN))) stagesActive = !stagesActive;

    if (startup && IsKeyPressed(KEY_ENTER)) {
        startup = false;
        if (!inGame) {
            stageManager.setStages(stagesActive);
            pauseFrames = 480;            
            stageTransition = true;
            PlaySound(SoundManager::theme);
            inGame = true; 
        }
    }

    if (!startup && !paused && !gameOver && pauseFrames <= 0) player->keyInputs();
   
}

void Program::PlayerReset() {
    Animation::animations.push_back(
        Animation(player->position.first, player->position.second, 16, 0, 33, 34, 30 ,30, 3, ImageManager::SpriteSheet)
    );


    PlaySound(SoundManager::gameOver);
    Projectile::projectiles.clear();
    player->position.first = GetScreenWidth() / 2 - 15;
    pauseFrames = 120;
    lives--;
}

void Program::Reset(bool next_stage) {
    Enemy::enemies.clear();
    Enemy::aliveEnemies = 0;
    StdEnemy::attackInProgress = false;
    player = new Player((GetScreenWidth() / 2) - 15, GetScreenHeight() * 0.75f);
    if (Loop::stagesActive){
        if (!next_stage){
            scoreManager.resetScore();
            lives = 3;
            stageManager.resetStage();
        }
    }
    else{
        scoreManager.resetScore();
        lives = 3;}
    stageManager.reset(next_stage);
    respawnCooldown = respawnCooldownReset;
    respawns = 0;
    count = 0;
    delay = 0;
    stageTransition = true;
    pauseFrames = 480;
    PlaySound(SoundManager::theme );
    inGame = true;
}