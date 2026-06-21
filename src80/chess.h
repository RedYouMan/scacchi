#ifndef CHESS
#define CHESS
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <cmath>

using namespace std;
// definizioni di casella, pezzo e scacchiera
// Autore Rosario Turco Dicembre 2025

enum TypeCastle
{
    LONG_CASTLE,
    SHORT_CASTLE
};

enum TypePiece
{
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING,
    NOTHING
};

// definizione classe base generica pezzo
class ChessPiece
{

private:
    char colorPiece;
    // W or B
    char namePiece[20];
    char namePieceImg[25];
    TypePiece tyePiece;

public:
    ChessPiece();
    ~ChessPiece();
    virtual bool checkMove(string source, string destination)
    {
        printf("stiamo usando il metodo virtuale della classe base Piece\n");
        return false;
    };
    void setNamePiece(const char *name);
    char *getNamePiece();
    void setColorPiece(char color);
    char getColorPiece();
    void setNameImg();
    char *getNameImg();
    void setTypePiece(TypePiece t);
    TypePiece getTypePiece();
    bool checkJump(string source, string destination, bool toPrint);
    bool checkTraversa(string src, string dest, bool toPrint);
    bool checkDiagonale(string src, string dest, bool toPrint);
};
// casella di scacchiera
class ChessSquare
{

private:
    char colorSquare;
    char squareName[3];
    bool busySquare;
    ChessPiece piece;
    bool isValid00;
    bool isValid000;

public:
    ChessSquare();
    ChessSquare(const char *name);
    ~ChessSquare();
    void setColorSquare(const char *name);
    char getColorSquare();
    void setBusySquare(bool present);
    bool getBusySquare();
    void setSquareName(const char *name);
    char *getSquareName();
    void setChessPiece(ChessPiece p);
    ChessPiece getChessPiece();
    void setValid00(bool valid);
    bool getValid00();
    void setValid000(bool valid);
    bool getValid000();
};

// definizione di tutti i pezzi

class Nothing : public ChessPiece
{
public:
    Nothing();
    ~Nothing();
};
class Bishop : public ChessPiece
{

public:
    Bishop();
    ~Bishop();
    Bishop(const char *name, char color);
    bool checkMove(string source, string destination);
};

class Rook : public ChessPiece
{

public:
    Rook();
    ~Rook();
    Rook(const char *name, char color);
    bool checkMove(string source, string destination);
};
class Knight : public ChessPiece
{
public:
    Knight();
    ~Knight();
    Knight(const char *name, char color);
    bool checkMove(string source, string destination);
};
class Queen : public ChessPiece
{
public:
    Queen();
    ~Queen();
    Queen(const char *name, char color);
    bool checkMove(string source, string destination);
};
class King : public ChessPiece
{
public:
    King();
    ~King();
    King(const char *name, char color);
    bool checkMove(string source, string destination);
    bool checkCastleUnderAttack(int i, int j, TypeCastle t, char who);
    bool checkSquareUnderAttack(int i, int j, char who, bool jumpKingControl = false);
    bool checkFreeSquare(int i, int j, char who);
};
class Pawn : public ChessPiece
{
public:
    Pawn();
    ~Pawn();
    Pawn(const char *name, char color);
    bool checkMove(string source, string destination);
    void promotion(string source, string destination, char color);
    bool checkFork(int i, int j, int r, int c, char who);
};

class ChessUtility
{

public:
    vector<int> getIndexPuntoDiVista(string square, string puntoDiVista);
    string getSquarePuntoDiVista(int i, int j, string puntoDiVista);
};

#endif