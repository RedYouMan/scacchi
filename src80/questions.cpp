// questions.cpp
// determina se scacco, scacco matto o stallo
#include <windows.h>
#include <iostream>
#include "globale.h"
#include "chess.h"
#include <string>
#include <vector>
#include "proto.h"
using namespace std;
// condiviso solo con undo.cpp
bool scaccoRimosso = false;

void cheScacco(int m, int n, char who)
{
    /*
    m e n coordinate del pezzo che fa scacco e who il colore
 dell'attaccante
 1= E' scacco se non è matto
    2) è scacco matto se sono vere le condizioni
    a) non ci sono vie di fuga libere
    b nessun pezzo può parare lo scacco o mangiare il pezzo avversario che fa scacco
    oppure il pezzo nemico in m e n è difeso
    */

    ultimaMossa.clear();
    printDebug(string("\nanalisi di 'cheScacco'\n"));

    string msg;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (chessBoard[x][y].getBusySquare() == true && chessBoard[x][y].getChessPiece().getTypePiece() == KING && chessBoard[x][y].getChessPiece().getColorPiece() != who)
            {
                King king_da_verificare = King();
                char UsecolorOfKing = chessBoard[x][y].getChessPiece().getColorPiece();
                if (king_da_verificare.checkSquareUnderAttack(x, y, UsecolorOfKing))
                {
                    msg.clear();
                    msg = "Scacco: verifichiamo se solo scacco al Re o scacco matto\n";
                    // callTextToSpeech(string("debug voice: scacco, verifichiamo se scacco al re o matto"));
                    printf("%s", msg.c_str());
                    string tipoMossa;
                    tipoMossa.clear();
                    if (king_da_verificare.checkFreeSquare(x, y, UsecolorOfKing) == false)
                    {
                        int distO = abs(x - m);
                        int distV = abs(y - n);

                        /*
                        devo verificare se scacco parabile o pezzo non difeso
                        Il concetto è che un pezzo è difeso se la casa in cui va è minacciata da un suo pezzo amico
*/

                        char color = chessBoard[m][n].getChessPiece().getColorPiece();
                        if ((distO <= 1 && distV <= 1 && !isPieceDefended(m, n, color)) || isPiecePresent(m, n, x, y, UsecolorOfKing))
                        {
                            // è parabile o catturabile quindi non è matto
                            // callTextToSpeech(string("debug voice:scacco parabile"));

                            isScaccoRe = true;

                            printf("Scacco parabile o pezzo catturabile\n");
                            callTextToSpeech(string("Scacco al Re!\n"));
                            tipoMossa = "+ ";
                            ultimaMossa.append(tipoMossa);
                            return;
                        }
                        callTextToSpeech(string("Scacco Matto!\n"));
                        invioMossaRete();
                        sendMove(roomOnline, playerOnline, "Matto");

                        tipoMossa = "#";
                        ultimaMossa.append(tipoMossa);
                        WriteGameToFile(nameFile, ultimaMossa, reg_to_file);
                        printf("%s\n", ultimaMossa.c_str());
                        ultimaMossa.clear();
                        printf("Partita terminata\n");
                        callTextToSpeech(string("Partita terminata"));
                        reStart();
                    }
                    else
                    {
                        isScaccoRe = true;
                        printf("Scacco al Re!\n");
                        callTextToSpeech(string("Scacco al Re!\n"));
                        tipoMossa = "+ ";
                        ultimaMossa.append(tipoMossa);

                        return;
                    }
                }
            }
        }
    }
    return;
}
bool isPiecePresent(int m, int n, int x, int y, char who)
{

    bool status = false;
    /*
    cerca un pezzo che possa parare lo scacco o mangiare avversario
    */

    // qui who è il colore del re sotto scacco

    int distO = abs(m - x);
    int distV = abs(n - y);

    // scansiono la scacchiera alla ricerca di pezzi che possono catturare in m,n

    printf("controllo se esistono pezzi che possono catturare il pezzo avversario\n");
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (chessBoard[i][j].getBusySquare() == true && chessBoard[i][j].getChessPiece().getColorPiece() == who)
            {
                if (chessBoard[i][j].getChessPiece().getTypePiece() == KING)
                {

                    King pezzo = King();
                    // solo nel caso del re devo controllare che il pezzo sia indifeso, altrimenti non catturabile dal re
                    char useColor = chessBoard[i][j].getChessPiece().getColorPiece();
                    // modifica ifper cavallomattoz
                    char color = chessBoard[m][n].getChessPiece().getColorPiece();

                    // if (pezzo.checkMove(chessBoard[i][j].getSquareName(), chessBoard[m][n].getSquareName()) && !isPieceDefended(m, n, chessBoard[m][n].getChessPiece().getTypePiece(), who))
                    // if (distO <= 1 && distV <= 1 && pezzo.checkMove(chessBoard[i][j].getSquareName(), chessBoard[m][n].getSquareName()))

                    if (distO <= 1 && distV <= 1 && !isPieceDefended(m, n, color))
                    {
                        // trovato un pezzo che può muoversi in m,n
                        printf("trovato il re in %d %d\n", i, j);
                        status = true;
                        break;
                    }
                }
                if (chessBoard[i][j].getChessPiece().getTypePiece() == BISHOP)
                {
                    Bishop pezzo = Bishop();
                    if (pezzo.checkDiagonale(chessBoard[i][j].getSquareName(), chessBoard[m][n].getSquareName(), false))
                    {

                        printDebug(string("trovato alfiere in " + to_string(i) + " " + to_string(j) + "\n"));
                        status = true;
                        break;
                    }
                }

                if (chessBoard[i][j].getChessPiece().getTypePiece() == QUEEN)
                {
                    Queen pezzo = Queen();
                    if (pezzo.checkDiagonale(chessBoard[i][j].getSquareName(), chessBoard[m][n].getSquareName(), false) ||
                        pezzo.checkTraversa(chessBoard[i][j].getSquareName(), chessBoard[m][n].getSquareName(), false))
                    {
                        // trovato un pezzo che può muoversi in m,n
                        printDebug(string("trovata donna in " + to_string(i) + " " + to_string(j) + "\n"));
                        status = true;
                        break;
                    }
                }
                if ((distO < 3 || distV < 3) && chessBoard[i][j].getChessPiece().getTypePiece() == KNIGHT)
                {
                    Knight pezzo = Knight();
                    if (pezzo.checkJump(chessBoard[i][j].getSquareName(), chessBoard[m][n].getSquareName(), false))
                    {
                        // trovato un pezzo che può muoversi in m,n

                        printDebug(string("trovato cavallo in " + to_string(i) + " " + to_string(j) + "\n"));
                        status = true;
                        break;
                    }
                }

                if (chessBoard[i][j].getChessPiece().getTypePiece() == ROOK)
                {
                    Rook pezzo = Rook();
                    if (pezzo.checkTraversa(chessBoard[i][j].getSquareName(), chessBoard[m][n].getSquareName(), false))
                    {
                        // trovato un pezzo che può muoversi in m,n
                        printDebug(string("trovata torre in " + to_string(i) + " " + to_string(j) + "\n"));
                        status = true;
                        break;
                    }
                }
                if (chessBoard[i][j].getChessPiece().getTypePiece() == PAWN)
                {
                    Pawn pezzo = Pawn();
                    if (pezzo.checkFork(i, j, m, n, who))
                    {
                        // trovato un pezzo che può muoversi in m,n
                        printDebug(string("trovato pedone in " + to_string(i) + " " + to_string(j) + "\n"));
                        status = true;
                        break;
                    }
                }
            }
        }
    }
    // seconda parte
    // scansiono la scacchiera per cercare pezzi che possono parare lo scacco proveniente da m,n controllando checkDiagonale o checkTraversa, o checkJump o checkFork
    if (status == false)
    {
        status = isPieceIntercept(m, n, x, y, who);
    }

    if (status)
    {
    }
    return status;
}
void cheStallo(char who)
{
    /*
who è il colore del giocatore che deve muovere
    se non è scacco al re e non ci sono mosse legali per il giocatore che deve muovere
    allora è stallo
    */

    printDebug(string("analisi di 'cheStallo'\n"));
    // tuttavia si verifica prima se ha altre mosse
    int numBianchi = 0, numNeri = 0;
    // devo anche verificare che abbia pezzi e pedoni che possa muovere
    // se sotto scacco non è in stallo altrimenti saremmo usciti per matto.
    if (isScaccoRe)
    {
        return;
    }

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {

            if (chessBoard[x][y].getBusySquare() && chessBoard[x][y].getChessPiece().getColorPiece() == 'W' && chessBoard[x][y].getChessPiece().getTypePiece() != PAWN && chessBoard[x][y].getChessPiece().getTypePiece() != KING)
            {
                numBianchi++;
            }
            if (chessBoard[x][y].getBusySquare() && chessBoard[x][y].getChessPiece().getColorPiece() == 'B' && chessBoard[x][y].getChessPiece().getTypePiece() != PAWN && chessBoard[x][y].getChessPiece().getTypePiece() != KING)
            {
                numNeri++;
            }
            if (puntoDiVista == "Bianco" && chessBoard[x][y].getBusySquare() && chessBoard[x][y].getChessPiece().getTypePiece() == PAWN && chessBoard[x][y].getChessPiece().getColorPiece() == 'W' && chessBoard[x - 1][y].getBusySquare() == false)
            {
                numBianchi++;
            }
            if (puntoDiVista == "Bianco" && chessBoard[x][y].getBusySquare() && chessBoard[x][y].getChessPiece().getTypePiece() == PAWN && chessBoard[x][y].getChessPiece().getColorPiece() == 'B' && chessBoard[x + 1][y].getBusySquare() == false)
            {
                numNeri++;
            }
            if (puntoDiVista == "Nero" && chessBoard[x][y].getBusySquare() && chessBoard[x][y].getChessPiece().getTypePiece() == PAWN && chessBoard[x][y].getChessPiece().getColorPiece() == 'B' && chessBoard[x - 1][y].getBusySquare() == false)
            {
                numNeri++;
            }
            if (puntoDiVista == "Nero" && chessBoard[x][y].getBusySquare() && chessBoard[x][y].getChessPiece().getTypePiece() == PAWN && chessBoard[x][y].getChessPiece().getColorPiece() == 'W' && chessBoard[x + 1][y].getBusySquare() == false)
            {
                numBianchi++;
            }
        }
    }

    int num = (who == 'W' ? numBianchi : numNeri);

    if (num > 0)
    {
        // Puoi muovere altri pezzi oltre al Re
        return;
    }
    printf("verifichiamo se il re si può muovere\n");
    // si controlla ora se è stallo
    string msg;
    bool isStallo = false;

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {

            if (chessBoard[x][y].getBusySquare() == true && chessBoard[x][y].getChessPiece().getTypePiece() == KING && chessBoard[x][y].getChessPiece().getColorPiece() == who)
            {
                King king_da_verificare = King();
                if (king_da_verificare.checkFreeSquare(x, y, who) == false)
                {
                    callTextToSpeech(string("stallo!\n"));
                    callTextToSpeech(string("Patta per stallo! Partita terminata.\n"));
                    ultimaMossa.append("1/2-1/2");
                    WriteGameToFile(nameFile, ultimaMossa, reg_to_file);
                    isStallo = true;
                }
            }
            // 2 for di chiusura
        }
    }

    if (isStallo)
    {
        callTextToSpeech(string("Partita terminata pari"));
        sendMove(roomOnline, playerOnline, "Stallo");
        reStart();
    }

    return;
}
int checkMoveKing(int m, int n, int p, int q, char who)
{
    // p e q sono src di un pezzo mosso al posto del re
    // m e n la destination
    bool check = false;
    bool isPiecePresent = false;
    scaccoRimosso = false;
    bool reset = false;
    jumpFalsePiece = false; // usato per saltare oggetti Nothing altrimenti risultano catturati
    ChessPiece pieceCatched;
    bool cattura_ok = false;
    int x = 0, y = 0, x_re = 0, y_re = 0;
    string coloreCattura = "";

    callTextToSpeech(string("Verifico che il re si sia tolto dallo scacco"));

    King re = King();

    /*
     se muovo altro pezzo m ed n non sono le coordinate del re e devo trovarle
    */

    // inizio a considerare se in p e q ci sia il re

    ChessUtility utilityKing;
    string srcKing = utilityKing.getSquarePuntoDiVista(p, q, puntoDiVista);
    string destKing = utilityKing.getSquarePuntoDiVista(m, n, puntoDiVista);

    if (chessBoard[p][q].getChessPiece().getTypePiece() == KING && chessBoard[p][q].getBusySquare() && chessBoard[p][q].getChessPiece().getColorPiece() == who)
    {
        check = re.checkMove(srcKing, destKing);
        if (check)
        {
            int val = (who == 'W' ? 0 : 1);
            coloreCattura.clear();
            if (chessBoard[m][n].getChessPiece().getColorPiece() != who)
            {
                if (chessBoard[m][n].getChessPiece().getColorPiece() == 'W')
                {
                    coloreCattura = "Bianco";
                }
                else
                {
                    coloreCattura = "Nero";
                }
                insertUndoMove(who, srcKing, destKing, startColor, val, chessBoard[m][n].getChessPiece(), "cattura", "--", coloreCattura);
                jumpFalsePiece = true;
                isScaccoRe = false;
                scaccoRimosso = true;
                callTextToSpeech(string("catturato ") + chessBoard[m][n].getChessPiece().getNamePiece());
            }
            return 1;
        }
    }
    if (chessBoard[p][q].getChessPiece().getTypePiece() == KING && chessBoard[p][q].getBusySquare() && re.checkSquareUnderAttack(p, q, who) == true)
    {
        callTextToSpeech(string("Re ancora sotto scacco"));
        return 0;
    }
    // non si tratta del re
    if (chessBoard[p][q].getChessPiece().getTypePiece() != KING)
    {

        reset = true;
        x = m;
        y = n;
        if (chessBoard[m][n].getBusySquare() == false)
        {
            // userò un oggetto nothing che non deve essere rilevato come cattura da board.cpp
            jumpFalsePiece = true;
        }
        if (chessBoard[m][n].getBusySquare() == true)
        {
            string msg_d1 = "verifichiamo se ";
            string msg_d2;
            msg_d2.append(chessBoard[m][n].getChessPiece().getNamePiece());
            pieceCatched = chessBoard[m][n].getChessPiece();
            isPiecePresent = true;
            if (chessBoard[m][n].getChessPiece().getColorPiece() == 'W')
            {
                coloreCattura = "Bianco";
            }
            else
            {
                coloreCattura = "Nero";
            }
            msg_d1.append(msg_d2);
            msg_d1.append(" sia catturabile");
            callTextToSpeech(msg_d1);
        }

        // oggetto segna posto per la verifica se lo scacco è stato risolto
        Nothing boh = Nothing();
        ChessSquare casellaDest = chessBoard[x][y];
        casellaDest.setChessPiece(boh);
        casellaDest.setBusySquare(true);
        chessBoard[x][y] = casellaDest;
        // cerco le reali coordinate del re
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (chessBoard[i][j].getBusySquare() && chessBoard[i][j].getChessPiece().getTypePiece() == KING && chessBoard[i][j].getChessPiece().getColorPiece() == who)
                {
                    x_re = i;
                    y_re = j;
                    break;
                }
            }
        }
    }

    // se la casa del re è sotto attacco
    if (re.checkSquareUnderAttack(x_re, y_re, who) == true)
    {
        isScaccoRe = true;
        callTextToSpeech(string("Attenzione: Il re ancora sotto scacco"));
        if (reset == true)
        {
            chessBoard[x][y].setBusySquare(false);
        }

        return 0;
    }
    else
    {
        if (isPiecePresent)
        {
            cattura_ok = true;
            isPiecePresent = false;
        }
    }

    if (cattura_ok)
    {
        // si può catturare
        jumpFalsePiece = true;
        isScaccoRe = false;
        scaccoRimosso = true;
        ChessUtility utilitySrc, utilityDest;
        string Src = utilitySrc.getSquarePuntoDiVista(p, q, puntoDiVista);
        string Dest = utilityDest.getSquarePuntoDiVista(x, y, puntoDiVista);

        callTextToSpeech(string("pezzocatturato  ") + pieceCatched.getNamePiece());
        int numWho = (who == 'W' ? 0 : 1);
        insertUndoMove(who, Src, Dest, startColor, numWho, pieceCatched, "cattura", "--", coloreCattura);
    }
    // isScaccoRe = false;
    //  altro pezzo diverso da re in m,n presente
    if (reset == true && cattura_ok == false)
    {
        jumpFalsePiece = true;
        isScaccoRe = false;
        scaccoRimosso = true;

        ChessUtility utility;
        string casaInd = utility.getSquarePuntoDiVista(x, y, puntoDiVista);
        string sorg = utility.getSquarePuntoDiVista(p, q, puntoDiVista);

        PieceNoTouch[IndiceNoTouch] = casaInd;
        IndiceNoTouch++;
        int chi = (chessBoard[x][y].getChessPiece().getColorPiece() == 'W' ? 0 : 1);
        insertUndoMove(who, sorg, casaInd, startColor, chi, chessBoard[x][y].getChessPiece(), "mossa");
    }

    return 1;
}

