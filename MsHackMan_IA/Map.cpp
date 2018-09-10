#include "Map.h"

Map::Map(int _width, int _height)
    : coluna(_width), linha(_height)
{
}

void
Map::updateMap(string _field)
{
    //zera todos o vetores que armazenam os dados do mapa
    snippets.clear();
    bombs.clear();
    bugs.clear();
    spawn.clear();

    vector<Point> todo;

    //verifica na string recebida o que tem no mapa e adiciona os pontos
    int s = 0;
    for (int i = 0; i < linha; ++i)
    {
        for (int j = 0; j < coluna; ++j)
        {
            while (s < _field.size() && _field[s] != ',')
            {
                //se é um player
                if (_field[s] == 'P')
                {
                    s++;
                    if (_field[s] == playerID + 48)
                    {
                        playerPos.x = i;
                        playerPos.y = j;
                        todo.push_back(playerPos);
                    }
                    else
                    {
                        enemyPos.x = i;
                        enemyPos.y = j;
                        todo.push_back(enemyPos);
                    }
                }
                //se é um spawn point
                else if (_field[s] == 'S')
                {
                    tmp = new Point;
                    tmp->x = i;
                    tmp->y = j;
                    tmp->type = 250;
                    if ((s + 1) < _field.size() && _field[s + 1] != ','  && _field[s + 1] != ';')
                    {
                        s++;
                        tmp->type = _field[s] - 48;
                    }
                    spawn.push_back(*tmp);

                    todo.push_back(*tmp);
                }                
                //se é um bug
                else if (_field[s] == 'E')
                {
                    tmp = new Point;
                    tmp->x = i;
                    tmp->y = j;
                    s++;
                    tmp->type = _field[s] - 48;
                    bugs.push_back(*tmp);
                    todo.push_back(*tmp);                    
                }
                //se é bomba
                else if (_field[s] == 'B')
                {
                    tmp = new Point;
                    tmp->x = i;
                    tmp->y = j;
                    tmp->type = 10;
                    if ((s + 1) < _field.size() && _field[s + 1] != ','  && _field[s + 1] != ';')
                    {
                        s++;
                        tmp->type = _field[s] - 48;
                    }
                    bombs.push_back(*tmp);

                    todo.push_back(*tmp);
                }
                //se for um snippet
                else if (_field[s] == 'C')
                {
                    tmp = new Point;
                    tmp->x = i;
                    tmp->y = j;
                    snippets.push_back(*tmp);
                    todo.push_back(*tmp);
                    //cout << i << " " << j << endl;
                }
                s++;
            }
            s++;
        }
    }

    /*
    for (int i = 0; i < bugs.size(); i++)
    {
        cout << "bug: " << bugs[i].x << " " << bugs[i].y << " " << bugs[i].type << endl;
    }
    for (int i = 0; i < bombs.size(); i++)
    {
        cout << "bomb: " << bombs[i].x << " " << bombs[i].y << " " << bombs[i].type << endl;
    }
    for (int i = 0; i < snippets.size(); i++)
    {
        cout << "snippet: " << snippets[i].x << " " << snippets[i].y << endl;
    }
    */

    if (fUpdate) 
    {
        s = 0;
        for (int i = 0; i < linha; ++i)
        {
            for (int j = 0; j < coluna; ++j)
            {
                while (s < _field.size() && _field[s] != ',')
                {
                    if (_field[s] == 'x')
                    {
                        map[i][j] = "x";
                    }
                    else if(_field[s] == 'G')
                    {
                        map[i][j] = "G";
                        while (s+1 < _field.size() && _field[s+1] != ',')
                        {
                            s++;
                        }
                    }
                    else
                    {
                        map[i][j] = ".";
                    }               
                    s++;
                }
                s++;
            }
        }

        firstUpdate();

    }

    for (int s = 0; s < todo.size(); s++)
    {
        int i = todo[s].x;
        int j = todo[s].y;
        if (!influencias[i][j].done)
        {
            influencias[i][j].done = true;
            resetDist(influencias[i][j].map, influencias[i][j].bugMap);
            influencias[i][j].bugMap[i][j] = influencias[i][j].map[i][j] = 0;
            makeDist(influencias[i][j].map, i, j);
            makeDist(influencias[i][j].bugMap, i, j, true);
            //influencias[i][j].bugMap[gateA.x][gateA.y] = 1000;
            //influencias[i][j].bugMap[gateB.x][gateB.y] = 1000;
        }
    }

    //printInfluencias();
}

