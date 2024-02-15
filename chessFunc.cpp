#include <iostream>
#include <cmath>
#include "chess.h"

Chess::Chess() : m_player_turn(FigureColor::White),
    m_white({7, 4}), m_black({0, 4}), 
    m_board(8, std::vector<Piece*>(8, nullptr)), m_current_move_type(MoveType::TypeNone)
{
    initializeRow(0, FigureColor::Black, 0);
    initializeRow(1, FigureColor::Black, 1);
    initializeRow(6, FigureColor::White, 1);
    initializeRow(7, FigureColor::White, 0);
}

void Chess::initializeRow(int row_number, FigureColor color, bool mode)
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
    return move.size() == 4 && 
            borderCheck(move[0] - 'a') && borderCheck(move[1] - '1') && 
            borderCheck(move[2] - 'a') && borderCheck(move[3] - '1');
}

void Chess::initializeCoordinates(Point& start, Point& end, const std::string& move)
{
    start.x = move[1] - '1';
    start.y = move[0] - 'a';
    end.x = move[3] - '1';
    end.y = move[2] - 'a';
}

void Chess::setPiece(const Point& coord, Piece* const piece)
{
    if (piece && getPiece(coord))
    {
        delete getPiece(coord);
    }
    
    m_board[coord.x][coord.y] = piece;
}

Piece* Chess::getPiece(const Point& coord) const
{
     return m_board[coord.x][coord.y];
}

void Chess::setMoveType(MoveType type)
{
    m_current_move_type = type;
}

bool Chess::borderCheck(int coord)
{ 
    return 0 <= coord && coord < 8; 
};

bool Chess::checkStart(const Point& start) const
{
    Piece* piece = getPiece(start);
    return piece && piece->m_color == m_player_turn;
}

bool Chess::checkEnd(const Point& end) const
{
    Piece* piece = getPiece(end);
    return (!piece) || piece->m_color != m_player_turn;
}

Point Chess::direction(int delta_x, int delta_y) const
{
    int dl_x = 0;
    int dl_y = 0;

    if (delta_x > 0)
    {
        dl_x = 1;
    }

    else if (delta_x < 0)
    {
        dl_x = -1;
    }

    if (delta_y > 0)
    {
        dl_y = 1;
    }

    else if (delta_y < 0)
    {
        dl_y = -1;
    }

    return {dl_x, dl_y};
}

bool Chess::checkMoveLinear(const Point& start, const Point& end) const
{
    int delta_x = end.x - start.x;
    int delta_y = end.y - start.y;

    Point dir = direction(delta_x, delta_y);

    if (dir.x == 0 || dir.y == 0)
    {
        return !pieceInLine(start, end, dir);
    }

    return false;
}

bool Chess::checkMoveDiagonal(const Point& start, const Point& end) const
{
    int delta_x = end.x - start.x;
    int delta_y = end.y - start.y;

    if (abs(delta_x) == abs(delta_y))
    {
        Point dir = direction(delta_x, delta_y);
        return !pieceInLine(start, end, dir);
    }

    return false;
}

Piece* Chess::pieceInLine(Point start, Point end, const Point& delta) const
{
    while (start != end)
    {
        start.x += delta.x;
        start.y += delta.y;

        Piece* piece = getPiece(start);
        if (piece)
        {
            return piece;
        }
    }

    return nullptr;
}

bool Chess::isCheck() const
{
    return true;
}

void Chess::movePiece(const Point& start, const Point& end)
{
    Piece* piece = getPiece(end);
    
    if (piece)
    {
        delete piece;
    }

    setPiece(end, getPiece(start));
    setPiece(start, nullptr);

    switch (m_current_move_type)
    {
        case MoveType::TypePassant:
            {
                int delta_x = 1;
                if (getPlayerType() == FigureColor::Black)
                {
                    delta_x = -1;
                }

                setPiece({end.x + delta_x, end.y}, nullptr); 
                break;
            }
    
        case MoveType::TypeRook:
            {
                Rook* rook = dynamic_cast<Rook*>(getPiece(end));
                rook->setCastleAvailable(false);
                break;
            }

        case MoveType::TypeKing:
            {
                King* king = dynamic_cast<King*>(getPiece(end));
                king->setCastleAvailable(false);
                break;
            }

        case MoveType::TypeCastleLeft:
            setPiece({start.x, 3}, getPiece({start.x, 0}));
            setPiece({start.x, 0}, nullptr);  
            break;

        case MoveType::TypeCastleRight:
            setPiece({start.x, 5}, getPiece({start.x, 7}));
            setPiece({start.x, 7}, nullptr);  
            break;
    }

}

