#pragma once
#include "Hitbox.hpp"
#include "Projectile.hpp"
#include "Background.hpp"
#include "ImageManager.hpp"

class Player {
    private:
        int cooldown = 30; //Countdown for firing (FRAMES)
        int speed = 3; //Pixel per frame (speed = 3ppf)
    
    public:
        std::pair<float, float> position; //Position coordinates
        HitBox hitBox;

        Player(float x, float y) {
            this->position.first = x;
            this->position.second = y;
            this->hitBox = HitBox(x, y, 30, 30);
        }

        void draw();
        void update();
        void keyInputs();
        void attack();
};