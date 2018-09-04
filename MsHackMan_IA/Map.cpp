#include "Map.h"

Map::Map(int _width, int _height)
    : coluna(_width), linha(_height)
{
    string sMap = ".,.,.,x,.,.,.,.,.,.,.,.,.,.,.,x,.,.,.,.,x,.,x,.,x,x,x,x,.,x,x,x,x,.,x,.,x,.,.,x,.,.,.,x,.,.,.,.,.,.,.,x,.,.,.,x,.,.,x,x,x,.,x,.,x,x,x,x,x,.,x,.,x,x,x,.,.,x,.,.,.,x,.,.,.,.,.,.,.,x,.,.,.,x,.,.,.,.,x,.,x,.,x,x,.,x,x,.,x,.,x,.,.,.,x,.,x,x,.,.,.,x,x,.,x,x,.,.,.,x,x,.,x,G,.,x,x,.,x,x,x,x,.,x,x,x,x,.,x,x,.,G,x,.,x,x,.,.,.,.,.,.,.,.,.,.,.,x,x,.,x,.,.,.,x,.,x,x,x,x,x,x,x,x,x,.,x,.,.,.,.,x,.,.,.,.,.,.,x,x,x,.,.,.,.,.,.,x,.,.,x,.,x,x,.,x,.,.,.,.,.,x,.,x,x,.,x,.,.,x,.,x,x,.,x,x,x,x,x,x,x,.,x,x,.,x,.,.,x,.,x,x,.,x,.,.,.,.,.,x,.,x,x,.,x,.,.,.,.,.,.,.,.,.,x,x,x,.,.,.,.,.,.,.,.";

    int s = 0;
    for (int i = 0; i < linha; ++i)
    {
        for (int j = 0; j < coluna; ++j)
        {
            if (sMap[s] == ',')
                s++;
            map[i][j] = sMap[s];
            s++;

        }
    }

    findGates();

    for (int i = 0; i < linha; ++i)
    {
        for (int j = 0; j < coluna; ++j)
        {
            mapBad[i][j] = mapGood[i][j] = 1000;
        }
    }
    mapBad[0][0] = mapGood[0][0] = 0;

    //makeDist(mapBad, false);
    makeDist(mapGood, true);

    //printMapInt(mapGood);

    //printMap();

}

Map::~Map()
{
}

void
Map::findGates()
{
    bool a = true;
    bool b = true;

    for (int i = 0; i < linha; ++i)
    {
        for (int j = 0; j < coluna; ++j)
        {
            if (map[i][j] == "G")
            {
                if (a)
                {
                    gateA.x = i;
                    gateA.y = j;
                    a = false;
                }
                else
                {
                    gateB.x = i;
                    gateB.y = j;
                    b = false;
                }

            }
        }
    }

}

void
Map::updateMap(string _field)
{
    int s = 0;
    for (int i = 0; i < linha; ++i)
    {
        for (int j = 0; j < coluna; ++j)
        {
            if (_field[s] == ',')
                s++;
            map[i][j] = _field[s];
            s++;

        }
    }

    //printMap();
}

//verifica todos os lados (up, left, down n right)
//dessa maneira, dá valor para cada casa possivel de andar ("+1 p/ cada larço")
//params:
//  _map[][] = mapa para que será editado (good ou bad)
//  isGood = se o mapa a ser verificado é de coisas boas (snippers e bomba para pegar)
//           afeta principalmente no gate, pois o menor caminho atravessa o gate
//  _x e _y = posição atual
void
Map::makeDist(int _map[15][19], bool isGood, int _i, int _j)
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
    if (map[_i][_j] == "G" && isGood)
    {
        //caso estejamos no portal A
        if (_j == gateA.y)
        {
            if (_map[gateB.x][gateB.y] > (_map[_i][_j] + 1))
            {
                _map[gateB.x][gateB.y] = _map[_i][_j] + 1;
                makeDist(_map, isGood, gateB.x, gateB.y);
            }
        }
        else
        {
            if (_map[gateA.x][gateA.y] > (_map[_i][_j] + 1))
            {
                _map[gateA.x][gateA.y] = _map[_i][_j] + 1;
                makeDist(_map, isGood, gateA.x, gateA.y);
            }
        }
    }
    
    //verifica o valor de cima
    if (_i > 0 && map[_i - 1][_j] == ".")
    {
        //verifica se o valor de cima é maior que o valor atual +1
        //se for, coloca o valor atual mais no lugar e chama a recursão
        //para o novo ponto
        if (_map[_i - 1][_j] > (_map[_i][_j] + 1))
        {
            _map[_i - 1][_j] = _map[_i][_j] + 1;
            makeDist(_map, isGood, _i - 1, _j);
        }
    }
    //verifica o valor da esquerda
    if (_j > 0 && (map[_i][_j - 1] == "." || map[_i][_j - 1] == "G"))
    {
        if (_map[_i][_j - 1] > (_map[_i][_j] + 1))
        {
            _map[_i][_j - 1] = _map[_i][_j] + 1;
            makeDist(_map, isGood, _i, _j - 1);
        }
    }
    //verifica o valor de baixo
    if (_i < (linha - 1) && map[_i + 1][_j] == ".")
    {
        if (_map[_i + 1][_j] > (_map[_i][_j] + 1))
        {
            _map[_i + 1][_j] = _map[_i][_j] + 1;
            makeDist(_map, isGood, _i + 1, _j);
        }
    }
    //verifica o valor da direita
    if (_j < (coluna - 1) && (map[_i][_j + 1] == "." || map[_i][_j + 1] == "G"))
    {
        if (_map[_i][_j + 1] > (_map[_i][_j] + 1))
        {
            _map[_i][_j + 1] = _map[_i][_j] + 1;
            makeDist(_map, isGood, _i, _j + 1);
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
        }

        cout << endl;
    }
}

void
Map::printMapInt(int _map[15][19])
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

