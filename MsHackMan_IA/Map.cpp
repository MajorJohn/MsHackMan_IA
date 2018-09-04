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

    makeDist(mapGood, true, 2, 14);

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
Map::makeDist(int _map[15][19], bool isGood, int _x, int _y)
{
    //verifica o valor de cima
    if (_y > 0 && map[_x][_y - 1] == ".")
    {
        //verifica se o valor de cima é maior que o valor atual +1
        //se for, coloca o valor atual mais no lugar e chama a recursão
        //para o novo ponto
        cout << "caminho pra cima\n";
    }
    //verifica o valor da esquerda
    if (_x > 0 && map[_x - 1][_y] == ".")
    {
        cout << "caminho para a esquerda\n";
    }
    //verifica o valor de baixo
    if (_y < (linha - 1) && map[_x][_y + 1] == ".")
    {
        cout << "caminho para baixo\n";
    }
    //verifica o valor da direita
    if (_x < (coluna - 1) && map[_x + 1][_y] == ".")
    {
        cout << "caminho para direita\n";
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
            cout << map[i][j] << " ";
        }

        cout << endl;
    }
}