bool isPieceIntercept(int m, int n, int x, int y, char who)
{

    bool status = false;
    /*
    La funzione cerca su tutta la scacchiera un pezzo di colore who,
    che si possa interporre tra m,n e le coordinate del re sotto attacco (x,y) purchè la distanza trax,y e m,n sia maggiore di 1.
    */
    int distO = abs(x - m);
    int distV = abs(y - n);
    ChessUtility utility;
    string src, dest;
    ChessPiece pezzo;

    printf("controllo se esistono pezzi che possono interporrsi tra attaccante e re sotto scacco\n");
    /*
    devo cercare su tutta la scacchiera un pezzo che partendo da i,j possa mettersi sulla diagonale tra m,n e x,y
    o sulla traversa  tra m,n e x,y
    */

    // deve esserci spazio tra re e attaccante per intromettere un pezzo

    if (distO > 1 || distV > 1)
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (chessBoard[i][j].getBusySquare() == true && chessBoard[i][j].getChessPiece().getColorPiece() == who)
                {
                    pezzo = chessBoard[i][j].getChessPiece();
                    src = utility.getSquarePuntoDiVista(i, j, puntoDiVista);

                    // se src esiste nel array PieceNoTouch[] si il salta pezzo sta parando già uno scacco al re
                    if (searchPieceNoTouch(src) == true)
                    {
                        continue;
                    }

                    // determino la direzionedello scacco: diagonale o traversa
                    if (abs(x - m) == abs(y - n))
                    {
                        // diagonale
                        printDebug(string("caso scacco in diagonale\n"));
                        if (chessBoard[i][j].getChessPiece().getTypePiece() == BISHOP || chessBoard[i][j].getChessPiece().getTypePiece() == QUEEN)
                        {
                            // costruisco la diagonale col for tra m,n e x,y
                            // e devo verificare se partendo da i,j riesco ad arrivare in una casa del for
                            int startx = 0, starty = 0, endx = 0, endy = 0;
                            int incrx = 0, incry = 0;
                            // x,y il re m,n l'attaccante

                            // predispongo i termini del for
                            if (x < m)
                            {
                                // re sopra e parto dall'attaccante decrementando nel for
                                startx = m;
                                starty = n;
                                endx = x;
                                endy = y;

                                incrx = -1;
                                if (y < n)
                                {
                                    // re a sinistra sopra
                                    incry = -1;
                                }
                                else
                                {
                                    // re a destra sopra
                                    incry = +1;
                                }
                            }
                            else
                            {
                                // quando il re è sooto parto dal re e decremento
                                startx = x;
                                starty = y;
                                endx = m;
                                endy = n;
                                incrx = -1;
                                if (y < n)
                                {
                                    // re a sinisra quindi salendo aumentano le lettere
                                    incry = +1;
                                }
                                else
                                {
                                    // re a destra diminuiscono le lettere salendo
                                    incry = -1;
                                }
                            }
                            // due for a causa delle disuguagliaze del for, viene attraversato solo uno in base al gioco

                            // i for ognuno per la propria disuguaglianza partendo da un pezzo trovato
                            // costruiscono la diagonale tra re e attaccante e per ogni casa costruita determinano se essa è raggiungibile da un proprio pezzo di colore who
                            // dalle case vanno escluse quella del re e dell'attaccante
                            for (int p = startx + incrx, q = starty + incry; p > endx, q > endy; p = (p + incrx), q = (q + incry))
                            {

                                printDebug(string("1 - casa diagonale " + to_string(p) + " " + to_string(q) + "\n"));
                                // dest casa della diagonale da verificare se raggiungibile da un pezzo
                                dest = utility.getSquarePuntoDiVista(p, q, puntoDiVista);
                                if ((pezzo.getTypePiece() == BISHOP || pezzo.getTypePiece() == QUEEN) && pezzo.checkDiagonale(src, dest, false) == true)
                                {
                                    // trovato
                                    printf("individuato %s in %s che può interferire sulla diagonale\n", chessBoard[i][j].getChessPiece().getNamePiece(), src.c_str());
                                    return (true);
                                }
                                if ((pezzo.getTypePiece() == ROOK || pezzo.getTypePiece() == QUEEN) && pezzo.checkTraversa(src, dest, false) == true)
                                {
                                    // trovato
                                    printf("individuato %s in %s che può interferire sulla diagonale \n", chessBoard[i][j].getChessPiece().getNamePiece(), src.c_str());
                                    return (true);
                                }
                                if ((pezzo.getTypePiece() == KNIGHT) && pezzo.checkJump(src, dest, false) == true)
                                {
                                    // trovato
                                    printf("individuato %s in %s che può interferire sulla diagonale\n", chessBoard[i][j].getChessPiece().getNamePiece(), src.c_str());
                                    return (true);
                                }
                            }
                            // secondo for
                            for (int p = startx + incrx, q = starty + incry; p < endx, q < endy; p = (p + incrx), q = (q + incry))
                            {

                                printDebug(string("2 casa diagonale " + to_string(p) + " " + to_string(q) + "\n"));
                                dest = utility.getSquarePuntoDiVista(p, q, puntoDiVista);
                                if ((pezzo.getTypePiece() == BISHOP || pezzo.getTypePiece() == QUEEN) && pezzo.checkDiagonale(src, dest, false) == true)
                                {
                                    // trovato
                                    printf("individuato %s in %s che può interferire sulla diagonale\n", chessBoard[i][j].getChessPiece().getNamePiece(), src.c_str());
                                    return (true);
                                }
                                if ((pezzo.getTypePiece() == KNIGHT) && pezzo.checkJump(src, dest, false) == true)
                                {
                                    // trovato
                                    printf("individuato %s in %s che può interferire sulla diagonale\n", chessBoard[i][j].getChessPiece().getNamePiece(), src.c_str());
                                    return (true);
                                }
                                if ((pezzo.getTypePiece() == ROOK || pezzo.getTypePiece() == QUEEN) && pezzo.checkTraversa(src, dest, false) == true)
                                {
                                    // trovato
                                    printf("individuato %s in %s che può interferire sulla diagonale\n", chessBoard[i][j].getChessPiece().getNamePiece(), src.c_str());
                                    return (true);
                                }
                            }
                        }
                    }
                    // se no si tratta di traversa colonna o traversa riga
                    if (x == m)
                    {
                        // traversa riga
                        printDebug(string("caso scacco di traversa riga\n"));
                        int startrx = min(y, n);
                        int endx = max(y, n);
                        for (int p = startrx + 1; p < endx; p++)
                        {
                            dest = utility.getSquarePuntoDiVista(x, p, puntoDiVista);

                            printDebug(string("casa traversa " + to_string(x) + " " + to_string(p) + "\n"));
                            if ((pezzo.getTypePiece() == ROOK || pezzo.getTypePiece() == QUEEN) && pezzo.checkTraversa(src, dest, false) == true)
                            {
                                printf("individuato %s in %s che può interferire su traversa riga\n", chessBoard[i][j].getChessPiece().getNamePiece(), src.c_str());
                                return (true);
                            }
                            if ((pezzo.getTypePiece() == BISHOP || pezzo.getTypePiece() == QUEEN) && pezzo.checkDiagonale(src, dest, false) == true)
                            {
                                printf("individuato %s in %s che può interferire su traversa riga\n", chessBoard[i][j].getChessPiece().getNamePiece(), src.c_str());
                                return (true);
                            }
                            if ((pezzo.getTypePiece() == KNIGHT) && pezzo.checkJump(src, dest, false) == true)
                            {
                                printf("individuato %s in %s che può interferire su traversa riga\n", chessBoard[i][j].getChessPiece().getNamePiece(), src.c_str());
                                return (true);
                            }
                        }
                    }

                    // seconda parte
                    if (y == n)
                    {
                        // traversa colonna
                        printDebug(string("caso traversa colonna\n"));
                        int startcy = min(x, m);
                        int endx = max(x, m);
                        for (int q = startcy + 1; q < endx; q++)
                        {
                            dest = utility.getSquarePuntoDiVista(q, y, puntoDiVista);

                            printDebug(string("casa traversa " + to_string(q) + " " + to_string(y) + "\n"));
                            if ((pezzo.getTypePiece() == ROOK || pezzo.getTypePiece() == QUEEN) && pezzo.checkTraversa(src, dest, false) == true)
                            {
                                printf("individuato %s in %s che può interferire su traversa colonna\n", chessBoard[i][j].getChessPiece().getNamePiece(), src.c_str());
                                return (true);
                            }
                            if ((pezzo.getTypePiece() == BISHOP || pezzo.getTypePiece() == QUEEN) && pezzo.checkDiagonale(src, dest, false) == true)
                            {
                                printf("individuato %s in %s che può interferire su traversa colonna\n", chessBoard[i][j].getChessPiece().getNamePiece(), src.c_str());
                                return (true);
                            }
                            if ((pezzo.getTypePiece() == KNIGHT) && pezzo.checkJump(src, dest, false) == true)
                            {
                                printf("individuato %s in %s che può interferire su traversa colonna\n", chessBoard[i][j].getChessPiece().getNamePiece(), src.c_str());
                                return (true);
                            }
                        }
                    }
                    // fine for
                }

                // fine dei for e if
            }
        }
    }
    return status;
}
/*
La funzione algebricNotation annota secondo lo standard algebrico una partita.
In caso che una mossa si può fare con due pezzi dello stesso tipo, cosa che si verifica solo con torri e cavalli, usa la prima lettera della casa di partenza per distinguere i pezzi, altrimenti si limita a scrivere la mossa.
Ad esempio se c'è sia la torre in a1 che la torre in f1 che possono andare in c1 e dobbiamo dire che a muoversi è la torre in colonna a allora la mossa viene registrata come Tac1.
*/
bool isAmbiguousMove(string src, string dest, char pieceLetter)
{
    // Verifica se la mossa è ambigua (due pezzi dello stesso tipo possono muoversi nella stessa destinazione)
    int count = 0;
    ChessUtility utility;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (chessBoard[i][j].getBusySquare() == true)
            {
                ChessPiece pezzo = chessBoard[i][j].getChessPiece();

                // Controlla se è lo stesso tipo di pezzo
                if ((pieceLetter == 'T' && pezzo.getTypePiece() == ROOK) ||
                    (pieceLetter == 'C' && pezzo.getTypePiece() == KNIGHT))
                {
                    string currentSrc = utility.getSquarePuntoDiVista(i, j, puntoDiVista);

                    // Se non è il pezzo di partenza
                    if (currentSrc != src)
                    {
                        // Verifica se questo pezzo può raggiungere la stessa destinazione
                        if (pezzo.getTypePiece() == ROOK)
                        {
                            Rook r = Rook();
                            if (r.checkTraversa(currentSrc, dest, false))
                            {
                                count++;
                            }
                        }
                        else if (pezzo.getTypePiece() == KNIGHT)
                        {
                            Knight c = Knight();
                            if (c.checkJump(currentSrc, dest, false))
                            {
                                count++;
                            }
                        }
                    }
                }
            }
        }
    }

    return (count > 0);
}

