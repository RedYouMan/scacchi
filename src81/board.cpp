
// board.cpp, questo sorgente, è il main del gioco
// nav.cpp permette, invece, la navigazione su scacchiera
#include <windows.h>
#include <iostream>
#include <clocale>
#include <algorithm>
#include <cctype>
#include "chess.h"
#include <string>
#include "engine.h"
#include "proto.h"
#include <csignal>
#include <atomic>
#include "pgnreader.h"

#include <future>
#include <future>
#include <chrono>
#include <thread>

using namespace std;
std::atomic<bool> interrompiVocale(false);

/*
AUTORE: Rosario Turco
DATA: Dicembre 2025
COPYRIGHT: vedi MIT license
 */
/* variabili globali */
;
// variabili globali per giocare in rete
bool online = false;
string colorOnline = "Bianco";
string roomOnline = "";
string playerOnline = "";
// variabile globale usata da checkMoveKing e Intercepte start_game
string PieceNoTouch[100];
int IndiceNoTouch = 0;

// per saltare il messaggio di cattura pezzo nel caso di pezzo fermacarte
bool jumpFalsePiece = false;
// per printDebug mettendo a false non stampa
bool test = false;
// per consentire nei test di sospendere un problema mettere a true
bool sospendi_test = false;
// per farsi produrre un file in directory logdir contenente il fen
bool test_fen = false;
// le mosse inviate a stockfish o ricevute (invio di fen o una mossa ricevuta)
string allMovesToComputer;
// la profondità di analisi di stockfish
int engineDepth = 20;
int level = 20; // livello di profondità
int skill = 10;
int maxThreads = 0;
int maxHashTable = 0;
int multiPV = 0;
// il colore con cui deve giocare stockfish
string stockfish_color;
// chi ha appena giocato: usato da forzaApertura
char whoPlayed = '@';
// chi gioca: usato da undoMove e navOnBoard
char who_play = '@';

// Il numero di mossa
int numMove = 0;
// definipzione della scacchiera
ChessSquare chessBoard[8][8];
// Gestione en passant
bool EnP = false; // segnala a chess.cpp che va risolto a false enp
vector<string> pawnEnP;
// condiviso solo con undo.cpp
vector<std::string> undoPawnEnP;

string casellaDaCatturare;
char coloreAventeDiritto;
// gestione ultima mossa
string ultimaMossa;
// gestione registrazione
bool reg_to_file = false;
string nameFile;
// gestione per caricamento problemi da risolvere indipendentemente da chi muove per prima
bool isProblem = false;
string nameProblem;
// per scrittura su file di arrocco
string arrocco;
// Per stabilire il colore che deve giocare sia per i problemi che nella partita
int startColor = 0;
// dati di navigazione
string sourceOnBoard;
string destOnBoard;
int gRow = 0, gCol = 0; // per la navigazione su scacchiera
bool avvenutaCattura = false;
// punto di vista da cui giocare
string puntoDiVista;
// utile per dire se si è tolti da scacco
bool KingMove = false;
// gestione scacco al re per vedere se lo scacco è stato parato
bool isScaccoRe = false;
// per animazione arrocco
bool is_arrocco = false;
int startRookCol = 0, startRookRow = 0, endRookCol = 0, endRookRow = 0;
string tipoArrocco;
// per problema
int startSolRow = 0, startSolCol = 0, endSolRow = 0, endSolCol = 0;

// per la fen campo enpassant
string enpFEN;
// Autore : Rosario Turco
// data: Dicembre 2025

// Una breve introduzione che spiega come è realizzato il programmaè in chess.cpp
// una introduzione all'uso del programma è in scacchi.txt
// il programma è soggetto alla MITlicense allegata
int main()
{

    start_game();

    cout << "Grazie per aver giocato a scacchi con il nostro programma\n";
    return 0;
}

void start_game()
{

    // per default metto puntoDiVista a bianco
    // CtrlG per cambiarlo
    puntoDiVista = "Bianco";
    gRow = 6;
    gCol = 4;

    // grow e gcol su casa e2

    clear_globali();
    cleanBoard();
    readChessParam();
    clearNoTouch();
    deleteUndo();
    callTextToSpeech(string("Benvenuti a Scacchi-it! Il gioco degli scacchi in italiano con interfaccia vocale. \n"));
    cout << "Scacchi-it (C) 2025 versione 9.1 - Rosario Turco\n";
    cout << "sulla scacchiera: CtrlH per help(), CtrlX per tutorial\n";
    init();
    Sleep(10000);
    startGrafica();
    drawBoard();
    gioca();
    return;
}
void gioca()
{
    int status = 0, domanda = 0;

    while (status == 0)
    {

        if (startColor == 0)
        {
            playWhite();
            whoPlayed = 'W';
            startColor++;
        }
        if (startColor == 1)
        {
            playBlack();
            whoPlayed = 'B';
            startColor++;
        }

        // startColor deve passare da 0 a 1 e viceversa
        if (startColor > 1)
        {
            startColor = 0;
        }
    }

    return;
}
// void init() in gesfile   inizializza la scacchiera
void insertNameFile()
{
    int domanda = 0;

    string path = "..//registrazioni//";
    string appo;
    reg_to_file = true;
    Sleep(2000);
    size_t len = 50;
    while (len == 50)
    {

        callTextToSpeech(string("Inserisci il nome del file senza estensione txt, seguito da return"));
        nameFile.clear();
        cin >> appo;
        cin.ignore();
        len = appo.length();
        if (len > 35)
        {
            len = 50;
            callTextToSpeech(string("Warning: massima lunghezza ammessa 35 caratteri"));
        }

        // controllo se messo estensione

        size_t ext = appo.find('.');
        if (ext != std::string::npos)
        {
            callTextToSpeech(string("Non devi mettere la estensione .txt, ripeti il nome"));
            len = 50;
        }
    }
    nameFile.append(path);
    nameFile.append(appo);
    nameFile.append(".txt");
    return;
}

