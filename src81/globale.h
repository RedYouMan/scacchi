#ifndef GLOB__SCACCHI
#define GLOB_SCACCHI
#include <string>
#include "chess.h"
#include <atomic>
extern std::atomic<bool> interrompiVocale;

extern string enpFEN;
extern bool online;
extern string colorOnline;
extern string roomOnline;
extern string playerOnline;
extern string PieceNoTouch[100];
extern int IndiceNoTouch;
extern bool jumpFalsePiece;
extern bool is_arrocco;

extern int startRookCol, startRookRow, endRookCol, endRookRow;
extern string tipoArrocco; // azzero la segnalazione per renderla disponibile anche all'avversario
extern int startSolRow, startSolCol, endSolRow, endSolCol;

/* variabili globali */

// per printDebug mettendo a false non stampa
extern bool test;
// per consentire nei test di sospendere un problema mettere a true
extern bool sospendi_test;
// per farsi produrre un file in directory log contenente il fen
extern bool test_fen;
// le mosse inviate a stockfish(fen) oppure la mossa ricevuta
extern string allMovesToComputer;
// la profondità di analisi di stockfish
extern int engineDepth;
extern int level; // livello di profondità
extern int skill;
// il colore con cui deve giocare stockfish
extern string stockfish_color;
// chi ha appena giocato
extern char whoPlayed;
extern char who_play;
// Il numero di mossa
extern int numMove;
// definipzione della scacchiera
extern ChessSquare chessBoard[8][8];
// Gestione en passant
extern bool EnP;
extern vector<string> pawnEnP;
extern string casellaDaCatturare;
extern char coloreAventeDiritto;
// gestione ultima mossa
extern string ultimaMossa;
// gestione registrazione
extern bool reg_to_file;
extern string nameFile;
// gestione per caricamento problemi da risolvere indipendentemente da chi muove per prima
extern bool isProblem;
extern string nameProblem;
// per scrittura su file di arrocco
extern string arrocco;
// Per stabilire il colore che deve giocare sia per i problemi che nella partita
extern int startColor;
// dati di navigazione
extern string sourceOnBoard;
extern string destOnBoard;
extern int gRow, gCol; // per la navigazione su scacchiera
extern bool avvenutaCattura;
// punto di vista da cui giocare
extern string puntoDiVista;
// utile per verificare se tolti dallo scacco
extern bool KingMove;
// gestione scacco al re per vedere se lo scacco è stato parato
extern bool isScaccoRe;
extern int maxThreads;
extern int maxHashTable;
extern int multiPV;

#endif
