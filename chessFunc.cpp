#include "chess.h"
#include <iostream>

Board::Board() : m_board(8, std::vector<Piece*>(8, nullptr))
{
    initializeRow(0, 1, 1);
    initializeRow(1, 1, 0);
    initializeRow(6, 0, 0);
    initializeRow(7, 0, 1);
}

void Board::initializeRow(int row_number, bool color, bool mode)
{
    if (mode)
    {
        for (int i = 0; i < 8; ++i)
        {
            m_board[row_number][i] = new Pawn(color);
        }
    }

    else
    {
        m_board[row_number][0] = new Rook(color);
        m_board[row_number][1] = new Knight(color);
        m_board[row_number][2] = new Bishop(color);
        m_board[row_number][3] = new Queen(color);
        m_board[row_number][4] = new King(color);
        m_board[row_number][5] = new Bishop(color);
        m_board[row_number][6] = new Knight(color);
        m_board[row_number][7] = new Rook(color);
    }
}

Board::~Board()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (m_board[i][j])
            {
                delete m_board[i][j];
            }
        }
    }
}

void Board::printBoard() const
{
    for (int i = 0; i < 8; ++i)
    {
        std::cout << '|';

        for (int j = 0; j < 8; ++j)
        {
            if (m_board[i][j])
            {
                m_board[i][j]->printPiece();
                std::cout << '|';
            }

            else
            {
                std::cout << "  |";
            }
        }

        std::cout << std::endl;
    }
}

Piece::Piece(bool color, int value) : m_color(color), m_value(value) {}

char Piece::getColor() const
{
    if (m_color)
    {
        return 'B';
    }

    return 'W';
}

Pawn::Pawn(bool color) : Piece(color, 1) {}
void Pawn::printPiece() const
{
    std::cout << getColor() << 'P';
}

Knight::Knight(bool color) :  Piece(color, 3) {}
void Knight::printPiece() const
{
    std::cout << getColor() << 'N';
}

Bishop::Bishop(bool color) : Piece(color, 3) {}
void Bishop::printPiece() const
{
    std::cout << getColor() << 'B';
}

Rook::Rook(bool color) : Piece(color, 5) {}
void Rook::printPiece() const
{
    std::cout << getColor() << 'R';
}

Queen::Queen(bool color) : Piece(color, 9) {}
void Queen::printPiece() const
{
    std::cout << getColor() << 'Q';
}

King::King(bool color) : Piece(color, 0) {}
void King::printPiece() const
{
    std::cout << getColor() << 'K';
}

