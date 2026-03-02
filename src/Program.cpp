#include "Program.hpp"

#define RATIONAL_OFFSET 25
#define EXPONENTIAL_OFFSET 1000
#define MIN_VALUE 500

void Program::StartingPositionEnemies(){
    /*
    AddingEnemies() makes sure to add all the enemies to the vector, with the starting location.
    */
    //Sp Enemy adding (the two enemies at the top)------------------------
    Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
        std::pair<float, float>{350, 150}, 
        new SpEnemy(350, 150)
    });

    Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
        std::pair<float, float>{600, 150}, 
        new SpEnemy(600, 150)
    });
    //--------------------------------------------------------------------

    //Rest of the enemies-----------------------------------------------------
    for (int i = 0; i < 30; i++) {
        float x = 250 + 50 * (i%10);
        float y = 200 + 50 * (i/10);

        Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
            std::pair<float, float>{x, y}, 
            new StdEnemy(x, y)
        });
    //-------------------------------------------------------------------------
}};

Program::Program() {
    Background::sideWalls = std::pair<HitBox, HitBox>{ 
        HitBox(0, 0, 10, GetScreenHeight()), 
        HitBox(GetScreenWidth() - 10, 0, 10, GetScreenHeight())
    };

    //Aligns the enemies in the starting fase
    StartingPositionEnemies();
}

void Program::Update() {
    for (Animation& a : Animation::animations) a.update();
    for (int i = 0; i < Animation::animations.size(); i++) {
        if (Animation::animations[i].done) Animation::animations.erase(Animation::animations.begin() + i);
    }
    pauseFrames = std::max(pauseFrames - 1, 0);

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
    }
}

void Program::Draw() {
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

    // HUD
    scoreManager.draw({20, 20});
    this->DrawCurrentLives();

    // Debug HUD
    this->DrawDebugVariables();

    // Overlays
    if (startup) DrawStartup();
    if (paused) DrawPauseScreen();
    if (gameOver) DrawGameOver();
}

void Program::NewWave(void) {
    this->respawnCooldownReset = DEFAULT_COOLDOWN;
    this->respawnCooldown = this->respawnCooldownReset;

    for (int i = 0; i < 20; i++) {
        RespawnEnemy();
    }
}

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

            respawns++;
            break;
        } else if (!p.second && p.first.second == 150) {
            p.second = new SpEnemy(GetScreenWidth() / 2 - 15, 0, true);
            respawns++;
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
                NewWave();
                progWave++;
            }
        } else {
            int offsetScore = scoreManager.getScore() - (scoreManager.getWaveRate() * (scoreManager.getWave() - 1));
            respawnCooldownReset = DEFAULT_COOLDOWN * std::exp(-std::pow(offsetScore, 2) / 3000000);
                                            /* Original equation: ((DEFAULT_COOLDOWN - MIN_VALUE) * RATIONAL_OFFSET)   \
                                                        / (std::exp(scoreDistance / EXPONENTIAL_OFFSET) + RATIONAL_OFFSET) \
                                                                                                                        + MIN_VALUE;*/
            respawnCooldown = respawnCooldownReset;
            RespawnEnemy();
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

    FontManager::PixelFontBody.DrawText(TextFormat("Respawn cooldown reset: %01i", this->respawnCooldownReset), {150, 400}, baseSize, 0.1, RED);
    FontManager::PixelFontBody.DrawText(TextFormat("Respawn cooldown: %01i", this->respawnCooldown), {150, 500}, baseSize, 0.1, RED);
}

void Program::DrawStartup() {
    DrawRectangle(0, 0, (float)GetScreenWidth(), (float)GetScreenHeight(), Color{0, 0, 0, 125});
    DrawText("Galaga", (GetScreenWidth() / 2 - 237), 75, 144, WHITE);
    DrawText("Press Enter", (GetScreenWidth() / 2) - 75, GetScreenHeight() / 2, 24, GRAY);
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

    if (startup && IsKeyPressed(KEY_ENTER)) {
        startup = false;
        if (!inGame) {
            pauseFrames = 480;
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

void Program::Reset() {
    Enemy::enemies.clear();
    StdEnemy::attackInProgress = false;
    player = new Player((GetScreenWidth() / 2) - 15, GetScreenHeight() * 0.75f);
    scoreManager.resetScore(); // Reset score
    StartingPositionEnemies(); //Restarting the enemies to the start position
    respawnCooldown = respawnCooldownReset;
    respawns = 0;
    count = 0;
    delay = 0;
    lives = 3;
    pauseFrames = 480;
    PlaySound(SoundManager::theme );
    inGame = true;
}