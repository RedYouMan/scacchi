// chess.cpp
#include <iostream>
#include "globale.h"
#include "chess.h"
#include "proto.h"
#include <cctype>
#include <math.h>
using namespace std;

/*

 Il programma permette a due giocatori di giocare a scacchi, con notazione italiana algebrica.
    Implementa le classi per i pezzi degli scacchi, le caselle della scacchiera e la scacchiera stessa.
    Definisce i metodi per il movimento dei pezzi, la gestione della scacchiera e le regole di base del gioco.
    Permette di controllare che ogni movimento dei pezzi sia possibile, può consentire di registrare la partita o sospendere una partita  per poi riprenderla successivamente, sia se registrata o meno.
    Consente, ulteriormente, il caricamento di problemi da risolvere.

    A livello object oriented si avvale di ereditarietà e polimorfismo per gestire i diversi tipi di pezzi degli scacchi.
    La generica casella può contenere o meno un pezzo e gestisce il colore della casella stessa.
    Il pezzo si specializza in un oggetto Bishop, Rook, Knight, Queen, King o Pawn con il colore Black o White.
    La scacchiera è una matrice di caselle.
    La funzione void init() in board.cpp permette di disporre a partita iniziale i pezzi sulle caselle della scacchiera.

La scacchiera deve essere posizionata con casella bianca in h1 ovvero a destra della riga in basso .
 */
ChessPiece::~ChessPiece() {}

ChessPiece::ChessPiece() {};

ChessSquare::~ChessSquare() {}
ChessSquare::ChessSquare()
{
}
ChessSquare::ChessSquare(const char *name)
{
    setBusySquare(false);
    setSquareName(name);
    setColorSquare(name);
}

void ChessSquare::setColorSquare(const char *name)
{
    // in base al name devo settare il colore della casella
    // ad esempio casella a8, inizio a prendere il valore numerico

    int valore = name[1] - 48;
    // valori riga scacchiera dispari
    if (valore % 2 != 0 && (name[0] == 'a' || name[0] == 'c' || name[0] == 'e' || name[0] == 'g'))
    {
        colorSquare = 'B';
    }
    if (valore % 2 != 0 && (name[0] == 'b' || name[0] == 'd' || name[0] == 'f' || name[0] == 'h'))
    {
        colorSquare = 'W';
    }
    // valori riga scacchiera pari
    if (valore % 2 == 0 && (name[0] == 'a' || name[0] == 'c' || name[0] == 'e' || name[0] == 'g'))
    {
        colorSquare = 'W';
    }
    if (valore % 2 == 0 && (name[0] == 'b' || name[0] == 'd' || name[0] == 'f' || name[0] == 'h'))
    {
        colorSquare = 'B';
    }
    return;
}

void ChessPiece::setTypePiece(TypePiece t)
{
    tyePiece = t;
}

TypePiece ChessPiece::getTypePiece()
{
    return tyePiece;
}
void ChessPiece::setNamePiece(const char *name)
{
    strcpy_s(namePiece, name);

    return;
}

char *ChessPiece::getNamePiece() { return namePiece; };

void ChessPiece::setColorPiece(char color) { colorPiece = color; };
char ChessPiece::getColorPiece() { return colorPiece; };
void ChessPiece::setNameImg()
{
    strcpy_s(namePieceImg, getNamePiece());
    strcat_s(namePieceImg, ".png");
    return;
}
char *ChessPiece::getNameImg()
{
    strcpy_s(namePieceImg, namePiece);
    strcat_s(namePieceImg, ".png");
    return namePieceImg;
};

char ChessSquare::getColorSquare() { return colorSquare; }

void ChessSquare::setBusySquare(bool present)
{
    busySquare = present;

    // se false se casella di arrocco setto isValid a false

    if (present == false && (strcmp(this->getSquareName(), "a1") == 0 || strcmp(this->getSquareName(), "a8") == 0))
    {
        this->setValid000(false);
    }
    if (present == false && (strcmp(this->getSquareName(), "h1") == 0 || strcmp(this->getSquareName(), "h8") == 0))
    {
        this->setValid00(false);
    }
    return;
}
bool ChessSquare::getBusySquare() { return busySquare; };
void ChessSquare::setSquareName(const char *name) { strcpy_s(squareName, name); };
char *ChessSquare::getSquareName() { return squareName; };
Bishop::Bishop() {};
Bishop::~Bishop() {};
Bishop::Bishop(const char *name, char color)
{
    ChessPiece();
    setColorPiece(color);
    setNamePiece(name);
    setTypePiece(BISHOP);
}

Rook::Rook() {}
Rook::~Rook() {}
Rook::Rook(const char *name, char color)
{
    ChessPiece();
    setColorPiece(color);
    setNamePiece(name);
    setTypePiece(ROOK);
}
Queen::Queen() {}
Queen::~Queen() {}
Queen::Queen(const char *name, char color)
{
    ChessPiece();
    setColorPiece(color);
    setNamePiece(name);
    setTypePiece(QUEEN);
}

King::King() {}
King::~King() {}
King::King(const char *name, char color)
{

    ChessPiece();
    setColorPiece(color);
    setNamePiece(name);
    setTypePiece(KING);
}

Knight::Knight() {}
Knight::~Knight() {}
Knight::Knight(const char *name, char color)
{
    ChessPiece();
    setColorPiece(color);
    setNamePiece(name);
    setTypePiece(KNIGHT);
}
Pawn::Pawn() {}
Pawn::~Pawn() {}
Pawn::Pawn(const char *name, char color)
{
    ChessPiece();
    setColorPiece(color);
    setNamePiece(name);
    setTypePiece(PAWN);
}

void ChessSquare::setChessPiece(ChessPiece p)
{
    piece = p;
    setBusySquare(true);
}

ChessPiece ChessSquare::getChessPiece()
{
    return piece;
}

