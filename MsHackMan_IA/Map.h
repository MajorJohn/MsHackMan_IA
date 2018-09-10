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

struct InfluenciaMap
{
    bool done = false;
    int map[20][20];
    int bugMap[20][20];
};

class Map
{
private:
    string map[20][20];
    //int distMap[20][20];
    //int bugDistMap[20][20];

    InfluenciaMap influencias[20][20];

    Point gateA, gateB;

    int coluna;
    int linha;

    Point * tmp;
    vector<Point> snippets;
    vector<Point> bombs;

    vector<Point> bugs;
    vector<Point> spawn;

    bool fUpdate = true;
    int playerID;

    Point playerPos;
    Point enemyPos;

    void printMap();
    void printMapInt(int _map[20][20]);
    void findGates();
    void makeDist(int _map[20][20], int _i = 0, int _j = 0, bool _isBad = false);
    void firstUpdate();
    void resetDist(int _map[20][20], int _bMap[20][20]);

    void printInfluencias();

public:
    Map(int _width, int _height);
    ~Map() { delete[] tmp; };

    void updateMap(string _field);
    void setPlayerID(int id) { playerID = id; };
    void validMoves(vector<string> & _vm);

    void getPlayerPos(Point & pt) { pt.x = playerPos.x; pt.y = playerPos.y; };
    void getSnippets(vector<Point> & _snp);
    void getBugs(vector<Point> & _bugs);
    void getBombs(vector<Point> & _bombs);
    void getSpawns(vector<Point> & _spws);

    bool isWall(int _i, int _j);

    int distAB(Point _a, Point _b, bool _isGood = true);
    int distAB(int _i, int _j, Point _b, bool _isGood = true);
    int distAB(Point _a, int _i, int _j, bool _isGood = true);

    bool canGo(int _i, int _j);

    string getMap(int _i, int _j);
};

#endif

