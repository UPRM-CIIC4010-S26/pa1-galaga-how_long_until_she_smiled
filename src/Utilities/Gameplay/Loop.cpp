#include "Loop.hpp"



void Loop::StartingPositionEnemies(){
    /*
    AddingEnemies() makes sure to add all the enemies to the vector, with the starting location.
    */
    //Sp Enemy adding (the two enemies at the top)------------------------
    Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
        std::pair<float, float>{350, 150}, 
        new SpEnemy(350, 150)
    });

    Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
        std::pair<float, float>{600, 150}, 
        new SpEnemy(600, 150)
    });
    //Rest of the enemies
    for (int i = 0; i < 30; i++) {
        float x = 250 + 50 * (i%10);
        float y = 200 + 50 * (i/10);

        Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
            std::pair<float, float>{x, y}, 
            new StdEnemy(x, y)
        });

    stageRN = stages::DEFAULT;
}};

void Loop::KillEmAll(void) {
    // not sure if I should keep this method around, because whoever creates the 
    // enemies should destroy them
    // and there already exists a perfectly good clear() function for vectors
    //
    // but the method name is so fucking cool....
    // https://github.com/Svxy/The-Simpsons-Hit-and-Run/blob/0cf35df0a921a803a69d0da4c2aca02aa2543f03/game/code/worldsim/vehiclecentral.cpp#L1257
}

void Loop::reset(){
    switch (stageRN){
        case stages::DEFAULT:
            StartingPositionEnemies();
            break;
        case stages::SECOND:
            break;
    }

}