void algebricNotation(char who, string src, string dest, string prima_lettera)
{
    // simbolo di cattura nella notazione algebrica
    string simboloCattura = "x";
    char lettera = prima_lettera[0];
    int mossa = numMove + 1;
    if (who == 'W')
    {
        ultimaMossa.clear();
        ultimaMossa.append(to_string(mossa));
        ultimaMossa.append(".");

        if (!arrocco.empty())
        {
            ultimaMossa.append(arrocco);
            arrocco.clear();
        }
        else
        {

            if (!avvenutaCattura)
            {
                ultimaMossa.append(prima_lettera);
                if (!isAmbiguousMove(src, dest, lettera))
                {
                    ultimaMossa.append(dest);
                }
                else
                {
                    ultimaMossa.push_back(src[0]);
                    ultimaMossa.append(dest);
                }
            }
            else if (avvenutaCattura && !prima_lettera.empty())
            {
                // cattura con pezzi
                ultimaMossa.append(prima_lettera);
                if (!isAmbiguousMove(src, dest, lettera))
                {
                    ultimaMossa.append(simboloCattura);
                    ultimaMossa.append(dest);
                }
                else
                {
                    ultimaMossa.push_back(src[0]);
                    ultimaMossa.append(simboloCattura);
                    ultimaMossa.append(dest);
                }
            }
            else
            {
                // cattura con pedone
                ultimaMossa.push_back(src[0]);
                ultimaMossa.append(simboloCattura);
                ultimaMossa.append(dest);
            }
            avvenutaCattura = false;
        }
    }

    if (who == 'W')
    {
        WriteGameToFile(nameFile, ultimaMossa, reg_to_file);
    }

    // Nero
    if (who == 'B')
    {
        ultimaMossa.clear();
        ultimaMossa.append(" ");

        if (!arrocco.empty())
        {
            ultimaMossa.append(arrocco);
            arrocco.clear();
        }
        else
        {

            if (!avvenutaCattura)
            {
                ultimaMossa.append(prima_lettera);

                if (!isAmbiguousMove(src, dest, lettera))
                {
                    ultimaMossa.append(dest);
                }
                else
                {
                    ultimaMossa.push_back(src[0]);
                    ultimaMossa.append(dest);
                }
            }
            else if (avvenutaCattura && !prima_lettera.empty())
            {
                // cattura con pezzi
                ultimaMossa.append(prima_lettera);
                if (!isAmbiguousMove(src, dest, lettera))
                {
                    ultimaMossa.append(simboloCattura);
                    ultimaMossa.append(dest);
                }
                else
                {
                    ultimaMossa.push_back(src[0]);
                    ultimaMossa.append(simboloCattura);
                    ultimaMossa.append(dest);
                }
            }
            else
            {
                // cattura con pedone
                ultimaMossa.push_back(src[0]);
                ultimaMossa.append(simboloCattura);
                ultimaMossa.append(dest);
            }
            avvenutaCattura = false;
        }
        // altra modifica
        ultimaMossa.append(" ");
        printDebug(ultimaMossa);
        // modifica 2
        WriteGameToFile(nameFile, ultimaMossa, reg_to_file);
        ultimaMossa.clear();
    }

    return;
}