void Chess::makeMove() {
    Point start;
    Point end;
    while (true)
    {
        if (m_player_turn == FigureColor::White)
        {
            std::cout << "White's turn: ";
        }

        else
        {
            std::cout << "Black's turn: ";
        }

        std::string move;
        std::getline(std::cin, move);

        std::cout << std::endl;

        if (checkInput(move))
        {
            initializeCoordinates(start, end, move);       
            if (checkStart(start) && checkEnd(end) && 
                m_board[start.x][start.y]->checkMove(this, start, end) && isCheck())
            {
                // if eating, update score
                movePiece(start, end);
                getPlayer(m_player_turn).setMove(start, end);
                m_current_move_type = MoveType::TypeNone;
                break;
            }

            m_current_move_type = MoveType::TypeNone;
        }
    }

    if (m_player_turn == FigureColor::White)
    {
        m_player_turn = FigureColor::Black;
    }

    else
    {
        m_player_turn = FigureColor::White;
    }
}

FigureColor Chess::getPlayerType() const
{
    return m_player_turn;
}

Player& Chess::getPlayer(FigureColor color)
{
    if (color == FigureColor::White)
    {
        return m_white;
    }

    return m_black;
}

bool Chess::checkCastle(const Point& start, const Point& end) const
{
    checkMoveLinear(start, end);

    King* king = dynamic_cast<King*>(getPiece(start));
    Rook* rook = dynamic_cast<Rook*>(getPiece(end));
    if (king && king->getCastleAvailable() &&
        rook && rook->getCastleAvailable())
    {
        if (start.y > end.y)
        {
            m_current_move_type = MoveType::TypeCastleLeft;
        }

        else
        {
            m_current_move_type = MoveType::TypeCastleRight;
        }
     
        return true;
    }

    return false;
}

