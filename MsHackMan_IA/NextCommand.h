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

class NextCommand
{
private:
    Player me;
    Player enemy;

    Map *map;

    int width;
    int height;

    int timebank;
    int time_per_move;
    int time_remaining;
    int max_rounds;
    int current_round;

    vector< Point > snippets;
    vector< Point > weapons;
    vector< Point > bugs;
    vector< Point > spawn_points;
    vector< Point > gates;
    vector< vector<string> > cells;

    bool boy = false;

    bool firstUpdate = true;

    void settings();
    void update();
    void action();

    void chooseCharacter();
    void doMove();

public:

    NextCommand();
    ~NextCommand();

    void process();

    void debug();


};

#endif