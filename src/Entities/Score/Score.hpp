#pragma once

#include <cstdint>
#include "raylib-cpp/raylib-cpp.hpp"
#include "FontManager.hpp"

// Manage scoring actions performed by the scoring system relating to itself
class ScoreManager {
    private:
        uint32_t score;
        uint32_t maxScore = 99990;

        uint32_t highScore = 0;

        static const uint32_t toNextWaveStart = 1500;

        uint32_t waveRate = 2000;
        uint32_t toNextWave = toNextWaveStart;
        uint32_t wave = 1;
    public:
        // The single most important function in this class
        int getScore(void) { return this->score; }
        int getWave(void) { return this->wave; }
        int getRWave();
        int getWaveRate(void) { return this->waveRate; }
        int getToNextWave(void) { return this->toNextWave; }

        // Adds to score, checking maximum values in order to not overflow
        void addToScore(uint32_t increment);
        // Additionally, return the amount of extra lives the player would have given an amount to the next life
        uint32_t addToScore(uint32_t increment, uint32_t amountToNextLife);

        void nextWave(void);

        void draw(raylib::Vector2 position);
        void draw(raylib::Vector2 position, int asyncWave);

        ScoreManager(void) {this->score = 0; }
        ScoreManager(int startingValue) { this->score = startingValue; }

        void setHighScore(void) { if (this->score > this->highScore) this->highScore = this->score; }

        void resetScore(void) { 
            this->setHighScore(); 
            this->score = 0; 
            this->wave = 1;
            this->toNextWave = toNextWaveStart;
        }
        void resetScore(int startingValue) { 
            this->setHighScore(); 
            this->score = startingValue; 
            this->wave = 1; 
            this->toNextWave = toNextWaveStart; 
        }
};