void playWhite()
{

    if (numMove < 0)
    {
        numMove = 0; // per Undo
    }
    int numero = numMove + 1;
    string msg = "mossa numero " + to_string(numero) + "\n";
    callTextToSpeech(msg);
    callTextToSpeech(string("Turno del Bianco\n"));
    int status = 0;

    cheStallo('W'); // controllo se può muovere
    patteElementari();
    if (stockfish_color != "Bianco" || stockfish_color.empty())
    {
        // per comodità porto il giocatore in e2
        if (puntoDiVista == "Bianco")
        {
            gRow = 6;
            gCol = 4;
        }
        else
        {
            gRow = 1;
            gCol = 3;
        }
        callTextToSpeech(string("Sei in casa ")); // il messaggio viene completato daonSquare
        onSquare(gRow, gCol);

        string giocatore = "";

        if (stockfish_color == "Bianco")
        {
            giocatore = "Nero";
        }
        else
        {
            giocatore = "Bianco";
        }

        while (status == 0)
        {
            if (stockfish_color.empty() || giocatore == "Bianco")
            {
                // se in rete Aspetto le mosse del nero
                checkReceiveRete('B');
                navOnBoard('W');
                status = insertMoveNav('W');
                // se in rete e bianco devo inviare la mossa
                if (online && colorOnline == "Bianco" && status != 0)
                {

                    invioMossaRete();
                    // incremento qui perchè non continuo la funzione
                    numMove++;
                    startColor = 1;
                    return;
                }
            }
        }

        // mi devo chiedere se ora il nero è stockfish per inviargli i dati
        // se si devo inviargli qui la mossa
        if (stockfish_color == "Nero")
        {

            // comando per inviargli la mossa
            string result = getPositionFen('B');
            allMovesToComputer.clear();
            allMovesToComputer.append(result);
            string cmd = "position fen " + result + " moves\n";
            printDebug(string("invio " + cmd + "\n"));
            sendCommand(cmd);
            sendCommand(string("go depth " + to_string(level) + "\n"));
        }
    }
    else
    {

        // gioca stockfish come Bianco
        if (numMove <= 1)

        {

            // gli do comando di iniziare
            sendCommand("ucinewgame\n");
            string msg_str = "go depth " + to_string(level) + "\n";
            sendCommand(msg_str);
        }

        /*
         devo però verificare lo stato di stockfish che potrebbe trovarsi in
         matto, stallo, patta o abbandona. Per farlo setto
         un timer e gestisco la getOutputMove  in asincrono
        */
        string app;
        app.clear();
        auto timeout_durata = std::chrono::minutes(2);
        std::future<std::string> futuroRisultato = std::async(std::launch::async, getOutputMove);
        // aspetta il risultato
        if (futuroRisultato.wait_for(timeout_durata) == std::future_status::timeout)
        {
            // timeout scaduto
            callTextToSpeech("Stockfish abbandona");
            sendCommand("quit\n");
            Sleep(2000);
            exit(0);
        }
        else
        {

            // risposto a tempo
            app = futuroRisultato.get();
            if (app == "1")
            {
                sendCommand("quit\n");
                Sleep(2000);
                exit(0);
            }
        }

        allMovesToComputer.clear();

        allMovesToComputer.append(app);
        interpreter(app);
        status = insertMoveNav('W');
    }
    numMove++;
    return;
}

