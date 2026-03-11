#pragma once
#include "StdEnemy.hpp"
#include "SpEnemy.hpp"
#include "StEnemy.hpp"
#include "DyEnemy.hpp"
#include "Score.hpp"

using namespace std;

class Loop{
    private:
    enum class stages{DEFAULT, SECOND};
    bool isEndless;
    stages stageRN;


    public:
    int getNextStage(){return (int)stageRN+1;}
    int getStage(){return (int)stageRN;}
    void StartingPositionEnemies();
    void SecondStageEnemies();
    void ThirdStageEnemies();
    void FourthStageEnemies();
    void FifthStageEnemies();
    void KillEmAll(void);
    void reset(bool next_stage);
    void reset(){StartingPositionEnemies();}
    
};