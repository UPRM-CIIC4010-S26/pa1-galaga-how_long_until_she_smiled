#include "Score.hpp"

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