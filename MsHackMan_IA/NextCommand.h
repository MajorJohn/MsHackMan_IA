#ifndef _NEXTCOMMAND_H_
#define _NEXTCOMMAND_H_

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <ctime>

#include "map.h"

using namespace std;

struct Player {
    string name;
    int id;
    int snippets;
    int bombs;
    Point point;
};

struct Influencia {
    int snippet;
};

class NextCommand
{
private:
    Player me;
    Player enemy;

    Map *map;

    int width;
    int height;

    int coluna;
    int linha;

    int timebank;
    int time_per_move;
    int time_remaining;
    int max_rounds;
    int current_round;

    //influência pra cima([0][j]), esquerda([1][j])
    //pra baixo([2][j]) e direita ([3][j])
    //onde j é a quantidade de objetos com a influência j
    Influencia influencias[4][40];

    bool canUp;
    bool canLeft;
    bool canDown;
    bool canRight;

    int up;
    int left;
    int down;
    int right;

    int bUp;
    int bLeft;
    int bDown;
    int bRight;

    int gUp;
    int gLeft;
    int gDown;
    int gRight;

    bool sendoPerseguido;
    bool usingBomb;
    bool scapingBomb;
    int timeBomb;
    bool firstBomb = false;
    int pass;

    bool bugUp;
    bool bugLeft;
    bool bugDown;
    bool bugRight;

    string sUseBomb;
    string pathScapeBomb[10];
    string sMap[20][20];

    vector< Point > snippets;
    vector< Point > bombs;
    //não sei porque, mas quando não declaro algo antes do vector bugs, da uma especie de segmetion fall
    vector< Point > bug;
    vector< Point > bugs;
    vector< Point > spawns;

    bool boy = false;

    bool firstUpdate = true;

    void settings();
    void update();
    void action();

    void chooseCharacter();
    void doMove();

    void inflZero();

    void caseIqual();

    void getMap();
    void useBomb(int _distBug);
    void scape();

public:

    NextCommand();
    ~NextCommand();

    void process();


};

#endif