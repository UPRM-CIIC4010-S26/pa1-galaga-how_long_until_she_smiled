#include "Score.hpp"

void ScoreManager::draw(raylib::Vector2 position) {
    // Draw current score
    int baseSize = FontManager::PixelFontBody.GetBaseSize();
    FontManager::PixelFontBody.DrawText("Score", position, baseSize, 0.1f, RED);
    FontManager::PixelFontBody.DrawText(TextFormat("%04i", this->score), position + raylib::Vector2(0, 20), baseSize, 0.1, WHITE);

    // BONUS: Draw high score?
}

void ScoreManager::addToScore(uint32_t increment) {
    int scoreUpdate = this->score + increment;
    if (scoreUpdate > this->maxScore) scoreUpdate = this->maxScore;
    this->score = scoreUpdate;
}
uint32_t ScoreManager::addToScore(uint32_t increment, uint32_t amountToNextLife) {
    this->addToScore(increment);

    if (amountToNextLife == 0) return 0;

    // TODO: Fix, as currently, the function increases the lives obtained every 2000-interval
    bool thousandRangeUpdate = ((this->score / amountToNextLife) > (this->score - increment) / amountToNextLife);
    int livesObtained = ((thousandRangeUpdate) ? 1 : 0);

    return livesObtained;
}