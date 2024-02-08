#ifndef CHESS_H
#define CHESS_H

#include <vector>

struct Point {
    int x;
    int y;
};

class Piece {
    private:
        bool m_color; // 0 white, 1 black
        int m_value; // value of piece Queen 9

        virtual bool checkMove(Point dest) const;
    
    public:
        Piece(bool color, int value);
        virtual ~Piece() = default;

        virtual void printPiece() const = 0;

        char getColor() const;
};

class Pawn : public Piece {
    private:
        bool checkMove(Point dest) const override;

    public:
        Pawn(bool color);
        ~Pawn() override = default;

        void printPiece() const override;
};

class Knight : public Piece {
    private:
        bool checkMove(Point dest) const override;
        
    public:
        Knight(bool color);
        ~Knight() override = default;

        void printPiece() const override;
};

class Bishop : public Piece {
    private:
        bool checkMove(Point dest) const override;

    public:
        Bishop(bool color);
        ~Bishop() override = default;
       
        void printPiece() const override;
};

class Rook : public Piece {
    private:
        bool m_castle_available;
        bool checkMove(Point dest) const override;

    public:
        Rook(bool color);
        ~Rook() override = default;
        
        void printPiece() const override;
};

class Queen : public Piece {
    private:
        bool checkMove(Point dest) const override;

    public:
        Queen(bool color);
        ~Queen() override = default;
        
        void printPiece() const override;
};

class King : public Piece {
    private:
        bool m_castle_available;

        bool checkMove(Point dest) const override;
        
        bool checkShortCastle() const;
        bool checkLongCastle() const;

    public: 
        King(bool color);
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

class Board {
        std::vector<std::vector<Piece*>> m_board;
        
        void initializeRow(int row_number, bool color, bool mode);

        bool checkCheck() const;
        bool checkGameOver() const;

    public:
        Board();
        ~Board();

        void printBoard() const;
};

class Chess {
    private:
        bool m_turn;
        Player m_white;
        Player m_black;    

    public:
        Chess() = default;
        ~Chess() = default;

        Board m_data;
};

#include "chessFunc.cpp"

#endif