bool ChessPiece::checkTraversa(string src, string dest, bool toPrint)
{

    bool status = true;

    // controllo se è una mossa di traversa
    if (!(src[0] == dest[0] || src[1] == dest[1] && src[0] != dest[0]))
    {
        if (toPrint)
        {
            printf("Mossa errata. Non risulta corretta o vuota la traversa\n");
        }
        return false;
    }
    // ma se non ci sono pezzi dalla seconda alla penultima casa
    ChessUtility utility;
    vector<int> Idx_src = utility.getIndexPuntoDiVista(src, puntoDiVista);
    int riga_src = Idx_src.front();
    int col_src = Idx_src.back();

    vector<int> Idx_dest = utility.getIndexPuntoDiVista(dest, puntoDiVista);
    int riga_dest = Idx_dest.front();
    int col_dest = Idx_dest.back();
    int secondaCasa = 0;
    int penultimaCasa = 0;
    // se mi muovo sulla stessa colonna
    if (col_src == col_dest)
    {

        if (riga_src > riga_dest)
        {
            // conviene prendere la riga minore per il for, per cui scambio i ruoli di dest e src
            secondaCasa = riga_dest + 1;
            penultimaCasa = riga_src - 1;
        }
        else
        {
            secondaCasa = riga_src + 1;
            penultimaCasa = riga_dest - 1;
        }
        for (int i = secondaCasa; i <= penultimaCasa; i++)
        {
            if (chessBoard[i][col_src].getBusySquare() == true)
            {
                if (toPrint)
                {
                    printf("1 - La traversa non risulta vuota\n");
                }
                return false;
            }
        }
    }
    // se sono sulla stessa riga
    if (riga_src == riga_dest)
    {

        if (col_src > col_dest)
        {
            secondaCasa = col_dest + 1;
            penultimaCasa = col_src - 1;
        }
        else
        {
            secondaCasa = col_src + 1;
            penultimaCasa = col_dest - 1;
        }

        for (int i = secondaCasa; i <= penultimaCasa; i++)
        {
            if (chessBoard[riga_src][i].getBusySquare() == true)
            {

                if (toPrint)
                {
                    printf("2 - La traversa non risulta vuota\n");
                }
                return false;
            }
        }
    }

    return status;
}
bool Rook::checkMove(string source, string destination)
{
    bool status = true;
    string msg = "debug: verifica regole di gioco per la torre\n";
    printDebug(msg);

    if (!checkTraversa(source, destination, true))
    {
        return false;
    }
    return status;
}
bool Bishop::checkMove(string source, string destination)
{
    bool status = false;
    string msg = "debug: verifica regole di gioco per l'alfiere\n";
    printDebug(msg);

    if (checkDiagonale(source, destination, true) == true)
    {
        return true;
    }
    return status;
}
bool Queen::checkMove(string source, string destination)
{
    bool status = true;
    string msg = "debug: verifica regole di gioco per la donna\n";
    printDebug(msg);

    // chiamo traversa se le righe sono uguali o le colonne sono uguali altrimenti chiamo diagonale
    if (source[0] == destination[0] || source[1] == destination[1])
    {
        if (checkTraversa(source, destination, false) == false)
        {
            return false;
        }
    }
    else
    {
        if (checkDiagonale(source, destination, false) == false)
        {
            return false;
        }
    }
    return status;
}
bool King::checkMove(string source, string destination)
{
    bool status = true;
    string msg = "debug: verifica regole di gioco per il re\n";
    printDebug(msg);

    int distanceRow = abs(source[1] - destination[1]);
    int distanceCol = abs(source[0] - destination[0]);
    ChessUtility utility;
    vector<int> Idx_source = utility.getIndexPuntoDiVista(source, puntoDiVista);
    vector<int> Idx_destination = utility.getIndexPuntoDiVista(destination, puntoDiVista);

    int i = Idx_source.front();
    int j = Idx_source.back();
    int m = Idx_destination.front();
    int n = Idx_destination.back();
    // verifica mossa non ammissibile
    if (distanceCol > 2 || distanceRow > 1)
    {
        printf("Numero di passi errato per il re\n");
        return false;
    }

    // controllo per arrocco- movimento su stessa riga di due passi
    if (source[1] == destination[1] && distanceCol == 2 && (i == 0 || i == 7))
    {
        printDebug(string("verifica se arrocco stessariga\n"));

        // le due case di passaggio devono essere vuote innanzitutto

        King re_da_controllare = King();
        char myColor = chessBoard[i][j].getChessPiece().getColorPiece();

        // sotto scacco non si può arroccare
        if (re_da_controllare.checkSquareUnderAttack(i, j, myColor) == true)
        {
            printf("Non si può arroccare se sotto scacco!\n");
            return false;
        }

        // prima delle verifiche sugli arrocchi
        if ((j > n && puntoDiVista == "Bianco") || (puntoDiVista == "Nero" && j < n))
        {
            /* arrocco lungo */

            msg.clear();
            msg = "debug: Richiesto arrocco lungo\n";
            printDebug(msg);
            // mi chiedo prima se le due case sono libere

            if (puntoDiVista == "Bianco" && (chessBoard[i][j - 1].getBusySquare() == true || chessBoard[i][j - 2].getBusySquare() == true))
            {
                printf("Presenza di pezzo\n");
                return false;
            }
            if (puntoDiVista == "Nero" && (chessBoard[i][j + 1].getBusySquare() == true || chessBoard[i][j + 2].getBusySquare() == true))
            {
                printf("Presenza di pezzo\n");
                return false;
            }

            if ((puntoDiVista == "Bianco" && chessBoard[i][j - 4].getValid000()) || (puntoDiVista == "Nero" && chessBoard[i][j + 4].getValid000()))
            {
                if (checkCastleUnderAttack(i, j, LONG_CASTLE, chessBoard[i][j].getChessPiece().getColorPiece()) == true)
                {

                    printf("case dell'arrocco sotto attacco\n");
                    return false;
                }
            }
            else
            {
                printf("Pezzi precedentemente mossi, arrocco non consentito\n");
                return false;
            }

            // sposto la torre
            //  prendo il pezzo della casella src e lo metto nella casella dest
            is_arrocco = true;
            tipoArrocco = "lungo";
            ChessSquare casellaSrc, casellaDest;
            if (puntoDiVista == "Bianco")
            {
                casellaSrc = chessBoard[i][j - 4];
                startRookRow = i;
                startRookCol = j - 4;
                casellaDest = chessBoard[i][3];
                endRookRow = i;
                endRookCol = 3;
            }
            else
            {
                casellaSrc = chessBoard[i][j + 4];
                startRookRow = i;
                startRookCol = j + 4;
                casellaDest = chessBoard[i][4];
                endRookRow = i;
                endRookCol = 4;
            }
            casellaDest.setChessPiece(casellaSrc.getChessPiece());
            if (i == 7 && puntoDiVista == "Bianco")
            {
                casellaDest.setSquareName("d1");
            }

            if (i == 7 && puntoDiVista == "Nero")
            {
                casellaDest.setSquareName("d8");
            }

            if (i == 0 && puntoDiVista == "Bianco")
            {
                casellaDest.setSquareName("d8");
            }

            if (i == 0 && puntoDiVista == "Nero")
            {
                casellaDest.setSquareName("d1");
            }
            casellaDest.setBusySquare(true);

            if (puntoDiVista == "Bianco")
            {
                chessBoard[i][3] = casellaDest;
            }
            else
            {
                chessBoard[i][4] = casellaDest;
            }
            casellaSrc.setBusySquare(false);
            if (puntoDiVista == "Bianco")
            {
                chessBoard[i][j - 4] = casellaSrc;
            }
            else
            {
                chessBoard[i][j + 4] = casellaSrc;
            }
            arrocco.clear();

            arrocco = "000";
            printf("Effettuato arrocco lungo\n");
        }
        else
        {
            /* arrocco corto */
            msg.clear();
            msg = "debug: Richiesto arrocco corto\n";
            printDebug(msg);
            // mi chiedo prima se le due case sono libere
            if (puntoDiVista == "Bianco" && (chessBoard[i][j + 1].getBusySquare() == true || chessBoard[i][j + 2].getBusySquare() == true))
            {
                printf("Presenza di pezzo\n");

                return false;
            }
            if (puntoDiVista == "Nero" && (chessBoard[i][j - 1].getBusySquare() == true || chessBoard[i][j - 2].getBusySquare() == true))
            {
                printf("Presenza di pezzo\n");

                return false;
            }

            if ((puntoDiVista == "Bianco" && chessBoard[i][j + 3].getValid00()) || (puntoDiVista == "Nero" && chessBoard[i][j - 3].getValid00()))
            {

                if (checkCastleUnderAttack(i, j, SHORT_CASTLE, chessBoard[i][j].getChessPiece().getColorPiece()) == true)
                {
                    printf("Case dell'arrocco sotto attacco\n");

                    return false;
                }
            }
            else
            {
                printf("Pezzi precedentemente mossi, arrocco non consentito\n");

                return false;
            }

            // sposto la torre
            //  prendo il pezzo della casella src e lo metto nella casella dest
            is_arrocco = true;
            tipoArrocco = "corto";

            ChessSquare casellaSrc, casellaDest;
            if (puntoDiVista == "Bianco")
            {
                casellaSrc = chessBoard[i][j + 3];
                startRookRow = i;
                startRookCol = j + 3;
                casellaDest = chessBoard[i][5];
                endRookRow = i;
                endRookCol = 5;
            }
            else
            {
                casellaSrc = chessBoard[i][j - 3];
                startRookRow = i;
                startRookCol = j - 3;
                casellaDest = chessBoard[i][2];
                endRookRow = i;
                endRookCol = 2;
            }
            casellaDest.setChessPiece(casellaSrc.getChessPiece());
            if (i == 0 && puntoDiVista == "Bianco")
            {
                casellaDest.setSquareName("f8");
            }
            if (i == 0 && puntoDiVista == "Nero")
            {
                casellaDest.setSquareName("f1");
            }

            if (i == 7 && puntoDiVista == "Bianco")
            {
                casellaDest.setSquareName("f1");
            }
            if (i == 7 && puntoDiVista == "Nero")
            {
                casellaDest.setSquareName("f8");
            }

            casellaDest.setBusySquare(true);
            if (puntoDiVista == "Bianco")
            {
                chessBoard[i][5] = casellaDest;
            }
            else
            {
                chessBoard[i][2] = casellaDest;
            }
            casellaSrc.setBusySquare(false);
            if (puntoDiVista == "Bianco")
            {
                chessBoard[i][j + 3] = casellaSrc;
            }
            else
            {
                chessBoard[i][j - 3] = casellaSrc;
            }
            arrocco.clear();
            arrocco = "00";
            printf("Effettuato arrocco corto\n");
        }
    }

    // caso passo di 1 e mangia un pezzo difeso o va in casa minacciata
    if (distanceRow == 1 || distanceCol == 1)
    {
        // controllo la casa di destinazione
        printDebug(string("caso passo di 1 e mangia pezzo difeso o va in casa sotto attacco\n"));
        King testCasella = King();
        char useColorOfKing = chessBoard[i][j].getChessPiece().getColorPiece();
        // char color = chessBoard[m][n].getChessPiece().getColorPiece();
        if (testCasella.checkSquareUnderAttack(m, n, useColorOfKing))
        {

            printf("casella controllata dall'avversario!\n");
            return false;
        }
    }

    KingMove = true;
    // se il re viene mosso dalla casa di origine non sono validi gli arrocchi
    if (source == "e1")
    {

        vector<int> Iarr_a1 = utility.getIndexPuntoDiVista(string("a1"), puntoDiVista);
        vector<int> Iarr_h1 = utility.getIndexPuntoDiVista(string("h1"), puntoDiVista);
        chessBoard[Iarr_a1.front()][Iarr_a1.back()].setValid000(false);
        chessBoard[Iarr_h1.front()][Iarr_h1.back()].setValid00(false);
    }

    if (source == "e8")
    {
        vector<int> Iarr_a8 = utility.getIndexPuntoDiVista(string("a8"), puntoDiVista);
        vector<int> Iarr_h8 = utility.getIndexPuntoDiVista(string("h8"), puntoDiVista);

        chessBoard[Iarr_a8.front()][Iarr_a8.back()].setValid000(false);
        chessBoard[Iarr_h8.front()][Iarr_h8.back()].setValid00(false);
    }

    return status;
}
bool Knight::checkMove(string source, string destination)
{

    bool status = true;

    string msg = "debug: verifica regole di gioco per il cavallo\n";
    printDebug(msg);
    // mossa ad L
    status = checkJump(source, destination, true);
    return status;
}

