
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "chess.h"
#include "globale.h"
#include "proto.h"
#include "engine.h"
using namespace std;

// undoTail è interno a undo.cpp
std::vector<string> undoTail;
// condiviso solo con questions.cpp
extern bool scaccoRimosso;
// condiviso solo con board.cpp
extern std::vector<string> undoPawnEnP;

void setPiece(string square, const char *pieceName, int numWho);

void insertUndoMove(char who, string src, string destination, int num, int numWho, ChessPiece pieceCatched, string operation, string srcEnp, string colorCatched)
{

    /*
    La funzione insertUndoMove crea una unica string con gli input separati da virgole, poi inserisce la string nel vector undoTail

    */
    string cmd = "";
    if (online)
    {
        deleteUndo();
        return;
    }
    if (isEngineRunning())
    {
        deleteUndo();
        return;
    }
    string pieceName = pieceCatched.getNamePiece();
    if ((operation == "cattura" || operation == "enpassant") && pieceName.empty())
    {
        callTextToSpeech(string("Nome del pezzo catturato non disponibile durante inserimento undo\n"));
        return;
    }
    cmd.push_back(who);
    string cmd2 = "," + src + "," + destination + "," + to_string(num) + "," + to_string(numWho) + "," + pieceName + "," + operation + "," + srcEnp + "," + colorCatched;
    cmd.append(cmd2);
    string msg = cmd + "\n";

    undoTail.push_back(cmd);
    return;
}

void undoMove()
{

    /*
    La funzione annulla una mossa e rida il turno a chi si è visto annullare la mossa.
    char who viene tradotto in whoStr e
    int numWho rappresenta la mossa a chi appartiene, se 'W' è del giocatore bianco, se 'B' è del giocatore nero.
    Il termine int    numWho corrisponde a int startColor attuale che può valere 0 o 1 e va rimesso indietro: se 0 va rimesso a 1, se 1 va rimesso a 0.
    Le operation possibili sono:
    "mossa","cattura","enpassant","arrocco","promozione".
    Nel caso dell'arrocco deve essere settato di nuovo a valido l'arrocco relativo.
    Mentre deve essere messo a posto il pezzo nella src, rimettendo a posto eventuali pezzi catturati per cattura, enpassant. Anche nel caso di promozione si toglie il pezzo dalla destinazione e si mette il pedone nella casa src.
    Per gli arrocchi si utilizzano le variabili globali:
    tipoArrocco
    startRookRow
    startRookCol
    endRookRow
    endRookCol
Per rendere di nuovo validi gli arrocchi occorre usare setValid00 e setValid000
    */
    if (undoTail.size() == 0)
    {
        callTextToSpeech(string("Non vi sono mosse da annullare\n"));
        return;
    }
    if (isEngineRunning())
    {
        deleteUndo();
        callTextToSpeech(string("Non è possibile annullare la mossa durante la partita contro l'engine\n"));
        return;
    }

        // sviluppo della funzione
    string cmd = undoTail.back();
    undoTail.pop_back();
    stringstream ss(cmd);
    string msg = cmd + "\n";
    string src, destination, pieceCatchedName, operation;
    int num, numWho;
    string whoStr;
    string srcEnp;
    string colorCatched;
    getline(ss, whoStr, ','); // leggi il carattere 'W' o 'B'
    getline(ss, src, ',');
    getline(ss, destination, ',');

    string numStr, numWhoStr;
    getline(ss, numStr, ',');
    num = stoi(numStr);
    getline(ss, numWhoStr, ',');
    numWho = stoi(numWhoStr);

    getline(ss, pieceCatchedName, ',');

    if (pieceCatchedName.empty())
    {
        callTextToSpeech(string("Errore pezzo catturato non inserito"));
        exit(0);
    }
    getline(ss, operation, ',');

    getline(ss, srcEnp, ',');

    getline(ss, colorCatched, ',');

    if ((operation == "cattura" || operation == "enpassant") && pieceCatchedName.empty())
    {
        callTextToSpeech(string("Nome del pezzo catturato non ricevuto, impossibile ripristinare correttamente la cattura\n"));
        return;
    }

    startColor = numWho;
    int capturedColor = (numWho == 0 ? 1 : 0);

    if (operation == "mossa")
    {
        // sposto il pezzo indietro
        movePiece(destination, src);
    }
    else if (operation == "cattura")
    {
        // sposto il pezzo indietro e rimetto a posto il pezzo catturato
        movePiece(destination, src);
        removePiece(destination);
        if (colorCatched == "Bianco")
        {
            capturedColor = 0;
        }
        else
        {
            capturedColor = 1;
        }
        setPiece(destination, pieceCatchedName.c_str(), capturedColor);
    }
    else if (operation == "enpassant")
    {

        // Ricarico vettore pawnEnP, metto EnP a true e rigenero casellaDaCatturare
        for (const std::string &elemento : undoPawnEnP)
        {
            pawnEnP.push_back(elemento);
        }
        EnP = true;
        // non cancello undoPawnEnP per gestire i ritorni indietro di più enpassant
        // viene cancellato allo start o dalla deleteUndo
        // sposto il pezzo indietro e rimetto a posto il pezzo catturato
        string pedone = "Pedone";
        pedone.append(colorCatched);
        int colorePedone = 0;
        if (colorCatched == "Bianco")
        {
            colorePedone = 0;
        }
        else
        {
            colorePedone = 1;
        }
        casellaDaCatturare = destination;
        movePiece(destination, src);
        removePiece(destination);

        setPiece(srcEnp, pedone.c_str(), colorePedone);
    }
    else if (operation == "arrocco")
    {
        // sposto il re indietro e rimetto a posto la torre
        movePiece(destination, src);
        movePiece(endRookRow, endRookCol, startRookRow, startRookCol);
    }
    else if (operation == "promozione")
    {
        // tolgo il pezzo dalla destinazione e metto il pedone nella casa src

        ChessUtility utility;
        vector<int> Ind = utility.getIndexPuntoDiVista(destination, puntoDiVista);
        numWho = (chessBoard[Ind.front()][Ind.back()].getChessPiece().getColorPiece() == 'W' ? 0 : 1);
        removePiece(destination);
        setPiece(src, pieceCatchedName.c_str(), numWho);
        // se la promozione aveva provocato scacco
        if (isScaccoRe)
        {
            isScaccoRe = false;
        }
    }
    else
    {
        callTextToSpeech(string("Operazione non riconosciuta\n"));
        return;
    }
    // se ho annullato arrocco devo riabilitarlo
    if (operation == "arrocco")
    {
        if (tipoArrocco == "arrocco corto")
        {
            setValid00();
        }
        else if (tipoArrocco == "arrocco lungo")
        {
            setValid000();
        }
    }
    // devo rimettere startColor in modo che il giocatore possa rifare la mossa
    if (whoStr == "W")
    {
        startColor = 0;
        who_play = 'W';
        numMove--;
    }
    else
    {
        startColor = 1;
        who_play = 'B';
    }

    // pulisco case di PieceNoTouch che risultano vuote
    cleanSquare();

    if (scaccoRimosso == true)
    {
        isScaccoRe = true;
        scaccoRimosso = false;
    }
    callTextToSpeech(string("Mossa annullata\n"));
    drawBoard();
    return;
}
void deleteUndo()
{
    /*
    La funzione deleteUndo svuota il vector undoTail, da utilizzare quando si inizia una nuova partita.
    */
    undoTail.clear();
    undoPawnEnP.clear();
    return;
}
// Helper functions for undo operations
void movePiece(string src, string dest)
{
    ChessUtility utility;
    vector<int> Idx_src = utility.getIndexPuntoDiVista(src, puntoDiVista);
    vector<int> Idx_dest = utility.getIndexPuntoDiVista(dest, puntoDiVista);
    int si = Idx_src.front();
    int sj = Idx_src.back();
    int di = Idx_dest.front();
    int dj = Idx_dest.back();

    // Get the piece from source square
    ChessSquare casS = chessBoard[si][sj];
    ChessPiece piece = casS.getChessPiece();

    // Create destination square with the piece
    ChessSquare casD(dest.c_str());
    casD.setChessPiece(piece);
    casD.setBusySquare(true);
    chessBoard[di][dj] = casD;

    // Clear source square
    ChessSquare casEmpty(src.c_str());
    casEmpty.setBusySquare(false);
    chessBoard[si][sj] = casEmpty;
}

