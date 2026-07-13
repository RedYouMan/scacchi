// utility varie
#include <SFML/Audio.hpp>
#include <windows.h>
#include <iostream>
#include "globale.h"
#include "proto.h"
#include <string>
#include <thread> // Per std::this_thread::sleep_for
#include <chrono> // Per std::chrono::milliseconds
#include <csignal>
#include <atomic>
#pragma warning(disable : 4267)

using namespace std;

// riproduce il toc del pezzo sulla scacchiera
void gestione_toc()
{

    sf::SoundBuffer buffer;
    // Controlla di avere un file "toc.wav" nella cartella suoni del progetto

    string file_suono = "../suoni/toc.wav";
    // non rileviamo se non carica
    buffer.loadFromFile(file_suono.c_str());

    sf::Sound sound;
    sound.setBuffer(buffer);
    // Opzionale: Regola pitch o volume per renderlo più simile a un "toc" se necessario
    sound.setVolume(50.f);
    sound.play();
    // Senza questo ciclo, gestione_toc() finirebbe subito e il suono si interromperebbe.
    while (sound.getStatus() == sf::Sound::Playing)
    {
        // Dormi un po' per non usare il 100% della CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return;
}
void printDebug(string msg)
{
    if (test)
    {
        cout << msg << endl;
    }
    return;
}

void help()
{
    // commentata a causa Abbandono automatico di stockfish

    // signal(SIGINT, signalHandler);

    callTextToSpeech(string("Scacchi-it  v.8.6\n"));
    if (!interrompiVocale)
    {
        callTextToSpeech(string("CtrlX per approfondimenti sul sito\n"));
    }

    if (!interrompiVocale)
    {
        callTextToSpeech(string("CtrlN perinizializzare la scacchiera\n"));
    }

    if (!interrompiVocale)
    {
        callTextToSpeech(string("CtrlG per cambiare punto di vista di gioco\n"));
    }
    if (!interrompiVocale)
    {
        callTextToSpeech(string("frecce per navigare la scacchiera. Possibile anche il mouse. Attendere qualche secondo per avere le informazioni\n"));
    }

    if (!interrompiVocale)
    {
        callTextToSpeech(string("RETURN per selezionare il pezzo e ulteriore RETURN per selezionare la destinazione\n"));
    }

    if (!interrompiVocale)
    {
        callTextToSpeech(string("CtrlI per giocare online\n"));
    }

    if (!interrompiVocale)
    {
        callTextToSpeech(string("CtrlA per forzare un'apertura desiderata\n"));
    }
    if (!interrompiVocale)
    {
        callTextToSpeech(string("CtrlT per aprire l'Openings Trainer\n"));
    }

    if (!interrompiVocale)
    {
        callTextToSpeech(string("CtrlL per fare Abbandono col Re\n"));
    }
    if (!interrompiVocale)
    {
        callTextToSpeech(string("CtrlW per registrare la partita\n"));
    }

    if (!interrompiVocale)
    {
        callTextToSpeech(string("CtrlU per fare l'undo delle mosse\n"));
    }

    if (!interrompiVocale)
    {
        callTextToSpeech(string("CtrlP per caricare un problema da risolvere\n"));
    }
    if (!interrompiVocale)
    {
        callTextToSpeech(string("CtrlJ per far risolvere un problema al computer\n"));
    }
    if (!interrompiVocale)
    {
        callTextToSpeech(string("CtrlS per sospendere la partita da riprendere poi\n"));
    }
    if (!interrompiVocale)
    {
        callTextToSpeech(string("CtrlR per riprendere una partita\n"));
    }
    if (!interrompiVocale)
    {
        callTextToSpeech(string("CtrlQ per uscire dal gioco\n"));
    }

    interrompiVocale = false;
    return;
}

// funzione che deve rimandare a stockfish la mossa nel suo modo di interpretare le mosse
/* questa funzione si utilizza col comando uci position startpos soltanto*/
string unionMove()
{

    string result, app;

    if (sourceOnBoard.length() == 2 && destOnBoard.length() == 2)
    {
        result = sourceOnBoard + " " + destOnBoard;
    }
    if (sourceOnBoard.length() > 2)
    {
        app = sourceOnBoard.substr(1, 2);
        result = app;
        app = destOnBoard.substr(1, 2);
        result.append(app);
    }

    return result;
}
// funzione che riceve tutte le mosse e deve prendere le ultime 4
// e interpretare la mossa
// come pezzo e separandole in due posizioni
void interpreter(string response)
{
    string app;
    // printf("debug: ricevuto %s\n", response.c_str());
    printDebug(string("lunghezza response " + to_string(response.length()) + "\n"));
    // printf("debug: response da interpretare %s\n", response.c_str());
    // printf("debug: lunghezza response  %d\n", response.length());
    int start = response.length() - 4;
    if (start > 0)
    {
        start--;
    }
    app = response.substr(start, 4);
    char lettera = 'Z';
    string casa1, casa2;
    casa1 = app.substr(0, 2);
    casa2 = app.substr(2, 2);
    printDebug(string("casa1 = " + casa1 + "\n"));
    printDebug(string("casa2 = " + casa2 + "\n"));
    // printf("debug: casa1 %s\n", casa1.c_str());
    // printf("debug: casa2 %s\n", casa2.c_str());
    sourceOnBoard.clear();
    destOnBoard.clear();

    ChessUtility utility;
    vector<int> Ind_src = utility.getIndexPuntoDiVista(casa1, puntoDiVista);

    lettera = chessBoard[Ind_src.front()][Ind_src.back()].getChessPiece().getNamePiece()[0];
    sourceOnBoard.push_back(lettera);
    sourceOnBoard.append(casa1);
    destOnBoard.push_back(lettera);
    destOnBoard.append(casa2);

    printDebug(string("pezzo casa 1 = " + sourceOnBoard + "\n"));
    // printf("debug: pezzo casa 1 %s\n", sourceOnBoard.c_str());

    return;
}

// funzione che permette a risolvi il problema determinando le coordinate per usare animationBoard
void settingCoord()
{
    ChessUtility utility;
    string casa1, casa2;
    casa1 = sourceOnBoard.substr(1, 2);
    casa2 = destOnBoard.substr(1, 2);
    vector<int> Ind_src = utility.getIndexPuntoDiVista(casa1, puntoDiVista);
    vector<int> Ind_dest = utility.getIndexPuntoDiVista(casa2, puntoDiVista);
    startSolRow = Ind_src.front();
    startSolCol = Ind_src.back();
    endSolRow = Ind_dest.front();
    endSolCol = Ind_dest.back();
    return;
}
// Gestore del segnale SIGINT (Ctrl+C)
void signalHandler(int signum)
{
    interrompiVocale = true; // Imposta il flag a true
}