bool ChessPiece::checkJump(string source, string destination, bool toPrint)
{
    bool status = false;

    string msg;
    // mossa ad L di 4 caselle

    int d1 = source[0] - 97;
    int d2 = destination[0] - 97;
    int distanceO = abs(d1 - d2);
    int h1 = source[1] - 48;
    int h2 = destination[1] - 48;
    int distanceV = abs(h1 - h2);
    msg.clear();
    msg = "debug: salto cavallo. Distanza orizzontale " + to_string(distanceO) + " distanza verticale " + to_string(distanceV) + "\n";
    printDebug(msg);
    if ((distanceO == 1 && distanceV == 2 || distanceO == 2 && distanceV == 1))
    {
        status = true;
    }
    else
    {
        if (toPrint)
        {
            printf("violazione della regola di movimento del cavallo\n");
        }
        return false;
    }

    return status;
}
bool Pawn::checkMove(string source, string destination)
{

    bool status = true;
    string msg = "debug: verifica regole di gioco per il pedone\n";
    printDebug(msg);

    ChessUtility utility;
    vector<int> Idx_src = utility.getIndexPuntoDiVista(source, puntoDiVista);
    vector<int> Idx_dest = utility.getIndexPuntoDiVista(destination, puntoDiVista);
    int riga1_src = Idx_src.front();
    int riga2_dest = Idx_dest.front();
    int distanceV = abs(riga1_src - riga2_dest);

    int colonna1_src = Idx_src.back();
    int colonna2_dest = Idx_dest.back();
    int distanceO = abs(colonna1_src - colonna2_dest);

    char color = chessBoard[riga1_src][colonna1_src].getChessPiece().getColorPiece();

    // un pedone va solo avanti
    // le righe di un pedone nero devono diminuire mentre quelle del bianco devono aumentare
    // ma dipende dal punto di vista
    if (color == 'W' && riga1_src < riga2_dest && puntoDiVista == "Bianco")
    {
        printf("Errore di direzione\n");
        return false;
    }
    if (color == 'W' && riga1_src > riga2_dest && puntoDiVista == "Nero")
    {
        printf("Errore di direzione\n");
        return false;
    }

    if (color == 'B' && riga1_src > riga2_dest && puntoDiVista == "Bianco")
    {

        printf("Errore di direzione\n");
        return false;
    }

    if (color == 'B' && riga1_src < riga2_dest && puntoDiVista == "Nero")
    {
        printf("Errore di direzione\n");
        return false;
    }

    // se il pedone non è in casa di origine solo un passo possibile
    if ((!(riga1_src == 1 || riga1_src == 6)) && distanceV > 1)
    {
        printf("Un pedone non in casa di origine non può fare oltre un passo!\n");
        return false;
    }

    // quando non può catturare  in diagonale
    if (distanceV == 1 && distanceO == 1 && chessBoard[riga2_dest][colonna2_dest].getBusySquare() && chessBoard[riga2_dest][colonna2_dest].getChessPiece().getColorPiece() != this->getColorPiece())
    {

        printDebug(string("debug:step 1\n"));
        return true;
    }

    // i passi devono essere >0 e <= 2 ma devono essere libere tutte le case dei passi
    // case libere davanti
    if (puntoDiVista == "Bianco" && (!(distanceV > 0 && distanceV <= 2) || (distanceV > 1 && ((chessBoard[riga2_dest][colonna2_dest].getBusySquare() == true || chessBoard[riga2_dest + 1][colonna2_dest].getBusySquare() == true && chessBoard[riga1_src][colonna1_src].getChessPiece().getColorPiece() == 'W') || ((chessBoard[riga2_dest][colonna2_dest].getBusySquare() == true || chessBoard[riga2_dest - 1][colonna2_dest].getBusySquare() == true && chessBoard[riga1_src][colonna1_src].getChessPiece().getColorPiece() == 'B')))) || (distanceV == 1 && chessBoard[riga2_dest][colonna2_dest].getBusySquare() == true)))
    {
        printf("1 - violazione della regola di movimento del pedone con casella di destinazione non raggiungibile\n");
        return false;
    }
    if (puntoDiVista == "Nero" && (!(distanceV > 0 && distanceV <= 2) || (distanceV > 1 && ((chessBoard[riga2_dest][colonna2_dest].getBusySquare() == true || chessBoard[riga2_dest - 1][colonna2_dest].getBusySquare() == true && chessBoard[riga1_src][colonna1_src].getChessPiece().getColorPiece() == 'W') || ((chessBoard[riga2_dest][colonna2_dest].getBusySquare() == true || chessBoard[riga2_dest + 1][colonna2_dest].getBusySquare() == true && chessBoard[riga1_src][colonna1_src].getChessPiece().getColorPiece() == 'B')))) || (distanceV == 1 && chessBoard[riga2_dest][colonna2_dest].getBusySquare() == true)))
    {
        printf("2 - violazione della regola di movimento del pedone con casella di destinazione non raggiungibile\n");
        return false;
    }

    // Regola peri pezzi amici
    // I pezzi amici sono controllati anche in int move()

    // quando non può catturare  in diagonale
    if (distanceV == 1 && distanceO == 1 && chessBoard[riga2_dest][colonna2_dest].getBusySquare() && chessBoard[riga2_dest][colonna2_dest].getChessPiece().getColorPiece() == this->getColorPiece())
    {
        printf("violazione della regola di movimento di cattura del pedone: casella occupata da pezzo amico\n");
        return false;
    }

    // la regola di sopra non vale se ci sono caselle vuote
    if (distanceV == 1 && distanceO == 1 && chessBoard[riga2_dest][colonna2_dest].getBusySquare() == false && EnP == false)
    {
        printf("violazione della regola di movimento del pedone: casella vuota\n");
        return false;
    }
    // gestione enpassant
    // coordinamento tra board.cpp, undo.cpp e chess.cpp
    if (EnP == true)
    {
        // controllo se il pedone mosso corrisponde ad uno nel vectordegli enpassant
        bool trovato = false;
        for (const std::string &elemento : pawnEnP)
        {

            if (elemento == chessBoard[riga1_src][colonna1_src].getSquareName() || elemento == chessBoard[riga1_src][colonna1_src].getSquareName())
            {
                // trovato
                trovato = true;
                break;
            }
        }

        if (trovato)
        {
            if (distanceV == 1 && distanceO == 1)
            {
                status = true;
                printDebug(string("debug: step 2\n")); // permettendolo l'enpassant allora resetto

                EnP = false;
                pawnEnP.clear();
                msg = "debug: resetto enpassant\n";
                printDebug(msg);
                ChessSquare casella = ChessSquare(casellaDaCatturare.c_str());
                casella.setBusySquare(false);

                ChessUtility utility;
                vector<int> Idx = utility.getIndexPuntoDiVista(casellaDaCatturare, puntoDiVista);

                int riga = Idx.front();
                int colonna = Idx.back();
                chessBoard[riga][colonna] = casella;
                msg.clear();
                msg = "catturato pedone avversario  in " + casellaDaCatturare + " con enpassant\n";
                avvenutaCattura = true;

                printf("%s\n", msg.c_str());
                casellaDaCatturare.clear();
            }
        }
    }

    // un pedone può catturare un pezzo avversario a forchetta da lui
    if (distanceV == distanceO && chessBoard[riga2_dest][colonna2_dest].getChessPiece().getColorPiece() != color)
    {
        // cattura in diagonale
        return true;
    }

    return status;
}
void Pawn::promotion(string source, string destination, char color)
{

    ChessUtility utility;
    vector<int> Idx_prom = utility.getIndexPuntoDiVista(destination, puntoDiVista);
    int riga = Idx_prom.front();
    int col = Idx_prom.back();
    string appo1, appo2, appo3;
    char scelta;
    appo1 = (color == 'W' ? "Bianco" : "Nero");
    appo2 = (color == 'W' ? "Bianca" : "Nera");
    ChessSquare casella = ChessSquare(destination.c_str());
    if (riga == 7 || riga == 0)
    {
        // promuovere a pezzo
        callTextToSpeech(string("Fai Alt-Tab per andare sulla console e rispondere alle domande\n"));
        callTextToSpeech(string("Scegli la figura a cui promuovere il pedone (T come Torre, C come Cavallo, Acome Alfiere, D come Donna) e fai return\n"));
        cin >> scelta;

        if (toupper(scelta) == 'T')
        {
            appo3 = "Torre";
            appo3.append(appo2);
            Rook torre = Rook(appo3.c_str(), color);
            casella.setChessPiece(torre);
            callTextToSpeech(string("Pedone promosso a Torre\n"));
        }

        if (toupper(scelta) == 'D')
        {
            appo3 = "Donna";
            appo3.append(appo2);
            Queen donna = Queen(appo3.c_str(), color);
            casella.setChessPiece(donna);
            callTextToSpeech(string("Pedone promosso a Donna\n"));
        }

        if (toupper(scelta) == 'C')
        {
            appo3 = "Cavallo";
            appo3.append(appo1);
            Knight cavallo = Knight(appo3.c_str(), color);
            casella.setChessPiece(cavallo);
            callTextToSpeech(string("Pedone promosso a Cavallo\n"));
        }
        if (toupper(scelta) == 'A')
        {
            appo3 = "Alfiere";
            appo3.append(appo1);
            Bishop alfiere = Bishop(appo3.c_str(), color);
            casella.setChessPiece(alfiere);
            callTextToSpeech(string("Pedone promosso ad Alfiere\n"));
        }
        casella.setBusySquare(true);
        chessBoard[riga][col] = casella;
    }
    callTextToSpeech(string("Fai Alt-Tab e ritorna sulla scacchiera\n"));
    return;
}
bool ChessPiece::checkDiagonale(string src, string dest, bool toPrint)
{
    bool status = true;
    // true è ok

    /*
    Il chiamante di questa funzionalità sfrutta false
    per dire che il movimento non è corretto,
    Qui internamente true è ok come movimento e case tutte libere, mentre  false è nok
    */
    string msg;
    msg.clear();
    // controllo della diagonalità

    if (src[0] == dest[0] || src[1] == dest[1])
    {
        if (toPrint)
        {
            printf("Il pezzo non si muove su una diagonale corretta\n");
        }
        return false;
    }
    if (!(abs(src[0] - dest[0]) + 1 == abs(src[1] - dest[1]) + 1))
    {

        if (toPrint)
        {
            printf("Il pezzo non si muove su una diagonale corretta\n");
        }
        return false;
    }
    // controllo della ostruzione della diagonale
    // torna false se ostruita
    ChessUtility utility;
    vector<int> IndiceSrc = utility.getIndexPuntoDiVista(src, puntoDiVista);
    vector<int> IndiceDest = utility.getIndexPuntoDiVista(dest, puntoDiVista);
    int i = IndiceSrc.front();
    int j = IndiceSrc.back();

    int m = IndiceDest.front();
    int n = IndiceDest.back();

    // individuo se diagonale da sinistra a destra (S-D)o da destra a sinistra(D-S)
    // il verso dipende ora dal numero

    int starti = 0, startj = 0, endi = 0, endj = 0, incrx = 0, incry = 0;

    // controllo sulle colonne
    if (j < n)
    {
        if (i < m)
        {
            // S-D verso basso  (ricorda 0,0 sta in alto)
            // individuo seconda casa e penultima casa
            // cioè escludo le case source e destination per evitare di rilevare presente un pezzo ovvero me stesso.
            // starti e startj sono seconda casa e endi e endj penultima casa
            msg.clear();
            // S/D VERSO IL BASSO

            // ovvero la riga 0 è in alto nella matrice e le lettere iniziano a sinistra
            starti = i + 1;
            startj = j + 1;
            endi = m - 1;
            endj = n - 1;
            incrx = +1;
            incry = +1;
            msg = "debug: caso 1 sinistra destra verso il basso\n";
            printDebug(msg);

            for (int x = starti, y = startj; x <= endi, y <= endj; x = x + incrx, y = y + incry)
            {
                if (chessBoard[x][y].getBusySquare() == true)
                {

                    if (toPrint)
                    {
                        printf("Errore: La diagonale risulta occupata da pezzi\n");
                    }
                    return false;
                }
            }
        }
        else
        {
            // i > m
            //   Sinistra Destra  alto

            starti = i - 1;
            startj = j + 1;
            endi = m + 1;
            endj = n - 1;
            incrx = -1;
            incry = +1;
            msg.clear();
            msg = "debug: caso 2 sinistra destra verso alto \n";
            printDebug(msg);

            for (int x = starti, y = startj; x <= endi, y <= endj; x = x + incrx, y = y + incry)
            {
                if (chessBoard[x][y].getBusySquare() == true)
                {
                    if (toPrint)
                    {
                        printf("Errore: La diagonale risulta occupata da pezzi\n");
                    }
                    return false;
                }
            }
        }
    }
    else
    {
        // destra sinistrada sostituire con sinistra destra
        // invertendo source con destination
        vector<int> Inv_src, Inv_dest;
        Inv_src = utility.getIndexPuntoDiVista(dest, puntoDiVista);
        Inv_dest = utility.getIndexPuntoDiVista(src, puntoDiVista);

        i = Inv_src.front();
        j = Inv_src.back();
        m = Inv_dest.front();
        n = Inv_dest.back();

        // j>n
        if (m < i)

        {
            //  D/S verso basso
            // la riconduco a sinistra destra verso alto
            // ma devo invertire destination con source
            starti = i - 1;
            startj = j + 1;
            endi = m + 1;
            endj = n - 1;
            incrx = -1;
            incry = +1;
            msg.clear();
            msg = "debug: caso 3: destra sinistra basso sostituito con sinistra destra alto\n";
            printDebug(msg);
            for (int x = starti, y = startj; x <= endi, y <= endj; x = x + incrx, y = y + incry)
            {
                if (chessBoard[x][y].getBusySquare() == true)
                {

                    if (toPrint)
                    {
                        printf("Errore: La diagonale risulta occupata da pezzi\n");
                    }
                    return false;
                }
            }
        }
        else
        {

            //  D/S verso alto//
            // lo sostituiamo con sinistra destra basso
            // invertendo destinazione con sorgente
            // ovvero la riga 0 è in alto nella matrice
            starti = i + 1;
            startj = j + 1;
            endi = m - 1;
            endj = n - 1;
            incrx = +1;
            incry = +1;

            msg.clear();

            msg = "debug: caso 4 destra sinistra  alto sostituito con sinistra destra basso\n";
            printDebug(msg);
            for (int x = starti, y = startj; x <= endi, y <= endj; x = x + incrx, y = x + incry)
            {
                if (chessBoard[x][y].getBusySquare() == true)
                {
                    if (toPrint)
                    {
                        printf("Errore: La diagonale risulta occupata da pezzi\n");
                    }
                    return false;
                }
            }
        }
    }
    return status;
}