void movePiece(int srow, int scol, int drow, int dcol)
{
    ChessSquare casS = chessBoard[srow][scol];
    ChessSquare casD = casS;
    casD.setBusySquare(true);
    chessBoard[drow][dcol] = casD;
    casS.setBusySquare(false);
    chessBoard[srow][scol] = casS;
}

void removePiece(string square)
{
    ChessUtility utility;
    vector<int> Idx = utility.getIndexPuntoDiVista(square, puntoDiVista);
    int i = Idx.front();
    int j = Idx.back();
    ChessSquare cas = ChessSquare(square.c_str());
    cas.setBusySquare(false);
    chessBoard[i][j] = cas;
}

void setValid00()
{
    ChessUtility utility;
    vector<int> ih1 = utility.getIndexPuntoDiVista(string("h1"), puntoDiVista);
    vector<int> ih8 = utility.getIndexPuntoDiVista(string("h8"), puntoDiVista);
    chessBoard[ih1.front()][ih1.back()].setValid00(true);
    chessBoard[ih8.front()][ih8.back()].setValid00(true);
}

void setValid000()
{
    ChessUtility utility;
    vector<int> ia1 = utility.getIndexPuntoDiVista(string("a1"), puntoDiVista);
    vector<int> ia8 = utility.getIndexPuntoDiVista(string("a8"), puntoDiVista);
    chessBoard[ia1.front()][ia1.back()].setValid000(true);
    chessBoard[ia8.front()][ia8.back()].setValid000(true);
}

static string normalizePieceName(const string &name)
{
    string lower = name;
    for (char &c : lower)
    {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return lower;
}
void setPiece(string square, const char *pieceName, int numWho)
{
    // numWho: 0 -> white, else black
    char color = (numWho == 0 ? 'W' : 'B');
    string name = pieceName;
    string lower = name;
    ChessSquare cas = ChessSquare(square.c_str());
    if (lower.find("Pedone") != string::npos)
    {
        Pawn p(name.c_str(), color);
        cas.setChessPiece(p);
    }
    else if (lower.find("Torre") != string::npos)
    {
        Rook r(name.c_str(), color);
        cas.setChessPiece(r);
    }
    else if (lower.find("Cavallo") != string::npos)
    {
        Knight k(name.c_str(), color);
        cas.setChessPiece(k);
    }
    else if (lower.find("Alfiere") != string::npos)
    {
        Bishop b(name.c_str(), color);
        cas.setChessPiece(b);
    }
    else if (lower.find("Donna") != string::npos || lower.find("regina") != string::npos)
    {
        Queen q(name.c_str(), color);
        cas.setChessPiece(q);
    }
    else if (lower.find("Re") != string::npos)
    {
        King k(name.c_str(), color);
        cas.setChessPiece(k);
    }
    else
    {
        Nothing n;
        cas.setChessPiece(n);
    }
    cas.setBusySquare(true);
    ChessUtility utility;
    vector<int> Idx = utility.getIndexPuntoDiVista(square, puntoDiVista);
    chessBoard[Idx.front()][Idx.back()] = cas;
}