//verifica todos os lados (up, left, down n right)
//dessa maneira, dá valor para cada casa possivel de andar ("+1 p/ cada larço")
//params:
//  _map[][] = mapa para que será editado (good ou bad)
//  isGood = se o mapa a ser verificado é de coisas boas (snippets e bomba para pegar)
//           afeta principalmente no gate, pois o menor caminho atravessa o gate
//  _x e _y = posição atual
void
Map::makeDist(int _map[20][20], int _i, int _j, bool _isBad)
{
    /*
    map[_i][_j] = "o";

    if (_i > 0)
        map[_i-1][_j] = "c";
    if (_j > 0)
        map[_i][_j-1] = "e";
    if(_i < (linha - 1))
        map[_i+1][_j] = "b";
    if(_j < (coluna - 1))
        map[_i][_j+1] = "d";
    
    printMap();
    */

    //Caso estejamos no portal e seja pra mapear coisas boas
    //  (pois bugs não atravessam, logo a influência não passa pelo portal)
    if (map[_i][_j] == "G" && !_isBad)
    {
        //caso estejamos no portal A
        if (_j == gateA.y)
        {
            if (_map[gateB.x][gateB.y] > (_map[_i][_j] + 1))
            {
                _map[gateB.x][gateB.y] = _map[_i][_j] + 1;
                makeDist(_map, gateB.x, gateB.y, _isBad);
            }
        }
        else
        {
            if (_map[gateA.x][gateA.y] > (_map[_i][_j] + 1))
            {
                _map[gateA.x][gateA.y] = _map[_i][_j] + 1;
                makeDist(_map, gateA.x, gateA.y, _isBad);
            }
        }
    }
    
    //verifica o valor de cima
    if (_i > 0 && map[_i - 1][_j] != "x")
    {
        //verifica se o valor de cima é maior que o valor atual +1
        //se for, coloca o valor atual mais no lugar e chama a recursão
        //para o novo ponto
        if (_map[_i - 1][_j] > (_map[_i][_j] + 1))
        {
            _map[_i - 1][_j] = _map[_i][_j] + 1;
            makeDist(_map, _i - 1, _j, _isBad);
        }
    }
    //verifica o valor da esquerda
    if (_j > 0 && (map[_i][_j - 1] != "x"))
    {
        if (_map[_i][_j - 1] > (_map[_i][_j] + 1))
        {
            _map[_i][_j - 1] = _map[_i][_j] + 1;
            makeDist(_map, _i, _j - 1, _isBad);
        }
    }
    //verifica o valor de baixo
    if (_i < (linha - 1) && map[_i + 1][_j] != "x")
    {
        if (_map[_i + 1][_j] > (_map[_i][_j] + 1))
        {
            _map[_i + 1][_j] = _map[_i][_j] + 1;
            makeDist(_map, _i + 1, _j, _isBad);
        }
    }
    //verifica o valor da direita
    if (_j < (coluna - 1) && (map[_i][_j + 1] != "x"))
    {
        if (_map[_i][_j + 1] > (_map[_i][_j] + 1))
        {
            _map[_i][_j + 1] = _map[_i][_j] + 1;
            makeDist(_map, _i, _j + 1, _isBad);
        }
    }
}

void
Map::findGates()
{
    bool gA = false;

    for (int i = 0; i < linha; ++i)
    {
        for (int j = 0; j < coluna; ++j)
        {
            if (!gA && map[i][j] == "G")
            {
                gateA.x = i;
                gateA.y = j;
                gA = true;
            }
            else if (map[i][j] == "G")
            {
                gateB.x = i;
                gateB.y = j;
            }
        }
    }

    //cout << ">>> Gate A: " << gateA.x << " " << gateA.y << endl << ">>> Gate B: " << gateB.x << " " << gateB.y << endl;
}

void
Map::printMap()
{
    for (int i = 0; i < linha; ++i)
    {
        for (int j = 0; j < coluna; ++j)
        {
            cout << map[i][j] << " ";
        }

        cout << endl;
    }
}

void
Map::printMapInt(int _map[20][20])
{
    for (int i = 0; i < linha; ++i)
    {
        for (int j = 0; j < coluna; ++j)
        {
            if (_map[i][j] < 10)
                cout << 0 << _map[i][j];
            else if (_map[i][j] == 1000)
                cout << "xx";
            else
                cout << _map[i][j];
            cout << " ";
        }

        cout << endl;
    }
}

void
Map::firstUpdate()
{
    //printMap();

    findGates();

    //cout << "good map:\n"; printMapInt(distMap);
    //cout << "bad map:\n"; printMapInt(bugDistMap);

    fUpdate = false;
}

