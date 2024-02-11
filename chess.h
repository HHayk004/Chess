#ifndef CHESS_H
#define CHESS_H

#include <vector>
#include <string>

struct Point {
    int x;
    int y;

    bool operator==(const Point& p2) const
    {
        return x == p2.x && y == p2.y;
    }
    
    bool operator!=(const Point& p2) const
    {
        return x != p2.x && y != p2.y;
    }
};

enum class FigureColor {
    White = 0, Black = 1
};

enum class FigureType {
    Pawn, Knight, Bishop, Rook, Queen, King
};

class Chess;

class Piece {
    private:
        FigureType m_type;
        FigureColor m_color; // 0 white, 1 black
        int m_value; // value of piece Queen 9

        virtual bool checkMove(const Chess* const game, const Point& start, const Point& end) const = 0;
    
    public:
        Piece(FigureType type, FigureColor color, int value);
        virtual ~Piece() = default;

        virtual void printPiece() const = 0;

        char getFigureColor() const;
        char getFigureType() const;

        friend class Chess;
};

class Pawn : public Piece {
    private:
        bool checkMove(const Chess* const game, const Point& start, const Point& end) const override; 

    public:
        Pawn(FigureColor color);
        ~Pawn() override = default;

        void printPiece() const override;
};

class Knight : public Piece {
    private:
        bool checkMove(const Chess* const game, const Point& start, const Point& end) const override;

    public:
        Knight(FigureColor color);
        ~Knight() override = default;

        void printPiece() const override;
};

class Bishop : public Piece {
    private:
        bool checkMove(const Chess* const game, const Point& start, const Point& end) const override;

    public:
        Bishop(FigureColor color);
        ~Bishop() override = default;
       
        void printPiece() const override;
};

class Rook : public Piece {
    private:
        bool m_castle_available;
        bool checkMove(const Chess* const game, const Point& start, const Point& end) const override;

    public:
        Rook(FigureColor color);
        ~Rook() override = default;

        void printPiece() const override;
};

class Queen : public Piece {
    private:
        bool checkMove(const Chess* const game, const Point& start, const Point& end) const override;

    public:
        Queen(FigureColor color);
        ~Queen() override = default;
        
        void printPiece() const override;
};

class King : public Piece {
    private:
        bool m_castle_available;

        bool checkMove(const Chess* const game, const Point& start, const Point& end) const override;
        
        bool checkShortCastle() const;
        bool checkLongCastle() const;

    public: 
        King(FigureColor color);
        ~King() override = default;

        void printPiece() const override;
};

class Player {
    private:
        Point m_king_position;

    public:
        Player(const Point& king_position);
        ~Player() = default;
};

class Chess {
    private:
        FigureColor m_player_turn;
        Player m_white;
        Player m_black;    
 
        std::vector<std::vector<Piece*>> m_board;

        void setPiece(const Point& coord, Piece* const piece);
        Piece* getPiece(const Point& coord) const;

        void initializeRow(int row_number, FigureColor color, bool mode);

        static bool checkInput(const std::string& move);
        static void initializeCoordinates(Point& start, Point& end, const std::string& move);

        static bool borderCheck(int n);

        bool checkStart(const Point& start) const;
        bool checkEnd(const Point& end) const;

        Point direction(int delta_x, int delta_y) const; 

        Piece* pieceInLine(Point start, Point end, const Point& delta) const;

        bool isCheck() const;
        bool checkGameOver() const;

    public:
        Chess();
        ~Chess();

        void makeMove();

        bool checkMoveLinear(const Point& start, const Point& end) const;
        bool checkMoveDiagonal(const Point& start, const Point& end) const;

        void printBoard() const;

        friend class Piece;
};

#endif
