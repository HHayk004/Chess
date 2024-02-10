#ifndef CHESS_H
#define CHESS_H

#include <vector>
#include <string>

struct Point {
    int x;
    int y;
};

enum class Color {
    White = 0, Black = 1
};

class Piece {
    private:
        Color m_color; // 0 white, 1 black
        int m_value; // value of piece Queen 9

        virtual bool checkMove(Point end) const = 0;
    
    public:
        Piece(Color color, int value);
        virtual ~Piece() = default;

        virtual void printPiece() const = 0;

        char getColor() const;

        friend class Chess;
};

class Pawn : public Piece {
    private:
        bool checkMove(Point end) const override 
        {return true;}

    public:
        Pawn(Color color);
        ~Pawn() override = default;

        void printPiece() const override;
};

class Knight : public Piece {
    private:
        bool checkMove(Point end) const override
        {return true;}
        
    public:
        Knight(Color color);
        ~Knight() override = default;

        void printPiece() const override;
};

class Bishop : public Piece {
    private:
        bool checkMove(Point end) const override
        {return true;}

    public:
        Bishop(Color color);
        ~Bishop() override = default;
       
        void printPiece() const override;
};

class Rook : public Piece {
    private:
        bool m_castle_available;
        bool checkMove(Point end) const override
        {return true;}

    public:
        Rook(Color color);
        ~Rook() override = default;
        
        void printPiece() const override;
};

class Queen : public Piece {
    private:
        bool checkMove(Point end) const override
        {return true;}

    public:
        Queen(Color color);
        ~Queen() override = default;
        
        void printPiece() const override;
};

class King : public Piece {
    private:
        bool m_castle_available;

        bool checkMove(Point end) const override
        {return true;}
        
        bool checkShortCastle() const;
        bool checkLongCastle() const;

    public: 
        King(Color color);
        ~King() override = default;

        void printPiece() const override;
};

class Player {
    private:
        int m_material;

    public:
        Player() = default;
        ~Player() = default;
};

class Chess {
    private:
        Color m_player_turn;
        Player m_white;
        Player m_black;    
 
        std::vector<std::vector<Piece*>> m_board;

        void setPiece(const Point& coord, Piece* const piece);
        Piece* getPiece(const Point& coord) const;

        void initializeRow(int row_number, Color color, bool mode);

        static bool checkInput(const std::string& move);
        static void initializeCoordinates(Point& start, Point& end, const std::string& move);
        
        bool checkStart(const Point& start) const;
        bool checkEnd(const Point& end) const;

        bool checkCheck() const;
        bool checkGameOver() const;

    public:
        Chess();
        ~Chess();

        void makeMove();

        void printBoard() const;
};

#endif