void
Map::validMoves(vector<string> & _vm)
{
    int i = playerPos.x;
    int j = playerPos.y;

    if (i > 0 && map[i - 1][j] == ".")
        _vm.push_back("up");
    if (j > 0 && map[i][j - 1] == ".")
        _vm.push_back("left");
    if (i < (linha - 1) && map[i + 1][j] == ".")
        _vm.push_back("down");
    if (j < (coluna - 1) && map[i][j + 1] == ".")
        _vm.push_back("right");
}

void
Map::getSnippets(vector<Point> & _snp)
{
    for(int i = 0; i < snippets.size(); i++)
        _snp.push_back(snippets[i]);
}

void
Map::getBugs(vector<Point> & _bugs)
{
    for (int i = 0; i < bugs.size(); i++)
        _bugs.push_back(bugs[i]);
}

void
Map::getBombs(vector<Point> & _bombs)
{
    for (int i = 0; i < bombs.size(); i++)
        _bombs.push_back(bombs[i]);
}

bool
Map::isWall(int _i, int _j)
{
    if (_i < 0)
        return false;
    if (_j < 0)
        return false;
    if (_i > (linha - 1))
        return false;
    if (_j > (coluna - 1))
        false;

    if (map[_i][_j] == "x")
        return true;
    else
        return false;
}

void
Map::resetDist(int _map[20][20], int _bMap[20][20])
{
    for (int i = 0; i < linha; ++i)
    {
        for (int j = 0; j < coluna; ++j)
        {
            _map[i][j] = _bMap[i][j] = 1000;
        }
    }
}

void
Map::printInfluencias()
{
    for (int i = 0; i < linha; ++i)
    {
        for (int j = 0; j < coluna; ++j)
        {
            if (influencias[i][j].done)
            {
                cout << "mapa " << i << " " << j << ": \n";
                printMapInt(influencias[i][j].map);
                //printMapInt(influencias[i][j].bugMap);
                cout << endl;
            }
        }
    }
}

int
Map::distAB(Point _a, Point _b, bool _isGood)
{
    if (_isGood)
        return influencias[_a.x][_a.y].map[_b.x][_b.y];
    else
        return influencias[_a.x][_a.y].bugMap[_b.x][_b.y];
}

int
Map::distAB(int _i, int _j, Point _b, bool _isGood)
{
    //caso gate A
    if (_i == gateA.x)
    {
        //gate esquerdo
        if (_j == gateA.y - 1)
        {
            return distAB(gateB.x, gateB.y, _b, _isGood);
        }
    }

    //caso gate A
    if (_i == gateB.x)
    {
        //gate esquerdo
        if (_j == gateB.y + 1)
        {
            return distAB(gateA.x, gateA.y, _b, _isGood);
        }
    }

    if (_i < 0)
        return 1000;
    if (_j < 0)
        return 1000;
    if (_i > (linha - 1))
        return 1000;
    if (_j > (coluna - 1))
        return 1000;

    if (_isGood)
        return influencias[_i][_j].map[_b.x][_b.y];
    else
        return influencias[_i][_j].bugMap[_b.x][_b.y];
}

int
Map::distAB(Point _a, int _i, int _j, bool _isGood)
{
    //caso gate A
    if (_i == gateA.x)
    {
        //gate esquerdo
        if (_j == gateA.y - 1)
        {
            return distAB(_a, gateB.x, gateB.y, _isGood);
        }
    }
    //caso gate A
    if (_i == gateB.x)
    {
        //gate esquerdo
        if (_j == gateB.y + 1)
        {
            return distAB(_a, gateA.x, gateA.y, _isGood);
        }
    }

    if (_i < 0)
        return 1000000;
    if (_j < 0)
        return 1000000;
    if (_i > (linha - 1))
        return 1000000;
    if (_j > (coluna - 1))
        return 1000000;

    if (_isGood)
        return influencias[_a.x][_a.y].map[_i][_j];
    else
        return influencias[_a.x][_a.y].bugMap[_i][_j];
}

bool
Map::canGo(int _i, int _j)
{
    //caso gate A
    if (_i == gateA.x)
    {
        //gate esquerdo
        if (_j == gateA.y - 1)
        {
            return true;
        }
    }
    //caso gate A
    if (_i == gateB.x)
    {
        //gate esquerdo
        if (_j == gateB.y + 1)
        {
            return true;
        }
    }

    if (_i < 0)
        return false;
    if (_j < 0)
        return false;
    if (_i > (linha - 1))
        return false;
    if (_j > (coluna - 1))
        return false;

    return map[_i][_j] != "x";
}

void
Map::getMap(string _map[20][20])
{
    for (int i = 0; i < linha; ++i)
    {
        for (int j = 0; j < coluna; ++j)
        {
            _map[i][j] = map[i][j];
        }
    }
}