bool ChessSquare::getValid00()
{
    return isValid00;
}
void ChessSquare::setValid00(bool valid)
{
    isValid00 = valid;
    return;
}

void ChessSquare::setValid000(bool valid)
{
    isValid000 = valid;
    return;
}
bool ChessSquare::getValid000()
{
    return isValid000;
}

bool King::checkCastleUnderAttack(int i, int j, TypeCastle t, char who)
{
    bool status = false;
    // i e j sono le coordinate del Re
    // ritorna false se le case dell'arrocco non sono controllate da pezzi avversari
    // controllo di due case dell'arrocco (lungo o corto) se sono minacciate da pezzi avversari
    // dato che il Re ci deve transitare
    if (t == LONG_CASTLE)
    {
        /* arrocco lungo */
        int p = 0, q = 0;
        if (puntoDiVista == "Bianco")
        {
            p = j - 1;
            q = j - 2;
        }

        if (puntoDiVista == "Nero")
        {
            p = j + 1;
            q = j + 2;
        }
        if (checkSquareUnderAttack(i, p, who, true) || checkSquareUnderAttack(i, q, who, true))
        {
            printf("Caselle dell'arrocco sono sotto attacco. Arrocco non possibile\n");

            return true;
        }
    }
    if (t == SHORT_CASTLE)
    {

        int p = 0, q = 0;
        if (puntoDiVista == "Bianco")
        {
            p = j + 1;
            q = j + 2;
        }

        if (puntoDiVista == "Nero")
        {
            p = j - 1;
            q = j - 2;
        }

        if (checkSquareUnderAttack(i, p, who, true) || checkSquareUnderAttack(i, q, who, true))
        {

            printf("Caselle dell'arrocco sono sotto attacco. Arrocco non possibile\n");

            return true;
        }
    }
    return status;
}

