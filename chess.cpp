#include "chess.h"

int main()
{
    Chess game;
    while (true)
    {
        game.printBoard();
        game.makeMove();
    }

    return 0;
}
