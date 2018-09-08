#include "Map.h"

Map::Map(int _width, int _height)
    : coluna(_width), linha(_height)
{
    
    for (int i = 0; i < linha; ++i)
    {
        for (int j = 0; j < coluna; ++j)
        {
            distMap[i][j] = 1000;
        }
    }
}

void
Map::updateMap(string _field)
{
    //zera todos o vetores que armazenam os dados do mapa
    snippers.clear();
    bombs.clear();
    bugs.clear();
    spawn.clear();    

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
                    }
                    else
                    {
                        enemyPos.x = i;
                        enemyPos.y = j;
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
                }
                //se é um portal
                else if (_field[s] == 'G' && fUpdate)
                {
                    s++;
                    if (_field[s] == 'l')
                    {
                        gateA.x = i;
                        gateA.y = j;
                    }
                    else
                    {
                        gateB.x = i;
                        gateB.y = j;
                    }
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
                }
                //se for um snipper
                else if (_field[s] == 'C')
                {
                    tmp = new Point;
                    tmp->x = i;
                    tmp->y = j;
                    s++;
                    tmp->type = _field[s] - 48;
                    snippers.push_back(*tmp);
                }
                s++;
            }
            s++;
        }
    }

    for (int i = 0; i < bugs.size(); i++)
    {
        cout << "bug: " << bugs[i].x << " " << bugs[i].y << " " << bugs[i].type << endl;
    }
    for (int i = 0; i < bombs.size(); i++)
    {
        cout << "bomb: " << bombs[i].x << " " << bombs[i].y << " " << bombs[i].type << endl;
    }
    for (int i = 0; i < snippers.size(); i++)
    {
        cout << "snipper: " << snippers[i].x << " " << snippers[i].y << endl;
    }

    if (fUpdate) 
    {        
        for (int i = 0; i < linha; ++i)
        {
            for (int j = 0; j < coluna; ++j)
            {
                map[i][j] = "";
                while (s < _field.size() && _field[s] != ',')
                {
                    map[i][j] += _field[s];
                    s++;
                }
                s++;
            }
        }

        firstUpdate();
    }
}

//verifica todos os lados (up, left, down n right)
//dessa maneira, dá valor para cada casa possivel de andar ("+1 p/ cada larço")
//params:
//  _map[][] = mapa para que será editado (good ou bad)
//  isGood = se o mapa a ser verificado é de coisas boas (snippers e bomba para pegar)
//           afeta principalmente no gate, pois o menor caminho atravessa o gate
//  _x e _y = posição atual
void
Map::makeDist(int _map[100][100], int _i, int _j)
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
    if (map[_i][_j] == "G")
    {
        //caso estejamos no portal A
        if (_j == gateA.y)
        {
            if (_map[gateB.x][gateB.y] > (_map[_i][_j] + 1))
            {
                _map[gateB.x][gateB.y] = _map[_i][_j] + 1;
                makeDist(_map, gateB.x, gateB.y);
            }
        }
        else
        {
            if (_map[gateA.x][gateA.y] > (_map[_i][_j] + 1))
            {
                _map[gateA.x][gateA.y] = _map[_i][_j] + 1;
                makeDist(_map, gateA.x, gateA.y);
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
            makeDist(_map, _i - 1, _j);
        }
    }
    //verifica o valor da esquerda
    if (_j > 0 && (map[_i][_j - 1] != "x"))
    {
        if (_map[_i][_j - 1] > (_map[_i][_j] + 1))
        {
            _map[_i][_j - 1] = _map[_i][_j] + 1;
            makeDist(_map, _i, _j - 1);
        }
    }
    //verifica o valor de baixo
    if (_i < (linha - 1) && map[_i + 1][_j] != "x")
    {
        if (_map[_i + 1][_j] > (_map[_i][_j] + 1))
        {
            _map[_i + 1][_j] = _map[_i][_j] + 1;
            makeDist(_map, _i + 1, _j);
        }
    }
    //verifica o valor da direita
    if (_j < (coluna - 1) && (map[_i][_j + 1] != "x"))
    {
        if (_map[_i][_j + 1] > (_map[_i][_j] + 1))
        {
            _map[_i][_j + 1] = _map[_i][_j] + 1;
            makeDist(_map, _i, _j + 1);
        }
    }
}

void
Map::printMap()
{
    for (int i = 0; i < linha; ++i)
    {
        for (int j = 0; j < coluna; ++j)
        {
            cout << map[i][j] << " ";
            if (map[i][j].size() < 2)
                cout << " ";
        }

        cout << endl;
    }
}

void
Map::printMapInt(int _map[100][100])
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
    distMap[0][0] = 0;
    makeDist(distMap);

    fUpdate = false;

    //printMapInt(distMap);

    //printMap();
}