bool King::checkSquareUnderAttack(int i, int j, char who, bool jumpKingControl)
{
    bool status = false;
    // torna false se tutto ok non sotto attacco
    // i e j sono le coordinate della casella da controllare se sotto attacco (la destination)

    /*
     Questa funzionalità interviene come mattoncino di base nei seguenti casi:
        - prima di un arrocco per verificare che le case interessate non siano sotto attacco(vedi anche checkCastleUnderAttack)
        - per verificare il tipo di scacco (vedi checkFreeSquare e cheScacco())
        - per verificare lo stallo (vedi cheStallo)
        - se un pezzo risulta difeso, ovvero in una casa controllata da un pezzo amico

        Attenzione: la funzionalità è usata in due modi opposti:
        - da sola per controllare se le case sono attaccate o difese (jumpKingControl a false)
        - per controllare se le case del proprio arrocco sono sotto attacco (jumpKingControl a true, ovvero si salta il controllo del  re che altrimenti attaccherebbe le proprie case. Difatti nel primo caso who è il re avversario e nel secondo il nostro re)
        */
    // su tutta la scacchiera cerco Alfieri, Cavalli, Donna, Torri e pedoni di colore diverso da who e che insistono su i e j
    // torna true se la casella è sotto attacco

    /* calcola destination da i e j e source da r ed s per usare diagonale e traverse, salto di cavallo o forchetta di pedone
     */
    ChessUtility utility;
    string destination = utility.getSquarePuntoDiVista(i, j, puntoDiVista);
    string msg;
    msg.clear();
    msg = "debug: casella da controllare se sotto attacco: " + destination + "\n";
    printDebug(msg);
    /* destination è la casella da verificare se sotto attacco da una qualunque parte della scacchiera */
    /* source è la casa del pezzo da verificare se minaccia la casa destination*/
    string source;
    int start = 0, startc = 0;

    for (int r = start; r < 8; r++)
    {

        for (int s = startc; s < 8; s++)
        {
            source.clear();
            source = utility.getSquarePuntoDiVista(r, s, puntoDiVista);

            // salto se stessa casa

            if (source == destination)
            {
                continue;
            }

            // anche un re minaccia le case
            if (jumpKingControl == false && chessBoard[r][s].getBusySquare() == true && chessBoard[r][s].getChessPiece().getTypePiece() == KING && chessBoard[r][s].getChessPiece().getColorPiece() != who)
            {
                // Se la distanza pari a 1
                if (abs(i - r) <= 1 && abs(j - s) <= 1)
                {
                    // la casa è minacciata
                    printDebug(string("casa " + destination + " minacciata da re in " + source + "\n"));
                    return true;
                }
            }

            if (chessBoard[r][s].getBusySquare() == true && chessBoard[r][s].getChessPiece().getTypePiece() == BISHOP && chessBoard[r][s].getChessPiece().getColorPiece() != who)
            {

                // nel caso dell'alfiere se la casa non è ostruita (true)allora è una minaccia
                if (chessBoard[r][s].getChessPiece().checkDiagonale(source, destination, false) == true)
                {
                    printDebug(string("casa " + destination + " minacciata da alfiere in " + source + "\n"));
                    return true;
                }
            }

            if (chessBoard[r][s].getBusySquare() == true && chessBoard[r][s].getChessPiece().getTypePiece() == QUEEN && chessBoard[r][s].getChessPiece().getColorPiece() != who)
            {
                if (chessBoard[r][s].getChessPiece().checkDiagonale(source, destination, false) == true)
                {
                    printDebug(string("casa " + destination + " minacciata da donna in " + source + "\n"));
                    return true;
                }
            }

            if (chessBoard[r][s].getBusySquare() == true && chessBoard[r][s].getChessPiece().getTypePiece() == QUEEN && chessBoard[r][s].getChessPiece().getColorPiece() != who)
            {
                if (chessBoard[r][s].getChessPiece().checkTraversa(source, destination, false) == true)
                {
                    printDebug(string("casa " + destination + " minacciata da donna in " + source + "\n"));
                    return true;
                }
            }
            // solo se la distanza < 4come riga almeno
            int distVCavallo = abs(source[1] - destination[1]);
            if (chessBoard[r][s].getBusySquare() == true && chessBoard[r][s].getChessPiece().getTypePiece() == KNIGHT && chessBoard[r][s].getChessPiece().getColorPiece() != who)
            {
                if (distVCavallo < 3 && chessBoard[r][s].getChessPiece().checkJump(source, destination, false) == true)
                {
                    printDebug(string("casa " + destination + " minacciata da cavallo in " + source + "\n"));
                    return true;
                }
            }

            if (chessBoard[r][s].getBusySquare() == true && chessBoard[r][s].getChessPiece().getTypePiece() == ROOK && chessBoard[r][s].getChessPiece().getColorPiece() != who)
            {
                if (chessBoard[r][s].getChessPiece().checkTraversa(source, destination, false) == true)
                {
                    printDebug(string("casa " + destination + " minacciata da torre in " + source + "\n"));
                    return true;
                }
            }
            if (chessBoard[r][s].getBusySquare() == true && chessBoard[r][s].getChessPiece().getTypePiece() == PAWN && chessBoard[r][s].getChessPiece().getColorPiece() != who)
            {
                Pawn pedoneDaCercare = Pawn();
                // i e j la casa da controllare, r e s quelle qualsiasi sulla scacchiera
                status = pedoneDaCercare.checkFork(i, j, r, s, who);
                if (status == true)
                {
                    return true;
                }
            }
        };
    }
    return status;
}
bool Pawn::checkFork(int i, int j, int m, int n, char who)
{
    bool status = false;
    // i e j sono le cordinate della casa da controllare se minacciata dal pedone
    // m  e n  sono le coordinate di qualsiasi casella dellascacchiera

    int distY = abs(i - m);
    int distX = abs(j - n);
    if (!(distX == 1 && distY == 1))
    {
        return false;
    }
    // se arriviamo qui le due distanze valgono 1
    // occorre solo assicurarsi che la casella da verificare sia davannti ai pedoni, altrimenti essi non sono una minaccia
    // ricordando che 0,0 sta in altoe tnendo conto anche del punto di vista
    // who il colore del re sotto attacco
    if (who == 'B' && puntoDiVista == "Bianco" && m > i && chessBoard[m][n].getChessPiece().getTypePiece() == PAWN && chessBoard[m][n].getBusySquare() == true)
    {
        return true;
    }
    if (who == 'B' && puntoDiVista == "Nero" && m < i && chessBoard[m][n].getChessPiece().getTypePiece() == PAWN && chessBoard[m][n].getBusySquare() == true)
    {
        return true;
    }

    if (who == 'W' && puntoDiVista == "Bianco" && m < i && chessBoard[m][n].getChessPiece().getTypePiece() == PAWN && chessBoard[m][n].getBusySquare() == true)
    {
        return true;
    }

    if (who == 'W' && puntoDiVista == "Nero" && m > i && chessBoard[m][n].getChessPiece().getTypePiece() == PAWN && chessBoard[m][n].getBusySquare() == true)
    {
        return true;
    }

    return status;
}

