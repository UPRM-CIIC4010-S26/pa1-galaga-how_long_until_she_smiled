#pragma once

#include <cstdint>
#include "raylib-cpp/raylib-cpp.hpp"
#include "FontManager.hpp"

// Manage scoring actions performed by the scoring system relating to itself
class ScoreManager {
    private:
        uint32_t score;
        uint32_t maxScore = 99990;
    public:
        // The single most important function in this class
        int getScore(void) { return this->score; }

        // Adds to score, checking maximum values in order to not overflow
        void addToScore(uint32_t increment);
        // Additionally, return the amount of extra lives the player would have given an amount to the next life
        uint32_t addToScore(uint32_t increment, uint32_t amountToNextLife);

        void draw(raylib::Vector2 position);

        ScoreManager(void) { this->score = 0; }
        ScoreManager(int startingValue) { this->score = startingValue; }

        void resetScore(void) { this->score = 0; }
        void resetScore(int startingValue) { this->score = startingValue; }
};