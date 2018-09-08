#ifndef _MAP_H_
#define _MAP_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Point
{
    int x;
    int y;
    int type;
};

class Map
{
private:
    string map[100][100];
    int distMap[100][100];

    Point gateA, gateB;

    int coluna;
    int linha;

    Point * tmp;
    vector<Point> snippers;
    vector<Point> bombs;
    vector<Point> bugs;
    vector<Point> spawn;

    bool fUpdate = true;
    int playerID;

    Point playerPos;
    Point enemyPos;

    void printMap();
    void printMapInt(int _map[100][100]);
    void makeDist(int _map[100][100], int _i = 0, int _j = 0);
    void firstUpdate();

public:
    Map(int _width, int _height);
    ~Map() { delete[] tmp; };

    void updateMap(string _field);
    void setPlayerID(int id) { playerID = id; };
};

#endif

