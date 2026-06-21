// engine4.cpp
/*
Il file definisce ed implementa tutte le funzionalità che permettono la interazione con un chess engine.
Esso si riferisce a comandi standard uci (universal chess interface)m ma testato solo per il motore stockfish.
Ad inizio del file viene incluso il file header engine.h che definisce strutture dati e prototipi

E' stato scelto di usare fstream per i file e string per la gestione delle stringhe

Esso implementa:
- un gestore di uscita (handle_sigint per fare un graceful stop
- start per startare l'engine
- sendCommandper inviare comandi all'engine
- stopper stoppare l'engine
- readConfigEngine per la lettura da file di configurazione engine.cnf del nome del exe di stockfish
- getOutputMove per ottenere la mossa migliore dal motore
getUciok per sincronizzarsi col motore-
isEngineRunning per verificare lo stato del motore
string getPositionFen per ottenere la fen
void evalPosition che da una valutazione della posizione
bool evalWithStop che valuta lo stato di gioco di stockfish (matto, stallo, patta, abbandona)
setSkill(unsigned int skill) per settare lo skill con cui giocare
setParam(int,int)per fare tuning del chess engine
bool isReady() per sapere se l'engine è pronto a ricevere comandi

Per ottenere questo, tutte le funzionalità hanno accesso condiviso alle variabili globali riguardanti:
- lo stato del processo
- le variabili relative alle pipe di comunicazione
- le variabili che riguardano la creazione del processo
- le variabili che riguardano il livello di gioco

Viene implementato una funzione  test_main di utilizzo e test delle funzionalità.

*/
#include <windows.h>
#include <iostream>
#include "globale.h"
#include <fstream>
#include <string>
#include <cstring>
#include <signal.h>
#include "engine.h"
#include <stdlib.h>
#include "proto.h"
#include "chess.h"
#pragma warning(disable : 4267)
#pragma warning(disable : 4805)

using namespace std;

// Global variables for process management
PROCESS_INFORMATION pi = {};
HANDLE hChildStdoutRd, hChildStdoutWr;
HANDLE hChildStdinRd, hChildStdinWr;
bool engineRunning = false;
bool SettingParam = false;

// Implementation
bool readConfigEngine(string &exePath)
{
    ifstream configFile("engine.cnf");
    if (!configFile.is_open())
    {
        cerr << "Cannot open engine.cnf" << endl;
        return false;
    }
    getline(configFile, exePath);

    // cout << "Engine executable path: " << exePath << endl;
    configFile.close();
    return true;
}