bool King::checkFreeSquare(int i, int j, char who)
{
    bool status = false;
    int countFreeSquare = 0;
    King king = King();
    char colorKing = who;
    // inizio
    if (i == 7)
    {
        if (j > 0 && king.checkSquareUnderAttack(i, j - 1, colorKing) == false)
        {
            countFreeSquare++;
            if (chessBoard[i][j - 1].getChessPiece().getColorPiece() == colorKing && chessBoard[i][j - 1].getBusySquare() == true)
            {
                countFreeSquare--;
            }
        }
        if (j < 7 && king.checkSquareUnderAttack(i, j + 1, colorKing) == false)
        {
            countFreeSquare++;
            if (chessBoard[i][j + 1].getChessPiece().getColorPiece() == colorKing && chessBoard[i][j + 1].getBusySquare() == true)
            {

                countFreeSquare--;
            }
        }
        if (j > 0 && king.checkSquareUnderAttack(i - 1, j - 1, colorKing) == false)
        {
            countFreeSquare++;
            if (chessBoard[i - 1][j - 1].getChessPiece().getColorPiece() == colorKing && chessBoard[i - 1][j - 1].getBusySquare() == true)
            {

                countFreeSquare--;
            }
        }
        if (king.checkSquareUnderAttack(i - 1, j, colorKing) == false)
        {
            countFreeSquare++;
            if (chessBoard[i - 1][j].getChessPiece().getColorPiece() == colorKing && chessBoard[i - 1][j].getBusySquare() == true)
            {

                countFreeSquare--;
            }
        }
        if (j < 7 && king.checkSquareUnderAttack(i - 1, j + 1, colorKing) == false)
        {
            countFreeSquare++;
            if (chessBoard[i - 1][j + 1].getChessPiece().getColorPiece() == colorKing && chessBoard[i - 1][j + 1].getBusySquare() == true)
            {

                countFreeSquare--;
            }
        }
    }
    if (i == 0)
    {
        // siamo in alto
        // ci sono 5 case da controllare
        if (j > 0 && king.checkSquareUnderAttack(i, j - 1, colorKing) == false)
        {
            countFreeSquare++;
            if (chessBoard[i][j - 1].getChessPiece().getColorPiece() == colorKing && chessBoard[i][j - 1].getBusySquare() == true)
            {

                countFreeSquare--;
            }
        }

        if (j < 7 && king.checkSquareUnderAttack(i, j + 1, colorKing) == false)
        {
            countFreeSquare++;
            if (chessBoard[i][j + 1].getChessPiece().getColorPiece() == colorKing && chessBoard[i][j + 1].getBusySquare() == true)
            {

                countFreeSquare--;
            }
        }
        if (j > 0 && king.checkSquareUnderAttack(i + 1, j - 1, colorKing) == false)
        {
            countFreeSquare++;
            if (chessBoard[i + 1][j - 1].getChessPiece().getColorPiece() == colorKing && chessBoard[i + 1][j - 1].getBusySquare() == true)
            {

                countFreeSquare--;
            }
        }
        if (king.checkSquareUnderAttack(i + 1, j, colorKing) == false)
        {
            countFreeSquare++;
            if (chessBoard[i + 1][j].getChessPiece().getColorPiece() == colorKing && chessBoard[i + 1][j].getBusySquare() == true)
            {

                countFreeSquare--;
            }
        }
        if (j < 7 && king.checkSquareUnderAttack(i + 1, j + 1, colorKing) == false)
        {
            countFreeSquare++;
            if (chessBoard[i + 1][j + 1].getChessPiece().getColorPiece() == colorKing && chessBoard[i + 1][j + 1].getBusySquare() == true)
            {

                countFreeSquare--;
            }
        }
    }
    if (i > 0 && i < 7)
    {
        // Le case sono 8

        if (j > 0 && king.checkSquareUnderAttack(i - 1, j - 1, colorKing) == false)
        {
            countFreeSquare++;
            if (chessBoard[i - 1][j - 1].getChessPiece().getColorPiece() == colorKing && chessBoard[i + 1][j + 1].getBusySquare() == true)
            {

                countFreeSquare--;
            }
        }
        if (king.checkSquareUnderAttack(i - 1, j, colorKing) == false)
        {
            countFreeSquare++;
            if (chessBoard[i - 1][j].getChessPiece().getColorPiece() == colorKing && chessBoard[i - 1][j].getBusySquare() == true)
            {

                countFreeSquare--;
            }
        }
        if (j < 7 && king.checkSquareUnderAttack(i - 1, j + 1, colorKing) == false)
        {
            countFreeSquare++;
            if (chessBoard[i - 1][j + 1].getChessPiece().getColorPiece() == colorKing && chessBoard[i - 1][j + 1].getBusySquare() == true)
            {

                countFreeSquare--;
            }
        }
        if (j > 0 && king.checkSquareUnderAttack(i, j - 1, colorKing) == false)
        {
            countFreeSquare++;
            if (chessBoard[i][j - 1].getChessPiece().getColorPiece() == colorKing && chessBoard[i][j - 1].getBusySquare() == true)
            {

                countFreeSquare--;
            }
        }
        if (j < 7 && king.checkSquareUnderAttack(i, j + 1, colorKing) == false)
        {
            countFreeSquare++;
            if (chessBoard[i][j + 1].getChessPiece().getColorPiece() == colorKing && chessBoard[i][j + 1].getBusySquare() == true)
            {

                countFreeSquare--;
            }
        }
        if (j > 0 && king.checkSquareUnderAttack(i + 1, j - 1, colorKing) == false)
        {
            countFreeSquare++;
            if (chessBoard[i + 1][j - 1].getChessPiece().getColorPiece() == colorKing && chessBoard[i + 1][j - 1].getBusySquare() == true)
            {

                countFreeSquare--;
            }
        }
        if (king.checkSquareUnderAttack(i + 1, j, colorKing) == false)
        {
            countFreeSquare++;
            if (chessBoard[i + 1][j].getChessPiece().getColorPiece() == colorKing && chessBoard[i + 1][j].getBusySquare() == true)
            {

                countFreeSquare--;
            }
        }
        if (j < 7 && king.checkSquareUnderAttack(i + 1, j + 1, colorKing) == false)
        {
            countFreeSquare++;
            if (chessBoard[i + 1][j + 1].getChessPiece().getColorPiece() == colorKing && chessBoard[i + 1][j + 1].getBusySquare() == true)
            {

                countFreeSquare--;
            }
        }
    }

    string msg;
    msg.clear();
    msg = "countFreeSquare " + to_string(countFreeSquare) + " \n";
    printDebug(msg);
    status = (countFreeSquare == 0 ? false : true);
    return status;
}

