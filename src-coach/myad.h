#ifndef MYAD
#define MYAD
// header di adattamento per la compilazione derivata da board.cpp
#include <iostream>
#include <atomic>
std::atomic<bool> interrompiVocale(false);

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

#endif