bool start()
{
    string exePath;

    if (!readConfigEngine(exePath))
        return false;

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &sa, 0))
        return false;
    if (!CreatePipe(&hChildStdinRd, &hChildStdinWr, &sa, 0))
        return false;

    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdOutput = hChildStdoutWr;
    si.hStdInput = hChildStdinRd;
    si.dwFlags |= STARTF_USESTDHANDLES;

    if (!CreateProcessA(exePath.c_str(), NULL, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
    {
        return false;
    }

    sendCommand("uci\n");
    int status = false;
    string out_ok = getUciok();
    if (out_ok == string("uciok"))
    {
        status = true;
        engineRunning = true;
    }

    if (status == true)
    {
        // setta gestore di contrl-c
        // commentate per consentire Abbandono automatico di stockfish
        // atexit(stop);
        // signal(SIGINT, handle_sigint);
    }
    return status;
}

void sendCommand(const string &command)
{
    DWORD dwWritten;
    WriteFile(hChildStdinWr, command.c_str(), command.length(), &dwWritten, NULL);
}

void stop()
{
    if (engineRunning)
    {
        sendCommand("quit\n");
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        CloseHandle(hChildStdoutRd);
        CloseHandle(hChildStdoutWr);
        CloseHandle(hChildStdinRd);
        CloseHandle(hChildStdinWr);
        engineRunning = false;
    }
}

string getOutputMove()
{
    string line = "";
    string bestMove = "";
    bestMove.clear();
    line.clear();
    DWORD dwRead;
    char buffer[1024];
    memset(buffer, '\0', sizeof(buffer));
    while (ReadFile(hChildStdoutRd, buffer, sizeof(buffer) - 1, &dwRead, NULL) && dwRead > 0)
    {
        buffer[dwRead] = '\0';
        line += buffer;
        // cout << line << endl;

        // se "Unknown command"
        size_t pos_error = line.find("Unknown");
        if (pos_error != string::npos)
        {
            cout << "uknown command\n";
            stop();
            cout << "computer arrestato nell'analisi\n";
            return "1";
            // exit(0);
        }

        // se stockfish abbandona
        pos_error = line.find("resign");
        if (pos_error != string::npos)
        {
            cout << "resign.\n";
            stop();
            cout << "computer arrestato per abbandono\n";
            return "1";

            // exit(0);
        }

        size_t pos = line.find("bestmove");
        if (pos != string::npos)
        {
            size_t start = pos + 9;

            size_t end = line.find(" ", start);
            if (end == string::npos)
                end = line.find("\n", start);
            if (end != string::npos)

                bestMove = line.substr(start, end - start);

            break;
        }
    }

    return bestMove;
}
bool isEngineRunning()
{
    return engineRunning;
}

int test_main()
{

    string response;
    if (start())
    {
        cout << "Engine started successfully" << endl;
        // setDepth(20);
        sendCommand("position startpos\n");
        sendCommand("go depth 10\n");
        response = getOutputMove();
        cout << "best move : " << response << endl;
        stop();
    }
    return 0;
}

void handle_sigint(int sig)
{
    printf("graceful stop proceeded\n");

    exit(0);
    // scatena sia atexit che signal
}

string getPositionFen(char who)
{
    string str;
    // determino prima dove sta il nero in base al punto di vista
    if (puntoDiVista == "Bianco")
    {
        // il nero sta sopra in 0,0
        str = getPositionFenBlackStart0();
    }
    else
    {
        str = getPositionFenBlackStart7();
    }
    string b = getPositionFenLast(who);
    string result = str + b;
    return result;
}

string getPositionFenLast(char who)
{
    string fen;
    // stockfish funziona solo con tolower
    fen.push_back(tolower(who));

    fen.append(" ");

    ChessUtility utility;
    vector<int> idx_e1 = utility.getIndexPuntoDiVista("e1", puntoDiVista);
    vector<int> idx_h1 = utility.getIndexPuntoDiVista("h1", puntoDiVista);
    vector<int> idx_a1 = utility.getIndexPuntoDiVista("a1", puntoDiVista);
    vector<int> idx_e8 = utility.getIndexPuntoDiVista("e8", puntoDiVista);
    vector<int> idx_h8 = utility.getIndexPuntoDiVista("h8", puntoDiVista);
    vector<int> idx_a8 = utility.getIndexPuntoDiVista("a8", puntoDiVista);

    string str_arrocchi = "-";

    if (chessBoard[idx_e1.front()][idx_e1.back()].getBusySquare() &&
        chessBoard[idx_e1.front()][idx_e1.back()].getChessPiece().getTypePiece() == KING &&
        chessBoard[idx_e1.front()][idx_e1.back()].getChessPiece().getColorPiece() == 'W')
    {
        if (chessBoard[idx_h1.front()][idx_h1.back()].getBusySquare() &&
            chessBoard[idx_h1.front()][idx_h1.back()].getChessPiece().getTypePiece() == ROOK &&
            chessBoard[idx_h1.front()][idx_h1.back()].getChessPiece().getColorPiece() == 'W' &&
            chessBoard[idx_h1.front()][idx_h1.back()].getValid00())
        {
            if (str_arrocchi == "-")
                str_arrocchi.clear();
            str_arrocchi.push_back('K');
        }
        if (chessBoard[idx_a1.front()][idx_a1.back()].getBusySquare() &&
            chessBoard[idx_a1.front()][idx_a1.back()].getChessPiece().getTypePiece() == ROOK &&
            chessBoard[idx_a1.front()][idx_a1.back()].getChessPiece().getColorPiece() == 'W' &&
            chessBoard[idx_a1.front()][idx_a1.back()].getValid000())
        {
            if (str_arrocchi == "-")
                str_arrocchi.clear();
            str_arrocchi.push_back('Q');
        }
    }

    if (chessBoard[idx_e8.front()][idx_e8.back()].getBusySquare() &&
        chessBoard[idx_e8.front()][idx_e8.back()].getChessPiece().getTypePiece() == KING &&
        chessBoard[idx_e8.front()][idx_e8.back()].getChessPiece().getColorPiece() == 'B')
    {
        if (chessBoard[idx_h8.front()][idx_h8.back()].getBusySquare() &&
            chessBoard[idx_h8.front()][idx_h8.back()].getChessPiece().getTypePiece() == ROOK &&
            chessBoard[idx_h8.front()][idx_h8.back()].getChessPiece().getColorPiece() == 'B' &&
            chessBoard[idx_h8.front()][idx_h8.back()].getValid00())
        {
            if (str_arrocchi == "-")
                str_arrocchi.clear();
            str_arrocchi.push_back('k');
        }
        if (chessBoard[idx_a8.front()][idx_a8.back()].getBusySquare() &&
            chessBoard[idx_a8.front()][idx_a8.back()].getChessPiece().getTypePiece() == ROOK &&
            chessBoard[idx_a8.front()][idx_a8.back()].getChessPiece().getColorPiece() == 'B' &&
            chessBoard[idx_a8.front()][idx_a8.back()].getValid000())
        {
            if (str_arrocchi == "-")
                str_arrocchi.clear();
            str_arrocchi.push_back('q');
        }
    }

    fen.append(str_arrocchi);
    fen.append(" ");
    fen.push_back('-');
    fen.append(" ");
    fen.push_back('0');
    fen.append(" ");
    fen.append(to_string(numMove));
    return fen;
}

string getPositionFenBlackStart7()
{
    string fen;
    int count_line = 0, count_free = 0;
    char lettera = 'Z';

    for (int i = 7; i >= 0; i--)
    {
        count_line++;
        count_free = 0;
        for (int j = 7; j >= 0; j--)
        {

            if (chessBoard[i][j].getBusySquare() == false)
            {
                count_free++;
                if (count_free > 0 && j == 0)
                {

                    if (count_free == 8 && count_line == 1)
                    {
                        fen = fen + "/" + to_string(count_free) + "/";
                    }
                    else
                    {
                        fen = fen + to_string(count_free) + "/";
                    }
                }
            }
            else
            {
                if (count_free > 0)
                {
                    fen = fen + to_string(count_free);
                    count_free = 0;
                }
                // aggiungo i pezzi che trovo
                if (chessBoard[i][j].getChessPiece().getTypePiece() == ROOK)
                {
                    lettera = 'R';
                    lettera = (chessBoard[i][j].getChessPiece().getColorPiece() == 'W' ? toupper(lettera) : tolower(lettera));
                    fen.push_back(lettera);
                }
                if (chessBoard[i][j].getChessPiece().getTypePiece() == KNIGHT)
                {

                    lettera = 'N';
                    lettera = (chessBoard[i][j].getChessPiece().getColorPiece() == 'W' ? toupper(lettera) : tolower(lettera));
                    fen.push_back(lettera);
                }
                if (chessBoard[i][j].getChessPiece().getTypePiece() == BISHOP)
                {
                    lettera = 'B';
                    lettera = (chessBoard[i][j].getChessPiece().getColorPiece() == 'W' ? toupper(lettera) : tolower(lettera));
                    fen.push_back(lettera);
                }
                if (chessBoard[i][j].getChessPiece().getTypePiece() == KING)
                {

                    lettera = 'K';
                    lettera = (chessBoard[i][j].getChessPiece().getColorPiece() == 'W' ? toupper(lettera) : tolower(lettera));
                    fen.push_back(lettera);
                }
                if (chessBoard[i][j].getChessPiece().getTypePiece() == QUEEN)
                {
                    lettera = 'Q';
                    lettera = (chessBoard[i][j].getChessPiece().getColorPiece() == 'W' ? toupper(lettera) : tolower(lettera));
                    fen.push_back(lettera);
                }
                if (chessBoard[i][j].getChessPiece().getTypePiece() == PAWN)
                {
                    lettera = 'P';
                    lettera = (chessBoard[i][j].getChessPiece().getColorPiece() == 'W' ? toupper(lettera) : tolower(lettera));
                    fen.push_back(lettera);
                }
                if (j == 0)
                {
                    fen = fen + "/";
                }
            }
        }
    }

    fen.erase(fen.length() - 1); // tolgo lo slash

    fen.append(" ");

    return fen;
}
// seconda parte
string getPositionFenBlackStart0()
{

    string fen;
    int count_line = 0, count_free = 0;
    char lettera = 'Z';

    for (int i = 0; i <= 7; i++)
    {
        count_line++;
        count_free = 0;
        for (int j = 0; j <= 7; j++)
        {

            if (chessBoard[i][j].getBusySquare() == false)
            {
                count_free++;
                if (count_free > 0 && j == 7)
                {

                    if (count_free == 8 && count_line == 1)
                    {
                        fen = fen + "/" + to_string(count_free) + "/";
                    }
                    else
                    {
                        fen = fen + to_string(count_free) + "/";
                    }
                }
            }
            else
            {
                if (count_free > 0)
                {
                    fen = fen + to_string(count_free);
                    count_free = 0;
                }
                // aggiungo i pezzi che trovo
                if (chessBoard[i][j].getChessPiece().getTypePiece() == ROOK)
                {
                    lettera = 'R';
                    lettera = (chessBoard[i][j].getChessPiece().getColorPiece() == 'W' ? toupper(lettera) : tolower(lettera));
                    fen.push_back(lettera);
                }
                if (chessBoard[i][j].getChessPiece().getTypePiece() == KNIGHT)
                {

                    lettera = 'N';
                    lettera = (chessBoard[i][j].getChessPiece().getColorPiece() == 'W' ? toupper(lettera) : tolower(lettera));
                    fen.push_back(lettera);
                }
                if (chessBoard[i][j].getChessPiece().getTypePiece() == BISHOP)
                {
                    lettera = 'B';
                    lettera = (chessBoard[i][j].getChessPiece().getColorPiece() == 'W' ? toupper(lettera) : tolower(lettera));
                    fen.push_back(lettera);
                }
                if (chessBoard[i][j].getChessPiece().getTypePiece() == KING)
                {

                    lettera = 'K';
                    lettera = (chessBoard[i][j].getChessPiece().getColorPiece() == 'W' ? toupper(lettera) : tolower(lettera));
                    fen.push_back(lettera);
                }
                if (chessBoard[i][j].getChessPiece().getTypePiece() == QUEEN)
                {
                    lettera = 'Q';
                    lettera = (chessBoard[i][j].getChessPiece().getColorPiece() == 'W' ? toupper(lettera) : tolower(lettera));
                    fen.push_back(lettera);
                }
                if (chessBoard[i][j].getChessPiece().getTypePiece() == PAWN)
                {
                    lettera = 'P';
                    lettera = (chessBoard[i][j].getChessPiece().getColorPiece() == 'W' ? toupper(lettera) : tolower(lettera));
                    fen.push_back(lettera);
                }
                if (j == 7)
                {
                    fen = fen + "/";
                }
            }
        }
    }

    fen.erase(fen.length() - 1); // tolgo lo slash
    fen.append(" ");
    return fen;
}

string getUciok()
{
    string line;
    string uciok = "Boh";
    DWORD dwRead;
    char buffer[1024];

    while (ReadFile(hChildStdoutRd, buffer, sizeof(buffer) - 1, &dwRead, NULL) && dwRead > 0)
    {
        buffer[dwRead] = '\0';
        line += buffer;
        // cout << line << endl;

        // se "Unknown command"
        size_t pos_error = line.find("Unknown");
        if (pos_error != string::npos)
        {
            cout << "uknown command\n";
            stop();
            cout << "computer arrestato nell'analisi\n";
            exit(1);
        }

        size_t pos = line.find("uciok");
        if (pos != string::npos)
        {
            // trovato
            uciok = "uciok";
            break;
        }
        else
        {
            uciok = "Boh";
        }
    }
    return uciok;
}
void evalPosition()
{

    // comando non standard uci ma che vari chess engin usano
    string line = "", evaluation = "";
    line.clear();
    evaluation.clear();
    char buffer[1024];
    memset(buffer, '\0', sizeof(buffer));

    sendCommand("eval\n");
    //  leggo la valutazione
    DWORD dwRead;
    while (ReadFile(hChildStdoutRd, buffer, sizeof(buffer) - 1, &dwRead, NULL) && dwRead > 0)
    {
        buffer[dwRead] = '\0';
        line += buffer;

        size_t pos = line.find("Final evaluation");
        if (pos != string::npos)
        {

            size_t end = line.find(")");
            if (end != string::npos)
            {
                evaluation = line.substr(pos + 23, end - pos - 23 + 1);
                // Sleep(2000);
                // cout << evaluation << endl;
                Sleep(3000);
                callTextToSpeech(evaluation);
                break;
            }
        }
    }
    return;
}
void setSkill(int skill)
{

    unsigned int value = 0;
    if (skill > 20)
    {
        value = 20;
    }
    if (skill < 0)
    {
        value = 0;
    }
    string s_skill = "setoption name Skill Level value " + to_string(value) + "\n";
    sendCommand(s_skill);
    return;
}

void setParam(int maxThreads, int maxHashTable, int multiPV)
{
    // solo se maxThreads è valorizzato setto i parametri
    string cmd_maxThreads = "setoption name Threads value " + to_string(maxThreads) + "\n";
    string cmd_maxHashTable = "setoption name Hash value " + to_string(maxHashTable) + "\n";

    // MultiPV se valorizzata viene attivata
    string cmd_multiPV = "setoption name MultiPV value " + to_string(multiPV) + "\n";
    if (maxThreads > 0 && maxHashTable > 0)
    {
        sendCommand(cmd_maxHashTable);

        sendCommand(cmd_maxThreads);
        SettingParam = true;
    }

    if (multiPV > 0)
    {
        sendCommand(cmd_multiPV);
        SettingParam = true;
    }
    return;
}

void getAsciiArt_withFen()
{
    char buffer[1024];
    memset(buffer, '\0', sizeof(buffer));

    // comando non standard uci ma che vari chess engin usano
    sendCommand("d\n");
    // leggo l'Ascii Art con la posizione fen
    DWORD dwRead;
    while (ReadFile(hChildStdoutRd, buffer, sizeof(buffer) - 1, &dwRead, NULL) && dwRead > 0)
    {
        buffer[dwRead] = '\0';
        cout << buffer << endl;
    }
    return;
}

bool isSettingParam()
{
    return SettingParam;
}

bool isReady()
{
    bool isready = false;
    string line = "", evaluation = "";
    line.clear();
    evaluation = "isReady ok";
    char buffer[1024];
    memset(buffer, '\0', sizeof(buffer));

    sendCommand("isready\n");
    //  leggo la valutazione
    DWORD dwRead;
    while (ReadFile(hChildStdoutRd, buffer, sizeof(buffer) - 1, &dwRead, NULL) && dwRead > 0)
    {
        buffer[dwRead] = '\0';
        line += buffer;

        size_t pos = line.find("readyok");
        if (pos != string::npos)
        {

            isready = true;
            callTextToSpeech(evaluation);
            break;
        }
    }
    return isready;
}

string evalWithStop(string stock_color)
{

    /*
     Questa funzione
     - ha un valore soglia in centipedone (es. 500 ovvero 5 pedoni) e riceve in input il colore con cui gioca stockfish (es. Bianco o Nero),
     - manda il comando uci eval
     - legge la valutazione
     - considera se la valutazione è riferita a stockfish:
     - se la valutazione  dice che stockfish ha ricevuto matto ne da messaggio vocale , fa stop e ritorna true
     - se la valutazione  riporta che stockfish è in stallo si manda il messaggio vocale, si fa stop e si ritorna true
     -se la valutazione dichiara patta se ne da messaggio vocale, si fa stop e si ritorna true
     - se la valutazione riferita a stockfish è oltre la soglia (perde più di della soglia) , da messaggio vocale che stockfish abbandona
     */
    /*
    NON USATA
    */
    double threshold = 5.0;
    string line = "", evaluation = "";
    line.clear();
    evaluation.clear();
    char buffer[1024];
    memset(buffer, '\0', sizeof(buffer));

    sendCommand("eval\n");
    DWORD dwRead;
    while (ReadFile(hChildStdoutRd, buffer, sizeof(buffer) - 1, &dwRead, NULL) && dwRead > 0)
    {
        buffer[dwRead] = '\0';
        line += buffer;

        size_t pos = line.find("Final evaluation");
        if (pos != string::npos)
        {
            size_t end = line.find(")");
            if (end != string::npos)
            {
                evaluation = line.substr(pos + 23, end - pos - 23 + 1);
                //   Controlla matto
                if (evaluation.find("#") != string::npos)
                {
                    callTextToSpeech("Matto");
                    return "1";
                }
                // Controlla stallo
                if (evaluation.find("Stalemate") != string::npos)
                {
                    callTextToSpeech("Stallo");
                    return "1";
                }
                // Controlla patta
                if (evaluation.find("0.00") != string::npos)
                {
                    callTextToSpeech("Patta");
                    return "1";
                }
                // Estrae valore numerico e confronta con threshold
                try
                {
                    double eval_value = stof(evaluation);
                    callTextToSpeech(evaluation);
                    if (stock_color == "Nero" && eval_value > 0)
                        if (eval_value > threshold)
                        {
                            callTextToSpeech("Abbandono");
                            return "1";
                        }
                    if (stock_color == "Bianco" && eval_value < 0)
                        threshold = -threshold;
                    if (eval_value < threshold)
                    {
                        callTextToSpeech("Abbandono");
                        return "1";
                    }
                }
                catch (...)
                {
                    return "0";
                }
                break;
            }
        }
    }
    return "0";
}