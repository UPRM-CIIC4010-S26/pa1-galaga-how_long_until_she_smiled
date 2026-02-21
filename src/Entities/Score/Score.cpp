#include "Score.hpp"

void ScoreManager::draw(raylib::Vector2 position) {
    // Draw current score
    int baseSize = FontManager::PixelFontBody.GetBaseSize();
    FontManager::PixelFontBody.DrawText("Score", position, baseSize, 0.1f, RED);
    FontManager::PixelFontBody.DrawText(TextFormat("%04i", this->score), position + raylib::Vector2(0, 20), baseSize, 0.1, WHITE);

    // BONUS: Draw high score?
}

void ScoreManager::addToScore(unsigned int increment) {
    int scoreUpdate = this->score + increment;
    if (scoreUpdate > this->maxScore) scoreUpdate = this->maxScore;
    this->score += scoreUpdate;
}
unsigned int ScoreManager::addToScore(unsigned int increment, unsigned int amountToNextLife) {
    this->addToScore(increment);

    if (amountToNextLife == 0) return 0;

    int livesObtained = (this->score / amountToNextLife);
    return livesObtained;
}
unsigned int ScoreManager::addToScore(unsigned int increment, unsigned int amountToNextLife, unsigned int maxLives) {
    int livesObtained = this->addToScore(increment, amountToNextLife);

    if (livesObtained > maxLives) livesObtained = maxLives;
    return livesObtained;
}