/*

 La funzione translateCastle se riceve una mossa di arrocco 0-0 o 0-0-0 la traduce per il programma in una mossa normale, ad esempio 0-0 diventa Rg1 per per il bianco e Rg8 per il nero, 0-0-0 diventa Rc1 per il bianco e Rc8 per il nero

Ritorna invece una stringa vuota se non si tratta di una mossa di arrocco
 */
string translateCastle(string move)
{
    if (move == "0-0")
    {
        if (startColor == 0)
        {
            arrocco = "0-0";
            return "Rg1";
        }
        else
        {
            arrocco = "0-0";
            return "Rg8";
        }
    }
    else if (move == "0-0-0")
    {
        if (startColor == 0)
        {
            arrocco = "0-0-0";
            return "Rc1";
        }
        else
        {
            arrocco = "0-0-0";
            return "Rc8";
        }
    }
    return "";
}
// controlla patta per solo i due re.
void patteElementari()
{

    int numBianchi = 0, numNeri = 0, numFigBianche = 0, numFigNere = 0;
    for (int i = 0; i < 8; i++)
    {

        for (int j = 0; j < 8; j++)
        {

            // conto i pezzi neri e bianchi
            if (chessBoard[i][j].getBusySquare() && chessBoard[i][j].getChessPiece().getTypePiece() != KING && chessBoard[i][j].getChessPiece().getColorPiece() == 'W')
            {
                numBianchi++;
                // se alfiere o cavallo
                if (chessBoard[i][j].getChessPiece().getTypePiece() == BISHOP || chessBoard[i][j].getChessPiece().getTypePiece() == KNIGHT)
                {
                    numFigBianche++;
                }
            }
            if (chessBoard[i][j].getBusySquare() && chessBoard[i][j].getChessPiece().getTypePiece() != KING && chessBoard[i][j].getChessPiece().getColorPiece() == 'B')
            {
                numNeri++;
                if (chessBoard[i][j].getChessPiece().getTypePiece() == BISHOP || chessBoard[i][j].getChessPiece().getTypePiece() == KNIGHT)
                {
                    numFigNere++;
                }
            }
        }
    }
    if (numBianchi == 0 && numNeri == 0)
    {
        callTextToSpeech(string("Re contro re risulta patta"));
        sendMove(roomOnline, playerOnline, "Patta");

        ultimaMossa.clear();
        ultimaMossa.append(" 1/2 - 1/2");
        WriteGameToFile(nameFile, ultimaMossa, reg_to_file);
        Sleep(3);
        reStart();
    }
    if (numBianchi == 1 && numFigBianche == 1 && numNeri <= 1 && numFigNere <= 1)
    {
        // patta teorica
        callTextToSpeech(string("Re con alfiere o cavallo contro re solo o con un alfiere oppure un cavallo risulta patta"));
        sendMove(roomOnline, playerOnline, "Patta");

        ultimaMossa.clear();
        ultimaMossa.append(" 1/2 - 1/2");
        WriteGameToFile(nameFile, ultimaMossa, reg_to_file);
        reStart();
    }

    if (numNeri == 1 && numFigNere == 1 && numBianchi <= 1 && numFigBianche <= 1)
    {
        // patta teorica
        callTextToSpeech(string("Re con alfiere o cavallo contro re solo o con un alfiere oppure un cavallo risulta patta"));
        ultimaMossa.clear();
        ultimaMossa.append(" 1/2 - 1/2");
        WriteGameToFile(nameFile, ultimaMossa, reg_to_file);
        reStart();
    }
    return;
}