void playBlack()
{
    string msg = "mossa numero " + to_string(numMove) + "\n";
    callTextToSpeech(msg);
    callTextToSpeech(string("Turno del Nero\n"));
    int status = 0;
    // controllo se può muovere

    cheStallo('B');
    patteElementari();
    if (stockfish_color != "Nero" || stockfish_color.empty())
    {
        // Per comodità porto il giocatore in e7
        if (puntoDiVista == "Bianco")
        {
            gRow = 1;
            gCol = 4;
        }
        else
        {
            gRow = 6;
            gCol = 3;
        }
        callTextToSpeech(string("Sei in casa ")); // il messaggio viene completato da onSquare
        onSquare(gRow, gCol);

        string giocatore = "";
        if (stockfish_color == "Nero")
        {
            giocatore = "Bianco";
        }
        else
        {
            giocatore = "Nero";
        }
        while (status == 0)
        {

            if (stockfish_color.empty() || giocatore == "Nero")
            {

                // se in rete Aspetto le mosse del bianco
                checkReceiveRete('W');
                navOnBoard('B');
                status = insertMoveNav('B');
                // se in rete e nero devo inviare la mossa
                if (online && colorOnline == "Nero" && status != 0)
                {
                    invioMossaRete();
                    startColor = 0;
                    return;
                }
            }
        }

        // mi devo chiedere se ora il Bianco è stockfish
        // se si devo inviargli qui la mossa

        if (stockfish_color == "Bianco")
        {

            //  comando per inviargli la mossa
            string result = getPositionFen('W');
            allMovesToComputer.clear();
            allMovesToComputer.append(result);
            string cmd = "position fen " + result + " moves\n";
            sendCommand(cmd);
            sendCommand(string("go depth " + to_string(level) + "\n"));

            allMovesToComputer.clear();
        }
    }
    else
    {

        callTextToSpeech(string("Gioca il computer. Attendere ...\n"));
        // gioca stockfish come nero
        // avendo ricevuto mossa dal bianco, ora con get risponde

        /*
         devo però verificare lo stato di stockfish che potrebbe trovarsi in
         matto, stallo, patta o abbandona. Per farlo setto
         un timer e gestisco la getOutputMove  in asincrono
        */
        string app;
        app.clear();
        auto timeout_durata = std::chrono::minutes(2);
        std::future<std::string> futuroRisultato = std::async(std::launch::async, getOutputMove);
        // aspetta il risultato
        if (futuroRisultato.wait_for(timeout_durata) == std::future_status::timeout)
        {
            // timeout scaduto
            callTextToSpeech("Stockfish abbandona");
            sendCommand("quit\n");
            Sleep(2000);
            exit(0);
        }
        else
        {

            // risposto a tempo
            app = futuroRisultato.get();
            if (app == "1")
            {
                sendCommand("quit\n");
                Sleep(2000);
                exit(0);
            }
        }

        allMovesToComputer.clear();
        allMovesToComputer.append(app);

        interpreter(app);
        status = insertMoveNav('B');
    }
    return;
}
int move(string src, string dest, char who)
{
    int i = 0, j = 0, m = 0, n = 0;
    static string casaEnpReale = ""; // casaEnpReale dove attaccante va realmente con enpassant
    static string srcEnp;
    string primaLettera;
    // indici sulla scacchiera
    /*
    devo tener conto che 0,0 sta sopra a sinistra nella matrice
    e devo tener conto del punto di vista da cui gioco
    */
    ChessUtility utility;
    vector<int> Isrc, Idest;
    Isrc = utility.getIndexPuntoDiVista(src, puntoDiVista);
    Idest = utility.getIndexPuntoDiVista(dest, puntoDiVista);
    i = Isrc.front();
    j = Isrc.back();
    m = Idest.front();
    n = Idest.back();

    string msg = "debug: indice riga partenza " + to_string(i) + " " + to_string(j) + "\n";
    printDebug(msg);

    msg.clear();
    msg = "debug: indice riga arrivo " + to_string(m) + " " + to_string(n) + "\n";
    printDebug(msg);

    if (i < 0 || i > 7)
    {
        printf("Coordinata riga di partenza non valida!\n");
        return 0;
    }
    if (j < 0 || j > 7)
    {
        printf("Coordinata colonna di partenza non valida!\n");
        return 0;
    }
    if (m < 0 || m > 7)
    {
        printf("Coordinata riga di arrivo non valida!\n");
        return 0;
    }
    if (n < 0 || n > 7)
    {
        printf("Coordinata colonna di arrivo non valida!\n");
        return 0;
    }

    if (!chessBoard[i][j].getBusySquare())
    {

        if (isEngineRunning() && !stockfish_color.empty())
        {
            evalPosition();
            string msg = "Il " + stockfish_color + " abbandona\n";
            callTextToSpeech(msg);
            sendCommand("quit\n");
            reStart();
        }
        return 0;
    }

    if (chessBoard[m][n].getBusySquare() && chessBoard[m][n].getChessPiece().getColorPiece() == who)
    {
        printf("nella casella di arrivo risulta presente un pezzo amico!\n");
        return 0;
    }

    primaLettera.clear();
    bool check = false;
    // individuo il tipo di pezzo
    if (chessBoard[i][j].getChessPiece().getTypePiece() == PAWN)
    {
        Pawn pedone = Pawn();
        char menouno = src[0] - 1;
        char piuuno = src[0] + 1;

        if (abs(i - m) == 2 && (menouno >= 'a' || piuuno <= 'h') && (chessBoard[m][n + 1].getChessPiece().getTypePiece() == PAWN || chessBoard[m][n - 1].getChessPiece().getTypePiece() == PAWN))
        {
            pawnEnP.clear();
            msg.clear(); // verifico se affianco ci sono pedoni avversari per settare EnP a true e string pawnEnPal al valore della posizione del pedone che ha tale opportunità
            printDebug(string("verifica per enpassant\n"));
            // gestione enpassant per il pedone che si muove di due passi e si trova affianco ad un pedone avversario, se c'è questa situazione allora setta EnP a true e la string pawnEnP al nome della casella del pedone che ha diritto di catturare in enpassant

            int app = 0;
            if (piuuno <= 'h')
            {
                if (puntoDiVista == "Nero")
                {
                    app = 7 - piuuno + 97;
                }
                else
                {
                    app = piuuno - 97;
                }
            }
            if (piuuno <= 'h' && chessBoard[m][app].getChessPiece().getTypePiece() == PAWN && chessBoard[m][app].getBusySquare() == true && chessBoard[m][app].getChessPiece().getColorPiece() != who)
            {

                printDebug(string("entrato in piuuno\n"));
                EnP = true;
                pawnEnP.push_back(chessBoard[m][app].getSquareName());
                undoPawnEnP.push_back(chessBoard[m][app].getSquareName());

                // chi colore ne ha dirittoda chessBoard[m][n+1]
                coloreAventeDiritto = chessBoard[m][n + 1].getChessPiece().getColorPiece();
                string appo = (coloreAventeDiritto == 'W' ? "bianco" : "nero");

                msg = "Possibile en passant per pedone " + pawnEnP.back() + "di colore " + appo;
                printf("%s\n", msg.c_str());
                casellaDaCatturare = dest;
                srcEnp = casellaDaCatturare;

                casaEnpReale = "##";
                casaEnpReale[0] = casellaDaCatturare[0];
                if (puntoDiVista == "Bianco")
                {
                    casaEnpReale[1] = casellaDaCatturare[1] - 1;
                }
                else
                {
                    casaEnpReale[1] = casellaDaCatturare[1] + 1;
                }
                msg.clear();
                enpFEN.clear();
                enpFEN = casaEnpReale;
                msg = "pedone da catturare in " + casellaDaCatturare + " \n";
                printf("%s", msg.c_str());
                msg.clear();
            }
            app = 0;
            if (menouno >= 'a')
            {
                if (puntoDiVista == "Nero")
                {
                    app = 7 - menouno + 97;
                }
                else
                {
                    app = menouno - 97;
                }
            }

            ;
            if (menouno >= 'a' && (chessBoard[m][app].getChessPiece().getTypePiece() == PAWN && chessBoard[m][app].getBusySquare() == true && chessBoard[m][app].getChessPiece().getColorPiece() != who))
            {
                printDebug(string("entrato in menouno\n"));
                EnP = true;
                pawnEnP.push_back(chessBoard[m][app].getSquareName());
                undoPawnEnP.push_back(chessBoard[m][app].getSquareName());

                // chi colore ne ha dirittoda chessBoard[m][n-1]
                coloreAventeDiritto = chessBoard[m][n - 1].getChessPiece().getColorPiece();
                string appo = (coloreAventeDiritto == 'W' ? "bianco" : "nero");

                msg = "Possibile en passant per pedone " + pawnEnP.back() + "di colore " + appo;
                printf("%s\n", msg.c_str());
                msg.clear();
                casellaDaCatturare = dest;
                srcEnp = casellaDaCatturare;
                casaEnpReale = "##";
                casaEnpReale[0] = casellaDaCatturare[0];
                if (puntoDiVista == "Bianco")
                {
                    casaEnpReale[1] = casellaDaCatturare[1] - 1;
                }
                else
                {
                    casaEnpReale[1] = casellaDaCatturare[1] + 1;
                }

                enpFEN.clear();
                enpFEN = casaEnpReale;
                msg = "pedone da catturare in " + casellaDaCatturare + " \n";
                printf("%s", msg.c_str());
            }
        }
        // primaLettera = "P";
        primaLettera.clear();
        // notazione algebrica nel filedi registrazione  e in ultima mossa senza P
        check = pedone.checkMove(src, dest);
    }

    if (chessBoard[i][j].getChessPiece().getTypePiece() == ROOK)
    {
        primaLettera = "T";
        Rook torre = Rook();
        check = torre.checkMove(src, dest);
    }
    if (chessBoard[i][j].getChessPiece().getTypePiece() == KNIGHT)
    {
        primaLettera = "C";

        Knight cavallo = Knight();
        check = cavallo.checkMove(src, dest);
    }
    if (chessBoard[i][j].getChessPiece().getTypePiece() == BISHOP)
    {
        primaLettera = "A";

        Bishop alfiere = Bishop();
        check = alfiere.checkMove(src, dest);
    }
    if (chessBoard[i][j].getChessPiece().getTypePiece() == QUEEN)
    {
        primaLettera = "D";

        Queen donna = Queen();
        check = donna.checkMove(src, dest);
    }
    if (chessBoard[i][j].getBusySquare() == true && chessBoard[i][j].getChessPiece().getTypePiece() == KING)
    {
        primaLettera = "R";

        King re = King();
        check = re.checkMove(src, dest);
    }

    if (isScaccoRe == true && checkMoveKing(m, n, i, j, who) == 0)
    {
        check = false;
        printf("mossa non valida, il re è sotto scacco e questa mossa non lo toglie dallo scacco!\n");
    }
    if (!check)
    {
        printf("mossa non valida per il pezzo selezionato!\n");
        return 0;
    }

    string msg_str = "muove " + string(chessBoard[i][j].getChessPiece().getNamePiece()) + "\n";
    callTextToSpeech(msg_str);

    msg_str.clear();
    msg_str = "da " + src + " a " + dest + "\n";
    callTextToSpeech(msg_str);
    msg.clear();

    // inizio a settare la notazione algebrica che si basa come informazioni su  ultimaMossa
    algebricNotation(who, src, dest, primaLettera);
    /*
        dopo le istruzioni di sopra, essendo passati per il check del pedone
                                             sia EnP che casellaDaCatturare sono state annullate in chess.cpp
                                                 per cui ci basiamo su coloreAventeDiritto e se la casa destinazione del movimento corrisponde a quella dell'enpassant
                                                     */
    string operation = "mossa";
    bool isEnp = false;
    if (who == coloreAventeDiritto && dest == casaEnpReale && chessBoard[i][j].getChessPiece().getTypePiece() == PAWN && chessBoard[i][j].getBusySquare() == true)
    {
        isEnp = true;
    }

    if (isEnp == true || jumpFalsePiece == false && chessBoard[m][n].getBusySquare() == true && chessBoard[m][n].getChessPiece().getColorPiece() != chessBoard[i][j].getChessPiece().getColorPiece())
    {
        avvenutaCattura = true;
        operation.clear();
        msg.clear();
        msg = "Catturato ";

        operation = "cattura";
        if (isEnp)
        {
            // colore del pedone catturato
            string app = (coloreAventeDiritto == 'W' ? "Nero" : "Bianco");
            string pe = "Pedone" + app;
            msg.append(pe);
        }
        else
        {
            msg.append(chessBoard[m][n].getChessPiece().getNamePiece());
        }
        callTextToSpeech(msg);

        // se catturo un pezzo di intercept devo cancellarlo
        ChessUtility utilityMN;
        string src_incpt = utilityMN.getSquarePuntoDiVista(m, n, puntoDiVista);
        for (int w = 1; w < 100; w++)
        {
            if (PieceNoTouch[w] == src_incpt)
            {
                PieceNoTouch[w].clear();
            }
        }
    }
    // predispongo le variabili per operation="cattura"
    ChessPiece pezzoCatturato;
    string coloreCattura = "";
    pezzoCatturato = chessBoard[m][n].getChessPiece();
    if (pezzoCatturato.getTypePiece() == KING)
    {
        printf("Mossa non valida. Non si deve catturare il Re.\n");
        return 0;
    }
    if (chessBoard[m][n].getBusySquare() && chessBoard[m][n].getChessPiece().getColorPiece() == 'W')
    {
        coloreCattura = "Bianco";
    }

    else if (chessBoard[m][n].getBusySquare() && chessBoard[m][n].getChessPiece().getColorPiece() == 'B')
    {

        coloreCattura = "Nero";
    }
    else
    {
        coloreCattura = "--";
    }
    // Ora esegue la mossa ritenuta valida
    //   prendo il pezzo della casella src e lo metto nella casella dest
    ChessSquare casellaSrc = chessBoard[i][j];
    ChessSquare casellaDest = chessBoard[m][n];
    casellaDest.setChessPiece(casellaSrc.getChessPiece());
    casellaDest.setSquareName(dest.c_str());
    casellaDest.setBusySquare(true);
    chessBoard[m][n] = casellaDest;
    casellaSrc.setBusySquare(false);
    chessBoard[i][j] = casellaSrc;
    animationBoard(i, j, m, n);

    // qua devo fare i controlli se preso enpassant

    // inoltre dovendo gestire sia mossa che cattura basandomi su m,n devo fare il cotrollo dopo aver settato l'esecuzione della mossa
    if (avvenutaCattura == true && isEnp == true)
    {
        isEnp = false;
        // colore pedone che fa la cattura
        string app = (coloreAventeDiritto == 'W' ? "Bianco" : "Nero");
        string pe = "Pedone" + app;
        Pawn pedoneEnP = Pawn(pe.c_str(), coloreAventeDiritto);
        // colore del pedone catturato
        string colorCatched = (coloreAventeDiritto == 'W' ? "Nero" : "Bianco");
        insertUndoMove(who, src, dest, startColor, numMove, pedoneEnP, "enpassant", srcEnp, colorCatched);
    }
    else
    {
        if (jumpFalsePiece == false)
        {

            if (operation == "mossa")
            {
                pezzoCatturato = chessBoard[m][n].getChessPiece(); // caso "mossa" coloreCattura non viene considerato
            }
            insertUndoMove(who, src, dest, startColor, numMove, pezzoCatturato, operation, "--", coloreCattura);
        }
    }

    // pulisco case di PieceNoTouch che risultano vuote

    cleanSquare();
    jumpFalsePiece = false;

    string msg_castle;
    if (is_arrocco)
    {
        is_arrocco = false;
        // azzero la segnalazione per renderla disponibile anche all'avversario
        // se c'è arrocco da fare lo faccio dopo aver fatto la mossa del re sopra
        animationBoard(startRookRow, startRookCol, endRookRow, endRookCol);
        msg_castle = "Effettuato arrocco " + tipoArrocco + "\n";
        callTextToSpeech(msg_castle);
        insertUndoMove(who, src, dest, startColor, numMove, chessBoard[endRookRow][endRookCol].getChessPiece(), "arrocco");
    }

    gestione_toc();

    // NOTA: i controlli di scacco, scacco matto e stallo
    // vanno fatti per forza dalla prima mossa cosa molto evidente // sia per i problemi che per la partita.
    // Ad esempio in partita:
    // 1 g4 - e5; 2 f3 - Dh4# matto
    // controllo se devo promuovere il pedone
    if (chessBoard[m][n].getChessPiece().getTypePiece() == PAWN && (m == 0 || m == 7))
    {
        insertUndoMove(who, src, dest, startColor, numMove, chessBoard[i][j].getChessPiece(), "promozione");

        Pawn pedonePromosso = Pawn();
        pedonePromosso.promotion(src, dest, chessBoard[m][n].getChessPiece().getColorPiece());
    }
    if (KingMove == false)
    {
        cheScacco(m, n, who);
    }
    if (KingMove == true)
    {
        // se si muove il re si è certi di esserci tolti dallo scacco
        KingMove = false;
    }

    /*
    la funzione cheScacco va chiamata prima
    della funzione algebricNotation che predispone la
    scrittura di registrazione partita iniziale
    */

    /*
    Attenzione: il successivo pezzetto di codice su ultimaMossa
    si trova qui dato che serve sapere se ci sono scacco al re o matto,
    da scrivere nel log della registrazione della partita.    che vengono aggiunti nnellaa funzione cheScacco()
    */

    if (who == 'B')
    {
        WriteGameToFile(nameFile, ultimaMossa, reg_to_file);
        if (isEngineRunning())
        {
            callTextToSpeech(string("Valutazione posizione:"));
            evalPosition();
        }
    }

    // se EnP vale true ed ho diritto come colore a farlo ma non uso enpassant allora devo resettarlo
    if (who == coloreAventeDiritto && EnP == true && (src != pawnEnP.front() || src != pawnEnP.back()))
    {
        EnP = false;
        pawnEnP.clear();
        coloreAventeDiritto = ' ';
        casellaDaCatturare.clear();
        printDebug(string("enpassant non utilizzato e non più utilizzabile\n"));
    }
    return 1;
}
int insertMoveNav(char who)
{
    // ritorna 0 per errore, 1 per ok
    string source, destination;
    int statusMove = 0;

    source.clear();
    destination.clear();

    source.append(sourceOnBoard);
    destination.append(destOnBoard);

    printDebug("debug: mossa da casella iniziale " + source + "\n");
    printDebug("debug: mossa verso casella destinazione " + destination + "\n");

    if (source == destination)
    {
        callTextToSpeech(string("Mossa non valida. Hai inserito come destinazione ancora la casa di partenza!\n"));
        return 0;
    }
    /*elimina la prima lettera
    del pezzo e si concentra solo sulle caselle contenenti i pezzi
    per richiamare il metodo move
    useremo P per pedone, A per alfiere, R per re, T per torre, D per Donna, C per cavallo
*/
    string src = source.substr(1);
    string dest = destination.substr(1);
    src[0] = tolower(src[0]);
    dest[0] = tolower(dest[0]);

    statusMove = move(src, dest, who);
    if (statusMove == 0)
    {
        callTextToSpeech(string("mossa non valida\n"));
        string trc = "mosso da " + src + " a " + dest + "\n";
        printf("%s\n", trc.c_str());
        return 0;
    }
    return 1;
}

