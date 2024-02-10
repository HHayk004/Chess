#include <iostream>
#include "chess.h"

Chess::Chess() : m_player_turn(Color::White), m_board(8, std::vector<Piece*>(8, nullptr))
{
    initializeRow(0, Color::Black, 0);
    initializeRow(1, Color::Black, 1);
    initializeRow(6, Color::White, 1);
    initializeRow(7, Color::White, 0);
}

void Chess::initializeRow(int row_number, Color color, bool mode)
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

Chess::~Chess()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (m_board[i][j])
            {
                delete m_board[i][j];
                m_board[i][j] = nullptr;
            }
        }
    }
}

bool Chess::checkInput(const std::string& move)
{
    auto border_check = [](int coord) { return 0 <= coord && coord <= 7; };
    return move.size() == 4 && 
            border_check(move[0] - 'A') && border_check(move[1] - '1') && 
            border_check(move[2] - 'A') && border_check(move[3] - '1');
}

void Chess::initializeCoordinates(Point& start, Point& end, const std::string& move)
{
    start.x = move[0] - 'A';
    start.y = move[1] - '1';
    end.x = move[2] - 'A';
    end.y = move[3] - '1';
}

void Chess::setPiece(const Point& coord, Piece* const piece)
{
    m_board[coord.x][coord.y] = piece;
}

Piece* Chess::getPiece(const Point& coord) const
{
     return m_board[coord.x][coord.y];
}

bool Chess::checkStart(const Point& start) const
{
    Piece* piece = getPiece(start);
    return piece && piece->m_color == m_player_turn;
}

bool Chess::checkEnd(const Point& end) const
{
    return getPiece(end)->m_color != m_player_turn;
}

void Chess::makeMove() {
    Point start;
    Point end;
    while (true)
    {
        if (m_player_turn == Color::White)
        {
            std::cout << "White's turn: ";
        }

        else
        {
            std::cout << "Black's turn: ";
        }

        std::string move;
        std::getline(std::cin, move);

        if (checkInput(move))
        {
            initializeCoordinates(start, end, move);       
            if (checkStart(start) && checkEnd(end) && m_board[start.x][start.y]->checkMove(end))
            {
                // if eating, update score
                setPiece(end, getPiece(start));
                setPiece(start, nullptr);
                break;
            }
        }
    }

    if (m_player_turn == Color::White)
    {
        m_player_turn = Color::Black;
    }

    else
    {
        m_player_turn = Color::White;
    }
}

void Chess::printBoard() const
{    
    for (int i = 0; i < 8; ++i)
    {
        std::cout << static_cast<char>(i + 'A') << ' ';

        for (int j = 0; j < 8; ++j)
        {
            if (m_board[i][j])
            {
                m_board[i][j]->printPiece();
                std::cout << ' ';
            }

            else
            {
                std::cout << "-- ";
            }
        }

        std::cout << std::endl;
    }

    for (int i = 1; i < 9; ++i)
    {
        std::cout << "  " << i;
    }
    std::cout << std::endl;
}

Piece::Piece(Color color, int value) : m_color(color), m_value(value) {}

char Piece::getColor() const
{
    if (m_color == Color::White)
    {
        return 'W';
    }

    return 'B';
}

Pawn::Pawn(Color color) : Piece(color, 1) {}
void Pawn::printPiece() const
{
    std::cout << getColor() << 'P';
}

Knight::Knight(Color color) :  Piece(color, 3) {}
void Knight::printPiece() const
{
    std::cout << getColor() << 'N';
}

Bishop::Bishop(Color color) : Piece(color, 3) {}
void Bishop::printPiece() const
{
    std::cout << getColor() << 'B';
}

Rook::Rook(Color color) : Piece(color, 5) {}
void Rook::printPiece() const
{
    std::cout << getColor() << 'R';
}

Queen::Queen(Color color) : Piece(color, 9) {}
void Queen::printPiece() const
{
    std::cout << getColor() << 'Q';
}

King::King(Color color) : Piece(color, 0) {}
void King::printPiece() const
{
    std::cout << getColor() << 'K';
}