void Chess::printBoard() const
{    
    for (int i = 0; i < 8; ++i)
    {
        std::cout << i + 1 << ' ';

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

    for (int i = 0; i < 8; ++i)
    {
        std::cout << "  " << static_cast<char>(i + 'a');
    }
    std::cout << "\n\n";
}

Player::Player(const Point& king_position) : m_king_position(king_position) {} 

void Player::setMove(const Point& start, const Point& end)
{
    m_last_move_start = start;
    m_last_move_end = end;
}

Point Player::getMoveStart() const
{
    return m_last_move_start;
}

Point Player::getMoveEnd() const
{
    return m_last_move_end;
}

Piece::Piece(FigureType type, FigureColor color, int value) : m_type(type), m_color(color), m_value(value) {}

char Piece::getFigureColor() const
{
    if (m_color == FigureColor::White)
    {
        return 'W';
    }

    return 'B';
}

char Piece::getFigureType() const
{
    switch (m_type) {
        case FigureType::Pawn:
            return 'P';

        case FigureType::Knight:
            return 'N';

        case FigureType::Bishop:
            return 'B';
    
        case FigureType::Rook:
            return 'R';
    
        case FigureType::Queen:
            return 'Q';
    
        case FigureType::King:
            return 'K';
    }

    return '\0'; // for no warnings
}

Pawn::Pawn(FigureColor color) : Piece(FigureType::Pawn, color, 1) , m_double_move(0) {}

void Pawn::setDoubleMove(bool value)
{
    m_double_move = value;
}

bool Pawn::getDoubleMove() const
{
    return m_double_move;
}

bool Pawn::checkMove(Chess* const game, const Point& start, const Point& end) const
{
    int delta_x = end.x - start.x;
    int delta_y = end.y - start.y;

    if (delta_y == 0)
    {
        if (getFigureColor() == 'W')
        {
            if (delta_x == -1)
            {
                return true;
            }

            if (delta_x == -2 && start.x == 6)
            {
                if (!game->getPiece({start.x - 1, start.y}))
                {
                    return true;
                }       
            }

            return false;
        }// else

        if (delta_x == 1)
        {
            return true;
        }

        if (delta_x == 2 && start.x == 1)
        {
            if (!game->getPiece({start.x + 1, start.y}))
            {
                return true;
            }
        }

        return false;
    }

    if (((delta_x == 1 && getFigureColor() == 'B') || 
            (delta_x == -1 && getFigureColor() == 'W')) && abs(delta_y) == 1)
    {
        if (game->getPiece(end))
        {
            return true;
        }

        Point side = {start.x, start.y + delta_y};
        Pawn* pawn = dynamic_cast<Pawn*>(game->getPiece(side));
        if (pawn)
        {
            FigureColor color = FigureColor::White;
            if (game->getPlayerType() != FigureColor::Black)
            {
                color = FigureColor::Black;
            }

            Player player = game->getPlayer(color);
            Point last_move_start = player.getMoveStart();
            Point last_move_end = player.getMoveEnd();

            Point must_start = {end.x + delta_x, end.y};

            if (last_move_start == must_start && last_move_end == side)
            {
                game->setMoveType(MoveType::TypePassant);
                return true;
            }
        }
    }

    return false;
}

void Pawn::printPiece() const
{
    std::cout << getFigureColor() << getFigureType();
}

Knight::Knight(FigureColor color) : Piece(FigureType::Knight, color, 3) {}

void Knight::printPiece() const
{
    std::cout << getFigureColor() << getFigureType();
}

bool Knight::checkMove(Chess* const game, const Point& start, const Point& end) const
{
    int delta_x = abs(end.x - start.x);
    int delta_y = abs(end.y - start.y);
    return (delta_x == 2 && delta_y == 1) || 
        (delta_x == 1 && delta_y == 2);
}

Bishop::Bishop(FigureColor color) : Piece(FigureType::Bishop, color, 3) {}

bool Bishop::checkMove(Chess* const game, const Point& start, const Point& end) const
{
    return game->checkMoveDiagonal(start, end);
}

void Bishop::printPiece() const
{
    std::cout << getFigureColor() << getFigureType();
}

Rook::Rook(FigureColor color) : Piece(FigureType::Rook, color, 5), m_castle_available(true) {}

void Rook::setCastleAvailable(bool value)
{
    m_castle_available = value;
}

bool Rook::getCastleAvailable() const
{
    return m_castle_available;
}

bool Rook::checkMove(Chess* const game, const Point& start, const Point& end) const
{
    if (game->checkMoveLinear(start, end))
    {
        game->setMoveType(MoveType::TypeRook);
        return true;
    }

    return false;
}

void Rook::printPiece() const
{
    std::cout << getFigureColor() << getFigureType();
}

Queen::Queen(FigureColor color) : Piece(FigureType::Queen, color, 9) {}

bool Queen::checkMove(Chess* const game, const Point& start, const Point& end) const
{
    return game->checkMoveLinear(start, end) || game->checkMoveDiagonal(start, end);
}

void Queen::printPiece() const
{
    std::cout << getFigureColor() << getFigureType();
}

King::King(FigureColor color) : Piece(FigureType::King, color, 0), m_castle_available(true) {}

void King::setCastleAvailable(bool value)
{
    m_castle_available = value;
}

bool King::getCastleAvailable() const
{
    return m_castle_available;
}

bool King::checkMove(Chess* const game, const Point& start, const Point& end) const
{
    int delta_x = end.x - start.x;
    int delta_y = end.y - start.y;
    
    if (abs(delta_x) <= 1 && abs(delta_y) <= 1)
    {
        game->setMoveType(MoveType::TypeKing);
        return true;
    }

    if (delta_y == -2)
    {
        return game->checkCastle(start, {end.x, end.y - 2});
    }

    if (delta_y == 2)
    {
        return game->checkCastle(start, {end.x, end.y + 1});
    }

    return false;
}

void King::printPiece() const
{
    std::cout << getFigureColor() << getFigureType();
}

