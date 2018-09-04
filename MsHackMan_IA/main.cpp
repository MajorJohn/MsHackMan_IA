#include <iostream>
#include "NextCommand.h"


int main() {

    NextCommand nextCommand;

    while (true) {
        nextCommand.process();
    }

    return 0;
}