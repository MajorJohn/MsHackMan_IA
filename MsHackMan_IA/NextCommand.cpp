#include "nextCommand.h"


NextCommand::NextCommand()
{    
    inflZero();
}


NextCommand::~NextCommand()
{
}

void
NextCommand::process()
{
    string command;
    std::cin >> command;
    if (command == "settings") {
        settings();
    }
    else if (command == "update") {
        update();
    }
    else if (command == "action") {
        action();
    }
}

void
NextCommand::settings()
{
    string type;
    cin >> type;
    if (type == "timebank") {
        cin >> timebank;
    }
    else if (type == "time_per_move") {
        cin >> time_per_move;
    }
    else if (type == "player_names") {
        string names;
        cin >> names;
    }
    else if (type == "your_bot") {
        cin >> me.name;
    }
    else if (type == "your_botid") {
        cin >> me.id;
    }
    else if (type == "field_width") {
        cin >> width;
    }
    else if (type == "field_height") {
        cin >> height;
    }
    else if (type == "max_rounds") {
        cin >> max_rounds;
    }

}

void
NextCommand::update()
{
    if (firstUpdate)
    {
        map = new Map(width, height);
        firstUpdate = false;
        map->setPlayerID(me.id);
        //map->getMap(sMap);
        //useBomb();
    }

    string player_name, type, field;
    cin >> player_name >> type;
    if (type == "round") {
        cin >> current_round;
    }
    else if (type == "field") {
        cin >> field;
        map->updateMap(field);
    }


    else if (type == "snippets") {
        if (player_name == me.name) {
            cin >> me.snippets;
        }
        else {
            cin >> enemy.snippets;
        }
    }
    else if (type == "bombs") {
        if (player_name == me.name) {
            cin >> me.bombs;
        }
        else {
            cin >> enemy.bombs;
        }
    }
}

void
NextCommand::action()
{
    string action;
    cin >> action;
    if (action == "character") {
        cin >> time_remaining;
        chooseCharacter();
    }
    else if (action == "move") {
        cin >> time_remaining;
        doMove();
    }
}

void
NextCommand::chooseCharacter()
{
    float rd = rand() % 2;
    if (rd < 1)
        cout << "bixie" << endl;
    else
        cout << "bixiette" << endl;
}

void
NextCommand::doMove()
{
    sendoPerseguido = false;
    snippets.clear();
    bugs.clear();
    bombs.clear();

    map->getPlayerPos(me.point);
    map->getSnippets(snippets);
    map->getBugs(bugs);
    map->getBombs(bombs);


    up = 1000000;
    left = 1000000;
    down = 1000000;
    right = 1000000;

    bUp = 1000000;
    bLeft = 1000000;
    bDown = 1000000;
    bRight = 1000000;

    gUp = 1000000;
    gLeft = 1000000;
    gDown = 1000000;
    gRight = 1000000;
    
    if (map->canGo(me.point.x - 1, me.point.y))
    {
        canUp = true;
        bUp = gUp = up = 0;
    }
    if (map->canGo(me.point.x, me.point.y - 1))
    {
        canLeft = true;
        bLeft = gLeft = left = 0;
    }
    if (map->canGo(me.point.x + 1, me.point.y))
    {
        canDown = true;
        bDown = gDown = down = 0;
    }
    if (map->canGo(me.point.x, me.point.y + 1))
    {
        canRight = true;
        bRight = gRight = right = 0;
    }
    

    //identificar qual o snippet mais próximo
    Point snippetProximo;
    int maisProximo = 1000;
    bool achouSnippet = false;

    for (int i = 0; i < snippets.size(); i++)
    {
        int dist = map->distAB(me.point, snippets[i]);
        if (maisProximo > dist)
        {
            achouSnippet = true;
            maisProximo = dist;
            snippetProximo.x = snippets[i].x;
            snippetProximo.y = snippets[i].y;
        }
    }
    
    
    //cerifica o melhor caminho nas quatro direções
    if (achouSnippet)
    {
        if(canUp)
            gUp = 95 - map->distAB(snippetProximo, me.point.x - 1, me.point.y);
        if(canLeft)
            gLeft = 95 - map->distAB(snippetProximo, me.point.x, me.point.y - 1);
        if(canDown)
            gDown = 95 - map->distAB(snippetProximo, me.point.x + 1, me.point.y);
        if(canRight)
            gRight = 95 - map->distAB(snippetProximo, me.point.x, me.point.y + 1);
    }

    //identificar bomba
    Point bombProximo;
    int bombMaisProximo = 1000;
    bool achoubomb = false;

    for (int i = 0; i < bombs.size(); i++)
    {
        if (bombs[i].type == 10)
        {
            int dist = map->distAB(me.point, bombs[i]);
            if (bombMaisProximo > dist)
            {
                achoubomb = true;
                bombMaisProximo = dist;
                bombProximo.x = bombs[i].x;
                bombProximo.y = bombs[i].y;
            }
        }
    }

    //melhor caminho pras bombas
    if (achoubomb)
    { 
        int tmp;
        if (canUp)
        {
            tmp = 96 - map->distAB(bombProximo, me.point.x - 1, me.point.y);
            if(gUp < tmp)
                gUp = tmp;
        }
        if (canLeft)
        {
            tmp = 96 - map->distAB(bombProximo, me.point.x, me.point.y - 1);
            if (gLeft < tmp)
                gLeft = tmp;
        }
        if (canDown)
        {
            tmp = 96 - map->distAB(bombProximo, me.point.x + 1, me.point.y);
            if (gDown < tmp)
                gDown = tmp;
        }
        if (canRight)
        {
            tmp = 96 - map->distAB(bombProximo, me.point.x, me.point.y + 1);
            if (gRight < tmp)
                gRight = tmp;
        }
    }

    cout << gUp << " " << gLeft << " " << gDown << " " << gRight << endl;

    //identifica bugs
    Point bugProximo;
    int bugMaisProximo = 1000;
    bool achouBug = false;

    for (int i = 0; i < bugs.size(); i++)
    {
        int dist = map->distAB(me.point, bugs[i], false);
        if (bugMaisProximo > dist)
        {
            achouBug = true;
            bugMaisProximo = dist;
            bugProximo.x = bugs[i].x;
            bugProximo.y = bugs[i].y;
        }
    }

    if (bugMaisProximo < 5)
        sendoPerseguido = true;

    if (bugMaisProximo < 3)
        useBomb();

    if(achouBug)
    {
        if(canUp)
            bUp = 100 - map->distAB(bugProximo, me.point.x - 1, me.point.y, false);
        if(canLeft)
            bLeft = 100 - map->distAB(bugProximo, me.point.x, me.point.y - 1, false);
        if(canDown)
            bDown = 100 - map->distAB(bugProximo, me.point.x + 1, me.point.y, false);
        if(canRight)
            bRight = 100 - map->distAB(bugProximo, me.point.x, me.point.y + 1, false);
    }
    //cout << bUp << " " << bLeft << " " << bDown << " " << bRight << endl;

    if (canUp)
    {
        if (bUp >= gUp && sendoPerseguido)
            up = bUp;
        else
            up = -gUp;
    }
    if (canLeft)
    {
        if (bLeft >= gLeft && sendoPerseguido)
            left = bLeft;
        else
            left = -gLeft;
    }
    if (canDown)
    {
        if (bDown >= gDown && sendoPerseguido)
            down = bDown;
        else
            down = -gDown;
    }
    if (canRight)
    {
        if (bRight >= gRight && sendoPerseguido)
            right = bRight;
        else
            right = -gRight;
    }

    caseIqual();

    cout << "Trono up: " << up << endl << "Trono left: " << left << endl << "Trono down: " << down << endl << "Trono right: " << right << endl;

    //********* Decidir o movimento *************
    //caso Up
    if (up < left && up < down && up < right)
        cout << "up\n";
    else if (left < down && left < right)
        cout << "left\n";
    else if (down < right)
        cout << "down\n";
    else
        cout << "right\n";

    inflZero();
}

