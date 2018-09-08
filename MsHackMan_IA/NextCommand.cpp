#include "nextCommand.h"


NextCommand::NextCommand()
{
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
    if (boy)
        cout << "bixie" << endl;
    else
        cout << "bixiette" << endl;
}

void
NextCommand::doMove()
{
    float rd = rand() % 4;
}