void clear_globali()
{
    // pulizia variabili delle globali
    // tranne gRow , gCol, puntoDiVista

    enpFEN.clear();
    online = false;
    colorOnline.clear();
    roomOnline.clear();
    playerOnline.clear();
    jumpFalsePiece = false;
    who_play = '@';
    startRookCol = 0;
    startRookRow = 0;
    endRookCol = 0;
    endRookRow = 0;
    tipoArrocco.clear();
    is_arrocco = false;
    level = 20;
    skill = 0;
    maxThreads = 0;
    maxHashTable = 0;
    multiPV = 0;
    allMovesToComputer.clear();
    stockfish_color.clear();
    nameFile.clear();
    nameProblem.clear();
    startColor = 0;
    KingMove = false;
    numMove = 0;
    isProblem = false;
    EnP = false;
    pawnEnP.clear();
    casellaDaCatturare.clear();
    ultimaMossa.clear();
    reg_to_file = false;
    arrocco.clear();
    sourceOnBoard.clear();
    destOnBoard.clear();
    avvenutaCattura = false;
    isScaccoRe = false;
    return;
}
void risolviProblema(int chi_gioca)
{
    // per risolvere il problema gioca sempre il computer
    int count_moves = 0, giocatore = chi_gioca;
    string fen, cmd, move, who_play;

    // fen utilizza numMove che conviene che parta da 1
    numMove = 1;

    // il primo comando attiva stockfish per farlo muovere col colore passato in input
    // attivo il computer se non attivo
    if (!isEngineRunning())
    {
        if (start())
        {
            setSkill(20);
            printf("Problema analizzato dal computer (max 10 mosse)\n");
        }

        else
        {
            printf("indisponibilità connessione al computer , esco per consentirti di riprovare\n");
            exit(1);
        }
    }
    char color = (giocatore == 0 ? 'W' : 'B');
    who_play = (color == 'W' ? "Bianco" : "Nero");
    printf("Gioca il colore%s\n", who_play.c_str());
    fen = getPositionFen(color);

    cmd.clear();
    cmd = "position fen " + fen + " moves";
    cmd = cmd + "\n";
    sendCommand(cmd);
    sendCommand("go depth 20\n");

    Sleep(3000);
    printf("mossa numero %d\n", numMove);

    while (count_moves < 10)
    {
        count_moves++; // analisi solo  per 10 mosse
        // riceve la mossa a seguito della fen proposta
        /*
         devo però verificare lo stato di stockfish che potrebbe trovarsi in
         matto, stallo, patta o abbandona. Per farlo setto
         un timer e gestisco la getOutputMove  in asincrono
        */
        string app;

        app.clear();
        auto timeout_durata = std::chrono::minutes(2);
        std::future<std::string> futuroRisultato = std::async(std::launch::async, getOutputMove);
        // aspetta il risultato
        if (futuroRisultato.wait_for(timeout_durata) == std::future_status::timeout)
        {
            // timeout scaduto
            callTextToSpeech("Stockfish abbandona");
            sendCommand("quit\n");
            Sleep(2000);
            exit(0);
        }
        else
        {

            // risposto a tempo
            app = futuroRisultato.get();
            if (app == "1")
            {
                sendCommand("quit\n");
                Sleep(2000);
                exit(0);
            }

            allMovesToComputer.clear();

            allMovesToComputer.append(app);
            interpreter(app);
            int status = insertMoveNav(color);

            if (status == 0)
            {
                callTextToSpeech(string("Analisi interrotta, continua tu\n"));
                return;
            }
            // settingCoord serve ad animationBoard(Vedi utility.cpp)
            settingCoord();
            drawBoard();
            Sleep(1000);
            animationBoard(startSolRow, startSolCol, endSolRow, endSolCol);
            cmd.clear();
        }
        // gli devo rimandare la nuova posizione per farlo giocare con altro colore

        giocatore++;
        if (giocatore > 1)
        {
            giocatore = 0;
            numMove++;
            printf("mossa numero %d\n", numMove);
        }
        startColor = giocatore; // allineo la globale
        color = (giocatore == 0 ? 'W' : 'B');
        who_play = (color == 'W' ? "Bianco" : "Nero");

        fen = getPositionFen(color);
        cmd.clear();
        cmd = "position fen " + fen + " moves";
        cmd = cmd + "\n";
        sendCommand(cmd);
        sendCommand("go depth 20\n");
        printf("Analisi in corso\n");

        Sleep(5000);
    }
    callTextToSpeech(string("Fine analisi. Il controllo ora torna a te\n"));
    return;
}
/*
funzione che gestisce il contrl-A per forzare l'apertura desiderata giocata
prima ricava con getPositionFen la fen della posizione su scacchiera
poi valorizza allMovesToComputer, attiva stockfish, invia il comando con sendCommand
*/

