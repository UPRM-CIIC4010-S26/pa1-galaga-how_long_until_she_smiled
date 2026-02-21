#include "Score.hpp"

void ScoreManager::draw(Font providedFont, raylib::Vector2 position) {
    // Draw current score
    DrawTextEx(providedFont, "Score", position, 20, 0.1, RED);
    DrawTextEx(providedFont, TextFormat("%04i", this->score), position + raylib::Vector2(0, 20), 20, 0.1, WHITE);

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