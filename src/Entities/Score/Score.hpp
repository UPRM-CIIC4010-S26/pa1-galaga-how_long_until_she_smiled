#pragma once

#include "raylib-cpp/raylib-cpp.hpp"
#include "FontManager.hpp"

class ScoreManager {
    private:
        int score;
        int maxScore = 9999;
    public:
        // The single most important function in this class
        int getScore(void) { return this->score; }

        // Adds to score, checking maximum values in order to not overflow
        void addToScore(unsigned int increment);
        // Additionally, return the amount of extra lives the player would have given an amount to the next life
        unsigned int addToScore(unsigned int increment, unsigned int amountToNextLife);
        // Additionally additionally, cap the amount of lives returned to maxLives
        unsigned int addToScore(unsigned int increment, unsigned int amountToNextLife, unsigned int maxLives);

        void draw(raylib::Vector2 position);

        ScoreManager(void) { this->score = 0; }
        ScoreManager(int startingValue) { this->score = startingValue; }

        void resetScore(void) { this->score = 0; }
        void resetScore(int startingValue) { this->score = startingValue; }
};