void forzaApertura()
{
    string fen = "", cmd = "";
    char color = (startColor == 0 ? 'W' : 'B');

    drawBoard();

    fen = getPositionFen(color);
    // WriteGameToFile("..//logdir//testfen.txt", fen, true);

    cmd = "position fen " + fen + " moves\n";
    allMovesToComputer = fen + " ";

    callTextToSpeech(string("Fai Alt-Tab per spostarti sulla console per rispondere alle domande\n"));
    printf("Dati da Inserire\n");
    printf("skill : ");
    Sleep(1500);
    ::
        callTextToSpeech(string("Inserisci lo skill che deve avere il computer e fai return (da 0 a 20):\n"));
    std::cin >> skill;
    std::cin.ignore();
    printf("colore : ");
    Sleep(1500);
    callTextToSpeech(string("Inserisci colore con cui deve giocare il computer, seguito da return (1=Bianco, 2=Nero)\n"));

    int risposta;
    std::cin >> risposta;
    std::cin.ignore();
    if (risposta == 1)
    {
        stockfish_color = "Bianco";
        // puntoDiVista = "Bianco";
    }
    else if (risposta == 2)
    {
        stockfish_color = "Nero";
        // puntoDiVista = "Bianco";
    }
    else
    {
        callTextToSpeech(string("Sono ammessi solo i valori 1 o 2, esco per farti ricominciare correttamente\n"));
        exit(0);
    }
    /*
        qua controllo che se è stato chiesto che il computer giochi nero, deve essere stata già giocata
        una mossa di bianco per poter inviare il comando con la fen corretta. Viceversa nel caso che il computer deve giocare bianco
    */

    int numPrec = numMove;
    if (stockfish_color == "Nero" && (numMove < 1 || whoPlayed != 'W'))
    {
        callTextToSpeech(string("Per assegnare il nero al computer, devi prima giocare una mossa di bianco e poi fare CntrlA\n"));
        stockfish_color.clear();
        numMove = numPrec;
        return;
    }
    if (whoPlayed == '@')
    {
        whoPlayed = 'B';
    }
    if (stockfish_color == "Bianco" && numMove > 0 && whoPlayed != 'B')
    {
        callTextToSpeech(string("Per assegnare il bianco al computer, devi aver giocato precedentemente col nero e poi fare CntrlA\n"));
        stockfish_color.clear();
        numMove = numPrec;

        return;
    }
    if (start())
    {
        callTextToSpeech(string("Il computer risulta pronto per giocare"));

        setParam(maxThreads, maxHashTable, multiPV);
        setSkill(skill);
        sendCommand(cmd);
        sendCommand("go depth 20\n");
    }
    else
    {
        callTextToSpeech(string("indisponibilità connessione al computer , esco per consentirti di riprovare\n"));
        exit(1);
    }
    return;
}
int whatInitialSquare(std::string mossaCasaDestinazione, char who)
{
    /*
        questa funzione riceve la singola mossa mossaCasaDestinazione e il colore del giocatore che deve giocare
        Tale mossa può essere di lunghezza 2 o 3 o 4 (4 quando ci sono due pezzi che possono fare la stessa mossa es. Tac1) .
        La mossa rappresenta solo la mossa nella casa di destinazione.
        la mossa è del tipo e4 per un pedone oppure Ac5 per un pezzo. Tale mossa rappresenta la mossa verso la casa destinazione soltanto.
        Le mosse che arrivano in input non avranno O-O oppure O-O-O ma solo una mossa di Re per sfruttare il software già sviluppato.
        Una mossa come e4 di pedone, avrà la casa precedente sulla stessa colonna, indietro (numero minore) se giocatore bianco, avanti (numero maggiore) se giocatore nero, mentre non è la stessa colonna solo nei casi di cattura o enpassant e in tal caso il pedone va in diagonale a distanza 1.
        Invece per trovare la casa di partenza dei pezzi sapendo il colore del pezzo , si deve cercare dove è posizionato sulla scacchiera un pezzo che possa arrivare alla destinazione (di sicuro è unico) e si trova in tal modo la mossa della casa iniziale.

        Per cui la funzione ricerca il colore della casa della mossa ricevuta, per determinare:
        da who il colore del pezzo
        deve cercare la casa della mossa di partenza e
        settare sourceOnBoard e destOnBoard, facendo attenzione che le due variabili siano di lunghezza 3 ovvero aventi ad inizio la lettera P se si tratta di mossa di pedone o le altre lettere: T per torre, C per cavallo, A per alfiere, D per donna e R per re.
    */

    int status = 0; // ok
    bool trovato = false;
    string casa = "";
    char typePiece, colonna, riga;
    char colonna_scelta = '@'; // valorizzata se ci sono due pedoni che possono arrivare alla stessa casa di destinazione e serve scegliere la colonna di partenza
    string appoggio = "";
    sourceOnBoard.clear();
    destOnBoard.clear();
    if (mossaCasaDestinazione.length() < 2 || mossaCasaDestinazione.length() > 6)
    {
        callTextToSpeech(string("Mossa non valida, la mossa deve essere del tipo e4 per pedone oppure Ac5 per pezzo, riprova\n"));
        return 1;
    }
    // pulizia da mossaCasaDestinazione di + e di # quando abbiamo mosse come Tf7+ o Taxc1+ o Dd6# oppure Txf7+

    if (mossaCasaDestinazione.find('+') != std::string::npos)
    {
        mossaCasaDestinazione.erase(std::remove(mossaCasaDestinazione.begin(), mossaCasaDestinazione.end(), '+'), mossaCasaDestinazione.end());
    }
    if (mossaCasaDestinazione.find('#') != std::string::npos)
    {
        mossaCasaDestinazione.erase(std::remove(mossaCasaDestinazione.begin(), mossaCasaDestinazione.end(), '#'), mossaCasaDestinazione.end());
    }

    // pulizia di 1-0 o 0-1 o 1/2-1/2 in caso di mosse che arrivano da notazione algebrica completa
    if (mossaCasaDestinazione == "-0" || mossaCasaDestinazione == "-1" || mossaCasaDestinazione == "1-0" || mossaCasaDestinazione == "0-1" || mossaCasaDestinazione == "1/2-1/2")
    {
        callTextToSpeech(string("fine partita\n"));

        return 2;
    }

    // pulizia delle x nelle mosse dei pezzi o dei pedoni per le catture , mosse d lunghezza 4 o 5 (5 solo pezzi es: Taxc1+) che diventano di lunghezza 3 o 4

    if (mossaCasaDestinazione.length() == 4 && std::isupper(mossaCasaDestinazione[0]) && mossaCasaDestinazione[1] == 'x')
    {
        // pezzo

        appoggio.clear();
        appoggio = mossaCasaDestinazione;
        mossaCasaDestinazione.clear();
        mossaCasaDestinazione.push_back(appoggio[0]);
        mossaCasaDestinazione.append(appoggio.substr(2, 2));
    }
    if (mossaCasaDestinazione.length() == 5 && std::isupper(mossaCasaDestinazione[0]) && mossaCasaDestinazione[2] == 'x')
    {
        // pezzo

        appoggio.clear();
        appoggio = mossaCasaDestinazione;
        mossaCasaDestinazione.clear();
        mossaCasaDestinazione.push_back(appoggio[0]);
        mossaCasaDestinazione.append(appoggio.substr(3, 2));
    }

    if (mossaCasaDestinazione.length() == 4 && mossaCasaDestinazione[1] == 'x' && std::islower(mossaCasaDestinazione[0]))
    {
        // pedone

        appoggio.clear();
        appoggio = mossaCasaDestinazione;
        mossaCasaDestinazione.clear();
        mossaCasaDestinazione.append(appoggio.substr(2, 2));
        colonna_scelta = appoggio[0];
    }

    if (mossaCasaDestinazione.length() == 2)
    {
        // mossa di pedone
        typePiece = 'P';
        casa = mossaCasaDestinazione.substr(0, 2);
        colonna = casa[0];
        riga = casa[1];
        sourceOnBoard.append("P");
        destOnBoard.append("P");
        destOnBoard.append(mossaCasaDestinazione);
    }
    else if (mossaCasaDestinazione.length() == 3)
    {
        // mossa di pezzo
        typePiece = mossaCasaDestinazione[0];
        casa = mossaCasaDestinazione.substr(1, 2);
        colonna = casa[0];
        riga = casa[1];
        destOnBoard = mossaCasaDestinazione;
    }
    else if (mossaCasaDestinazione.length() == 4)
    {
        // mossa di pezzo
        typePiece = mossaCasaDestinazione[0];
        casa = mossaCasaDestinazione.substr(2, 2);
        colonna = casa[0];
        riga = casa[1];
    }

    // stabiliamo la casaprecedente di pedone
    if (typePiece == 'P')
    {
        char colonnaSrc;
        char rigaSrc;
        /*

In base al punto di vista sia bianco che nero bisogna decidere nei casi di pedone bianco spinto in riga '4' da che riga proviene.
Analogamente per il nero nei due punti di vista per la spinta in riga '5'.
Poi ci sono i casi diversi di spinte oltre '4' per il bianco eoltre '5' per il nero dove i pedoni si muovono solo di un passo
*/

        // Stabiliamo la casa precedente del pedone in base al colore e al punto di vista
        // Determiniamo la colonna di provenienza
        // Se la colonna è la stessa, è una mossa normale; altrimenti è una cattura o enpassant
        if (colonna_scelta != '@')
        {
            colonna = colonna_scelta;
        }

        if (colonna == casa[0])
        {
            // Mossa normale: colonna non cambia
            colonnaSrc = colonna;
        }
        else
        {
            // Cattura o enpassant: il pedone si muove in diagonale

            if (colonna_scelta != '@')
            {
                // se è stata specificata la colonna di partenza in caso di due pedoni che possono arrivare alla stessa casa di destinazione, allora prendo quella
                colonnaSrc = colonna_scelta;
            }
            if (colonna_scelta == '@' && colonna > casa[0])
            {
                // Movimento verso destra
                colonnaSrc = colonna - 1;
            }
            else if (colonna_scelta == '@' && colonna < casa[0])
            {
                // Movimento verso sinistra
                colonnaSrc = colonna + 1;
            }
        }

        // Cerchiamo la riga di provenienza controllando quale casa è occupata da un pedone
        if (who == 'W')
        {
            // Pedone Bianco: proviene da una riga inferiore (indietro)
            // Verifichiamo prima riga-1, poi riga-2
            rigaSrc = static_cast<char>(riga - 1);
            ChessUtility utility;
            vector<int> checkIdx = utility.getIndexPuntoDiVista(string(1, colonnaSrc) + rigaSrc, puntoDiVista);
            int checkRow = checkIdx.front();
            int checkCol = checkIdx.back();

            if (checkRow >= 0 && checkRow < 8 && checkCol >= 0 && checkCol < 8 &&
                chessBoard[checkRow][checkCol].getBusySquare() &&
                chessBoard[checkRow][checkCol].getChessPiece().getTypePiece() == PAWN &&
                chessBoard[checkRow][checkCol].getChessPiece().getColorPiece() == who)
            {
                // Trovato il pedone in riga-1
                rigaSrc = static_cast<char>(riga - 1);
            }
            else
            {
                // Altrimenti prova riga-2
                rigaSrc = static_cast<char>(riga - 2);
            }
        }
        else
        {
            // Pedone Nero: proviene da una riga superiore (avanti)
            // Verifichiamo prima riga+1, poi riga+2
            rigaSrc = static_cast<char>(riga + 1);
            ChessUtility utility;
            vector<int> checkIdx = utility.getIndexPuntoDiVista(string(1, colonnaSrc) + rigaSrc, puntoDiVista);
            int checkRow = checkIdx.front();
            int checkCol = checkIdx.back();

            if (checkRow >= 0 && checkRow < 8 && checkCol >= 0 && checkCol < 8 &&
                chessBoard[checkRow][checkCol].getBusySquare() &&
                chessBoard[checkRow][checkCol].getChessPiece().getTypePiece() == PAWN &&
                chessBoard[checkRow][checkCol].getChessPiece().getColorPiece() == who)
            {
                // Trovato il pedone in riga+1
                rigaSrc = static_cast<char>(riga + 1);
            }
            else
            {
                // Altrimenti prova riga+2
                rigaSrc = static_cast<char>(riga + 2);
            }
        }

        sourceOnBoard.push_back(colonnaSrc);
        sourceOnBoard.push_back(rigaSrc);
    }

    if (typePiece != 'P')
    {
        // mossa di pezzo diverso da pedone, cerco sulla scacchiera il pezzo che si muove

        TypePiece ttPiece;
        switch (typePiece)
        {
        case 'T':
            ttPiece = ROOK;
            break;
        case 'C':
            ttPiece = KNIGHT;
            break;
        case 'A':
            ttPiece = BISHOP;
            break;
        case 'D':
            ttPiece = QUEEN;
            break;
        case 'R':
            ttPiece = KING;
            break;
        default:
            callTextToSpeech(string("Mossa non valida, la mossa di pezzo deve iniziare con T per torre, C per cavallo, A per alfiere, D per donna e R per re, riprova\n"));
            return 1;
            break;
        }
        string casaSrc = "";
        string casaDest = "";

        for (int r = 0; r < 8 && !trovato; r++)
        {
            for (int c = 0; c < 8 && !trovato; c++)
            {
                if (chessBoard[r][c].getBusySquare() && chessBoard[r][c].getChessPiece().getColorPiece() == who && chessBoard[r][c].getChessPiece().getTypePiece() == ttPiece)
                {
                    ChessSquare square = chessBoard[r][c];
                    casaSrc.clear();
                    casaSrc = square.getSquareName();

                    // caso due pezzi stesso tipo che possono fare la stessa mossa
                    if (mossaCasaDestinazione.length() == 4 && casaSrc[0] != mossaCasaDestinazione[1])
                    {
                        continue;
                    }
                    if (mossaCasaDestinazione.length() == 3)
                    {
                        casaDest.clear();
                        casaDest = mossaCasaDestinazione.substr(1, 2);
                    }

                    if (mossaCasaDestinazione.length() == 4)
                    {
                        casaDest.clear();
                        casaDest = mossaCasaDestinazione.substr(2, 2);
                    }

                    // Verifica che il pezzo possa raggiungere la casa di destinazione
                    bool canMove = false;

                    switch (ttPiece)
                    {
                    case ROOK:
                    {
                        Rook torre;
                        canMove = torre.checkMove(casaSrc, casaDest);
                        break;
                    }
                    case KNIGHT:
                    {
                        Knight cavallo;
                        canMove = cavallo.checkMove(casaSrc, casaDest);
                        break;
                    }
                    case BISHOP:
                    {
                        Bishop alfiere;
                        canMove = alfiere.checkMove(casaSrc, casaDest);
                        break;
                    }
                    case QUEEN:
                    {
                        Queen donna;
                        canMove = donna.checkMove(casaSrc, casaDest);
                        break;
                    }
                    case KING:
                    {
                        King re;
                        // canMove = re.checkMove(casaSrc, casaDest);
                        canMove = true; // il checkMove fa arroccare
                        break;
                    }
                    default:
                        break;
                    }

                    if (canMove)
                    {
                        sourceOnBoard.clear();
                        sourceOnBoard.push_back(typePiece);
                        sourceOnBoard.append(casaSrc);
                        trovato = true;
                        destOnBoard.clear();
                        destOnBoard.push_back(typePiece);
                        destOnBoard.append(casaDest);
                    }
                }
            }
        }
    }
    return status;
}
void reStart()
{

    // Inizializzazione della scacchiera
    deleteUndo();
    if (isEngineRunning())
    {
        stop();
        callTextToSpeech(string("Fermo il computer\n"));
    }
    reg_to_file = false;
    clear_globali();
    cleanBoard();
    readChessParam();
    init();
    drawBoard();
    callTextToSpeech(string("Scacchiera inizializzata\n"));
    gioca();
    return;
}
void clearNoTouch()
{
    IndiceNoTouch = 0;
    for (int i = 0; i < 100; i++)
    {
        PieceNoTouch[i].clear();
    }
    return;
}

