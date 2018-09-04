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
};

class Map
{
private:
    string map[15][19];
    int mapGood[15][19];
    int mapBad[15][19];

    Point gateA, gateB;

    int coluna;
    int linha;

    void printMap();
    void printMapInt(int _map[15][19]);
    void makeDist(int _map[15][19], bool isGood, int _i = 0, int _j = 0);
    void findGates();

public:
    Map(int _width, int _height);
    ~Map();

    void updateMap(string _field);
};

#endif

