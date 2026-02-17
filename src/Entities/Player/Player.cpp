#include "Player.hpp"

void Player::draw() {
    /*
    draw() draws the player object using the spritesheets. If we are running in debug mode, its also going to draw the hitbox.
    */
    if (HitBox::drawHitbox) this->hitBox.draw();
    DrawTexturePro(ImageManager::SpriteSheet, Rectangle{0, 0, 17, 18}, 
                   Rectangle{this->position.first, this->position.second, 30, 30}, 
                   Vector2{0, 0}, 0, WHITE);
}

void Player::update() {
    /*
    update() makes sure that the same of the object hitbox are the same of the objects position coordinates and that the object cooldown counts down.
    It also checks if the player is out of bounds of the left or right walls, and if it is, then calculate how much out of bounds you are, then 
    depending on how the player is out of bounds (if its more vertical or horizontal out of bounds) puts the players in bounds.
    */

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
    */
    if (IsKeyDown('A')||IsKeyDown(KEY_LEFT)) this->position.first -= this->speed;
    if (IsKeyDown('D')||IsKeyDown(KEY_RIGHT)) this->position.first += this->speed;
    if (IsKeyPressed(KEY_SPACE) || IsKeyDown('Z')) this->attack();
}

void Player::attack() {
    /*
    attack() checks if attack() wasnt called in 30 frames (cooldown), creates a bullet and putting the location in the middle of the ship,
    and add it to the general vector (which all bullets are stored), plays the shoot sound effect and finally restarts the cooldown.
    */
    if (cooldown <= 0) {
        Projectile::projectiles.push_back(Projectile(position.first + + this->hitBox.box.width / 2, position.second, 0)); //"projectiles" and "Projectile" in "Projectile.hpp"
        PlaySound(SoundManager::shoot);
        cooldown = 30;
    }
}