bool searchPieceNoTouch(string src)
{

    for (int i = 0; i < 100; i++)
    {
        if (src == PieceNoTouch[i])
        {
            printf("Pezzo in %s che para già uno scacco!\n", src.c_str());
            return true;
        }
    }
    return false;
}

void cleanSquare()
{

    int p = 0, q = 0;
    ChessUtility utility;
    for (int i = 0; i < 100; i++)
    {
        if (PieceNoTouch[i].empty())
        {
            continue;
        }

        vector<int> Index = utility.getIndexPuntoDiVista(PieceNoTouch[i], puntoDiVista);
        p = Index.front();
        q = Index.back();
        if (chessBoard[p][q].getBusySquare() == false)
        {
            PieceNoTouch[i].clear();
        }
    }
    return;
}

void checkReceiveRete(char who)
{
    // vede se serve ricevere qualcosa se siamo in rete
    int status = 0;
    string moveFromServer = "";
    int tentativi = 0;
    if (online && (colorOnline == "Nero" || colorOnline == "Bianco" && numMove > 0))
    {

        while (status == 0)
        {
            moveFromServer.clear();
            while (moveFromServer.empty() || moveFromServer == "NONE")
            {
                Sleep(2000);

                moveFromServer = receiveMove(roomOnline, playerOnline);
            }
            if (moveFromServer == "quitAB")
            {
                callTextToSpeech(string("Il tuo avversario ha abbandonato"));
                reStart();
            }

            if (moveFromServer == "none")
            {
                callTextToSpeech(string("Il tuo avversario ha chiuso la connessione"));
                reStart();
            }

            if (!moveFromServer.empty() && moveFromServer != "NONE")
            {
                // Evita concatenazioni multiple di source/destination quando si riprova dopo un errore.
                sourceOnBoard = moveFromServer.substr(0, 3);
                destOnBoard = moveFromServer.substr(3, 3);
                status = insertMoveNav(who);
                if (status != 0)
                {
                    if (who == 'W')
                    {
                        numMove++;
                    }
                }
                else
                {
                    callTextToSpeech(string("Mossa ricevuta non valida, attendo nuova mossa dal server\n"));
                    sourceOnBoard.clear();
                    destOnBoard.clear();
                    moveFromServer.clear();
                    tentativi++;
                    if (tentativi > 2)
                    {
                        callTextToSpeech(string("Mossa ricevuta non valida, superati i 2 tentativi. Esco per farti inserire il player correttamente fin dall'inizio\n"));
                        exit(0);
                    }
                }
            }
        }
    }
    return;
}