bool isPieceDefended(int i, int j, char who)
{

    bool status = false;
    // torna false se tutto ok non sotto attacco
    // i e j sono le coordinate della casella da controllare se sotto attacco (la destination)

    /*
     Questa funzionalità interviene come mattoncino di base nei seguenti casi:
        - se un pezzo risulta difeso, ovvero in una casa controllata da un pezzo amico
        - se una casa è attaccata
        */
    // su tutta la scacchiera cerco Alfieri, Cavalli, Donna, Torri e pedoni di colore uguale a who e che insistono su i e j
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
            if (chessBoard[r][s].getBusySquare() == true && chessBoard[r][s].getChessPiece().getTypePiece() == KING && chessBoard[r][s].getChessPiece().getColorPiece() == who)
            {
                // Se la distanza pari a 1
                if (abs(i - r) <= 1 && abs(j - s) <= 1)
                {
                    // la casa è minacciata
                    printDebug(string("casa " + destination + " minacciata da re in " + source + "\n"));
                    return true;
                }
            }

            if (chessBoard[r][s].getBusySquare() == true && chessBoard[r][s].getChessPiece().getTypePiece() == BISHOP && chessBoard[r][s].getChessPiece().getColorPiece() == who)
            {

                // nel caso dell'alfiere se la casa non è ostruita (true)allora è una minaccia
                if (chessBoard[r][s].getChessPiece().checkDiagonale(source, destination, false) == true)
                {
                    printDebug(string("casa " + destination + " minacciata da alfiere in " + source + "\n"));
                    return true;
                }
            }

            if (chessBoard[r][s].getBusySquare() == true && chessBoard[r][s].getChessPiece().getTypePiece() == QUEEN && chessBoard[r][s].getChessPiece().getColorPiece() == who)
            {
                if (chessBoard[r][s].getChessPiece().checkDiagonale(source, destination, false) == true)
                {
                    printDebug(string("casa " + destination + " minacciata da donna in " + source + "\n"));
                    return true;
                }
            }

            if (chessBoard[r][s].getBusySquare() == true && chessBoard[r][s].getChessPiece().getTypePiece() == QUEEN && chessBoard[r][s].getChessPiece().getColorPiece() == who)
            {
                if (chessBoard[r][s].getChessPiece().checkTraversa(source, destination, false) == true)
                {
                    printDebug(string("casa " + destination + " minacciata da donna in " + source + "\n"));
                    return true;
                }
            }
            // solo se la distanza < 4come riga almeno
            int distVCavallo = abs(source[1] - destination[1]);
            if (chessBoard[r][s].getBusySquare() == true && chessBoard[r][s].getChessPiece().getTypePiece() == KNIGHT && chessBoard[r][s].getChessPiece().getColorPiece() == who)
            {
                if (distVCavallo < 3 && chessBoard[r][s].getChessPiece().checkJump(source, destination, false) == true)
                {
                    printDebug(string("casa " + destination + " minacciata da cavallo in " + source + "\n"));
                    return true;
                }
            }

            if (chessBoard[r][s].getBusySquare() == true && chessBoard[r][s].getChessPiece().getTypePiece() == ROOK && chessBoard[r][s].getChessPiece().getColorPiece() == who)
            {
                if (chessBoard[r][s].getChessPiece().checkTraversa(source, destination, false) == true)
                {
                    printDebug(string("casa " + destination + " minacciata da torre in " + source + "\n"));
                    return true;
                }
            }
            if (chessBoard[r][s].getBusySquare() == true && chessBoard[r][s].getChessPiece().getTypePiece() == PAWN && chessBoard[r][s].getChessPiece().getColorPiece() == who)
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