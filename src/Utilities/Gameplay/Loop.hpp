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
    void SetMode(bool endless){this->isEndless = endless;}
    void StartingPositionEnemies();
    void reset();
};