void invioMossaRete()
{

    if (online)
    {

        string mossa;
        mossa.clear();
        mossa.append(sourceOnBoard);
        mossa.push_back(sourceOnBoard[0]);
        mossa.append(destOnBoard.substr(1));

        if (sendMove(roomOnline, playerOnline, mossa) == false)
        {
            callTextToSpeech(string("Errore nell'invio della mossa al server, esco per consentirti di riprovare\n"));
            exit(1);
        }

        // callTextToSpeech(string("Mossa inviata al server: ") + mossa);
    }
    return;
}
void checkKings()
{

    // su tutta la scacchiera cerco i due re se esistono altrimenti è un errore e
    // devo uscire dal programma
    bool foundWhiteKing = false;
    bool foundBlackKing = false;
    ChessUtility utility;
    string casa, my_king;
    ;
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            casa.clear();
            casa = utility.getSquarePuntoDiVista(row, col, puntoDiVista);

            if (chessBoard[row][col].getBusySquare() && chessBoard[row][col].getChessPiece().getTypePiece() == KING && chessBoard[row][col].getChessPiece().getColorPiece() == 'W')
            {
                foundWhiteKing = true;
                my_king = "Re bianco in casa " + casa + "\n";
                Sleep(1500);
                callTextToSpeech(my_king);
            }
            if (chessBoard[row][col].getBusySquare() && chessBoard[row][col].getChessPiece().getTypePiece() == KING && chessBoard[row][col].getChessPiece().getColorPiece() == 'B')
            {
                foundBlackKing = true;
                my_king = "Re nero in casa " + casa + "\n";
                Sleep(1500);
                callTextToSpeech(my_king);
            }
        }
    }
    if (!foundWhiteKing || !foundBlackKing)
    {
        callTextToSpeech(string("Errore: assenza dei descrittori B o N oppure uno o entrambi i re non sono presenti sulla scacchiera. Esco dal programma.\n"));
        exit(1);
    }
    return;
}

void checkNumbers()
{

    // la funzione conta i pezzi del bianco e del nero
    // se il loro numero supera il 16 per il bianco o per il nero esiste un errore
    int numBianchi = 0, numNeri = 0;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (chessBoard[i][j].getBusySquare())
            {
                if (chessBoard[i][j].getChessPiece().getColorPiece() == 'W')
                {
                    numBianchi++;
                }
                if (chessBoard[i][j].getChessPiece().getColorPiece() == 'B')
                {
                    numNeri++;
                }
            }
        }
    }

    if (numBianchi > 16)
    {
        callTextToSpeech(string("Errore: il numero di pezzi bianchi supera 16. Esco dal programma.\n"));
        exit(1);
    }
    if (numNeri > 16)
    {
        callTextToSpeech(string("Errore: il numero di pezzi neri supera 16. Esco dal programma.\n"));
        exit(1);
    }
    return;
}