// utility per square,  riga e colonna
// questa utility serve principalmente
// a ruotare di 180 gradi la scacchiera
// ovvero a giocare dal lato del bianco o del nero
// ma chessBoard[][] ha sempre 0,0 in alto asinistra
// ruotano in essa le caselle (ChessSquare)mantenendo il nome algebrico
// ma associate a riga e colonna diverse in base al
// punto di Vista Bianco o Nero

string ChessUtility::getSquarePuntoDiVista(int i, int j, string pdVista)
{
    char appo[3];
    char x, y;

    if (pdVista == "Bianco")
    {
        x = j + 97;
        y = 7 - i + 48 + 1;
    }
    if (pdVista == "Nero")
    {
        x = 7 - j + 97;
        y = i + 1 + 48;
    }
    sprintf_s(appo, "%c%c", x, y);
    string square = string(appo);
    return square;
    ;
}

vector<int> ChessUtility::getIndexPuntoDiVista(string square, string pdVista)
{
    vector<int> index;
    index.clear();
    int i = 7 - (square[1] - 48 - 1);
    int j = square[0] - 97;
    int buf = j;

    if (pdVista == "Nero")
    {

        i = square[1] - 48 - 1;
        j = 7 - buf;
    }
    index.push_back(i);
    index.push_back(j);
    return index;
}

// oggetto fermacarte
Nothing::Nothing() {}
Nothing::~Nothing() {}
