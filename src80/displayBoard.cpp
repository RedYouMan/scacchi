// mostra vocalmente posizione dei pezzi
#include <iostream>
#include "chess.h"
#include <csignal>
#include <atomic>
#include "proto.h"
#include <cctype>
#include <string>
#include "globale.h"
using namespace std;

void displayBoard()
{
    // signalper interrompere vocale
    signal(SIGINT, signalHandler);
    string msg;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (chessBoard[i][j].getBusySquare())
            {
                msg.clear();
                msg = string(chessBoard[i][j].getChessPiece().getNamePiece()) + string(" nella casella ") + string(chessBoard[i][j].getSquareName()) + string("\n");
                if (!interrompiVocale)
                {
                    callTextToSpeech(msg);
                }
            }
        }
    }

    interrompiVocale = false;
    return;
}