void
NextCommand::useBomb()
{
    int coluna = width;
    int linha = height;

    for (int i = 0; i < linha; ++i)
    {
        for (int j = 0; j < coluna; ++j)
        {
            cout << sMap[i][j] << " ";
        }

        cout << endl;
    }
}

void
NextCommand::caseIqual()
{
    srand(time(NULL));
    float tmp;
    if (up <= left && up <= down && up <= right)
    {
        if (up == left && canUp)
        {
            if (up > 0)
            {
                up -= gUp;
                left -= gLeft;
            }
            else
            {
                up += bUp; 
                left += bLeft;
            }

            if (up == left)
            {
                tmp = rand() % 2;       
                if(tmp < 1)
                    up -= 1;
                else
                    left -= 1;
            }

            up -= 200;
            left -= 200;        
        }
        if (up == down && canUp)
        {
            if (up > 0)
            {
                up -= gUp;
                down -= gDown;
            }
            else
            {
                up += bUp;
                down += bDown;
            }

            if (up == down)
            {
                tmp = rand() % 2;
                if (tmp < 1)
                    up -= 1;
                else
                    down -= 1;
            }
            up -= 200;
            down -= 200;
        }
        if (up == right && canUp)
        {
            if (up > 0)
            {
                up -= gUp;
                right -= gRight;
            }
            else
            {
                up += bUp;
                right += bRight;
            }
            if (up == right)
            {
                tmp = rand() % 2;
                if (tmp < 1)
                    up -= 1;
                else
                    right -= 1;
            }
            up -= 200;
            right -= 200;
        }
    }

    if (left <= up && left <= down && left <= right)
    {
        if (left == down && canLeft)
        {
            if (left > 0)
            {
                left -= gLeft;
                down -= gDown;
            }
            else
            {
                left += bLeft;
                down += bDown;
            }

            if (left == down)
            {
                tmp = rand() % 2;
                if (tmp < 1)
                    left -= 1;
                else
                    down -= 1;
            }

            left -= 200;
            down -= 200;
        }
        if (left == right && canLeft)
        {
            if (left > 0)
            {
                left -= gLeft;
                right -= gRight;
            }
            else
            {
                left += bLeft;
                right += bRight;
            }

            if (left == right)
            {
                tmp = rand() % 2;
                if (tmp < 1)
                    left -= 1;
                else
                    right -= 1;
            }

            left -= 200;
            right -= 200;
        }
    }

    if (down <= up && down <= left && down <= right)
    {
        if (down == right && canDown)
        {
            if (down > 0)
            {
                down -= gDown;
                right -= gRight;
            }
            else
            {
                down += bDown;
                right += bRight;
            }

            if (down == right)
            {
                tmp = rand() % 2;
                if (tmp < 1)
                    down -= 1;
                else
                    right -= 1;
            }
            down -= 200;
            right -= 200;
        }
    }
}

void
NextCommand::inflZero()
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 50; ++j)
        {
            influencias[i][j].snippet = 0;
        }
    }
}