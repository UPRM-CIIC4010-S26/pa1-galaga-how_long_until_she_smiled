#include "Score.hpp"
#include "Enemy.hpp"
#include "Loop.hpp"


int ScoreManager::getRWave() { return Loop::stagesActive ? ((this->wave - 1) % 3) + 1 : this->wave;}

// ----- Game loop functions -----
// Drawing method for the score manager, to be called in main game loop
void ScoreManager::draw(raylib::Vector2 position) {
    // Draw current score
    int baseSize = FontManager::PixelFontBody.GetBaseSize();
    FontManager::PixelFontBody.DrawText("Score", position, baseSize, 0.1f, RED);
    FontManager::PixelFontBody.DrawText(TextFormat("%04i", this->score), position + raylib::Vector2(0, 20), baseSize, 0.1, WHITE);

    // BONUS: Draw high score
    raylib::Vector2 hiScoreOffset = raylib::Vector2(250, 0);
    FontManager::PixelFontBody.DrawText("Hi Score", position + hiScoreOffset, baseSize, 0.1f, RED);
    FontManager::PixelFontBody.DrawText(TextFormat("%05i", this->highScore), position + hiScoreOffset + raylib::Vector2(0, 20), baseSize, 0.1, WHITE);

    // BONUS: Draw current wave
    raylib::Vector2 waveOffset = raylib::Vector2(GetScreenWidth() * 3/4, 0);
    FontManager::PixelFontBody.DrawText(TextFormat("Wave %02i", this->getWave()), position + waveOffset, baseSize, 0.1f, RED);
}
void ScoreManager::draw(raylib::Vector2 position, int asyncWave) {
    this->draw(position);

    int baseSize = FontManager::PixelFontBody.GetBaseSize();
    int x = this->getWave();

    raylib::Vector2 waveOffset = raylib::Vector2(GetScreenWidth() * 5/8, 0);
    FontManager::PixelFontBody.DrawText((asyncWave != this->getWave() ? "Incoming Wave!" : ""), position + waveOffset + raylib::Vector2(0, 20), baseSize, 0.1f, RED);
}

// ----- Adding to score -----
void ScoreManager::addToScore(uint32_t increment) {
    int scoreUpdate = this->score + increment;
    if (scoreUpdate > this->maxScore) scoreUpdate = this->maxScore;

    if (score >= toNextWave){
        if (Loop::stagesActive){

            if (this->getWave() < 3){
                this->nextWave();
            } else if (this->getWave() == 3 && Enemy::aliveEnemies == 1) this->nextWave();
            else this->toNextWave += this->waveRate;

        } else this->nextWave();
    }
    
    this->score = scoreUpdate;
}
uint32_t ScoreManager::addToScore(uint32_t increment, uint32_t amountToNextLife) {
    this->addToScore(increment);

    if (amountToNextLife == 0) return 0;

    // Check if the amountToNextLife-th digit of the score is greater than what is was before
    bool thousandRangeUpdate = ((this->score / amountToNextLife) > (this->score - increment) / amountToNextLife);
    int livesObtained = ((thousandRangeUpdate) ? 1 : 0);

    return livesObtained;
}

// ----- Wave transition -----
// Currently works like a linear sequence
void ScoreManager::nextWave(void) {
    this->wave++;
    this->toNextWave += this->waveRate;
}