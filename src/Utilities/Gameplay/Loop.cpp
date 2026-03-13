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
    Enemy::aliveEnemies++;

    Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
        std::pair<float, float>{600, 150}, 
        new SpEnemy(600, 150)
    });
    Enemy::aliveEnemies++;

    //Rest of the enemies
    for (int i = 0; i < 30; i++) {
        float x = 250 + 50 * (i%10);
        float y = 200 + 50 * (i/10);

        Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
            std::pair<float, float>{x, y}, 
            new StdEnemy(x, y)
        });
        Enemy::aliveEnemies++;

}};

void Loop::ThirdStageEnemies(){
    int totalRows = 8;
    float spacing = 50;
    for (int rows = 0;rows<=7; rows++){
        float enemiesInRow = 10- rows;
        float xStart = 500 - (((enemiesInRow) * spacing) / 2);
        for (int i = 0; i < enemiesInRow;i++){
            float x = xStart + i * spacing;
            float y = 100 + ((rows)*50);

        
        if (rows==0 && ((i % 2 == 0 && i <= 4) || (i % 2 != 0 && i > 3))){
            Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
                std::pair<float, float>{x, y}, 
                new SpEnemy(x, y)
            });
        }else
            Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
            std::pair<float, float>{x, y}, 
            new StdEnemy(x, y)
        });
        Enemy::aliveEnemies++;
    }}
};


void Loop::SecondStageEnemies(){
    int spacing = 50;

    float rowXStart = 500 - ((3 * spacing) / 2.0f);
    for (int i = 0; i < 4; i++) {
        float x = rowXStart + (i * spacing);
        float y = 100;
        Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
            std::pair<float, float>{x, y}, 
            new SpEnemy(x, y)
        });
        Enemy::aliveEnemies++;
    }
    float rowMidXStart = 500 - ((7 * spacing) / 2.0f);
    for (int r = 1; r <= 3; r++) {
        for (int i = 0; i < 8; i++) {
            float x = rowMidXStart + (i * spacing);
            float y = 100 + (spacing * r); // Offset by 1 row
            Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
                std::pair<float, float>{x, y}, 
                new StdEnemy(x, y)
            });
            Enemy::aliveEnemies++;
        }
    }
    float rowBotXStart = 500 - ((5 * spacing) / 2.0f);
    for (int r = 4; r <= 5; r++) {
        for (int i = 0; i < 6; i++) {
            float x = rowBotXStart + (i * spacing);
            float y = 100 + (spacing * r); // Offset by 3 rows total
            Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
                std::pair<float, float>{x, y}, 
                new StdEnemy(x, y)
            });
            Enemy::aliveEnemies++;
        }
    }

}

void Loop::KillEmAll(void) {
    // not sure if I should keep this method around, because whoever creates the 
    // enemies should destroy them
    // and there already exists a perfectly good clear() function for vectors
    //
    // but the method name is so fucking cool....
    // https://github.com/Svxy/The-Simpsons-Hit-and-Run/blob/0cf35df0a921a803a69d0da4c2aca02aa2543f03/game/code/worldsim/vehiclecentral.cpp#L1257
}

void Loop::reset(bool next_stage){
    if (stagesActive){
        if (next_stage) this->stageRN=static_cast<stages>(static_cast<int>(stageRN) + 1);    
        switch (stageRN){
            case stages::DEFAULT:
                StartingPositionEnemies();
                break;
            case stages::SECOND:
                SecondStageEnemies();
                break;
            case stages::THIRD:
                ThirdStageEnemies();
                stageRN = stages::DEFAULT;
                break;
            }
    }else StartingPositionEnemies();


    
}


