#include "Player.hpp"

void Player::draw() {
    if (HitBox::drawHitbox) this->hitBox.draw();
    DrawTexturePro(ImageManager::SpriteSheet, Rectangle{0, 0, 17, 18}, 
                   Rectangle{this->position.first, this->position.second, 30, 30}, 
                   Vector2{0, 0}, 0, WHITE);
}

void Player::update() {
    this->hitBox.box.x = this->position.first;
    this->hitBox.box.y = this->position.second;
    this->cooldown--;

    if (HitBox::Collision(Background::sideWalls.first, this->hitBox)) {
        std::pair<double, double> collision = HitBox::CollisionMargins(this->hitBox, Background::sideWalls.first);
        if (abs(collision.second) < abs(collision.first)) {
            this->position.second += collision.second;
        } else {
            this->position.first += collision.first + 1;
        }
    }
    if (HitBox::Collision(Background::sideWalls.second, this->hitBox)) {
        std::pair<double, double> collision = HitBox::CollisionMargins(this->hitBox, Background::sideWalls.second);
        if (abs(collision.second) < abs(collision.first)) {
            this->position.second += collision.second;
        } else {
            this->position.first += collision.first + 1;
        }
    }
}

void Player::keyInputs() {
    /*
    keyInputs() makes that the player can move left ("A" or LEFT_KEY) and right ("D" or RIGHT_KEY) by adding 
    or removing speed and fire bullets (SPACE_KEY or "Z") by calling the attack function from the specific player object
    calling the function.
    (IsKeyDown is apart of RayLib Lib)
    */
    if (IsKeyDown('A')||IsKeyDown(KEY_LEFT)) this->position.first -= this->speed;
    if (IsKeyDown('D')||IsKeyDown(KEY_RIGHT)) this->position.first += this->speed;
    if (IsKeyPressed(KEY_SPACE) || IsKeyDown('Z')) this->attack();
}

void Player::attack() {
    if (cooldown <= 0) {
        Projectile::projectiles.push_back(Projectile(position.first + + this->hitBox.box.width / 2, position.second, 0));
        PlaySound(SoundManager::shoot);
        cooldown = 30;
    }
}