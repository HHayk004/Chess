#include "chess.h"
#include <cstdlib>

int main()
{
    std::system("clear");
    Chess game;
    while (true)
    {
        game.printBoard();
        game.makeMove();
        std::system("clear");
    }

    return 0;
}
