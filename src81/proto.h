#ifndef PROTO
#define PROTO
#include <iostream>
#include <string>
#include "chess.h"
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;
void signalHandler(int signum);
void settingCoord();
void gestione_Problemi(int caso);
sf::Vector2f getCoords(int col, int row);
int callTextToSpeech(std::string stringaInput);
void animationBoard(int startRow, int startCol, int endRow, int endCol);
void startGrafica();
void deleteWindow(); // nei casi quit, nuova partita o exit
sf::RenderWindow *getMyWindow();
void setWindow(sf::RenderWindow *p);
void drawBoard();
void viewValid();
void cleanBoard();
void help();
void apriUrl();
void gestione_toc();
void printDebug(string msg);
int move(string source, string destination, char who);
void displayBoard();
void clear_globali();
void insertNameFile();
void playWhite();
void playBlack();
void WriteGameToFile(string nameFile, string record, bool scrivi);
void gioca();
void loadFile(int tipoPath, bool cambioColore);
bool makeConfig(string name, int tipoPath);
bool setPieceIntoSquare(string com);
void setPiece(char color, string mossa);
void init();
void start_game();
void onSquare(int row, int col);
void navOnBoard(char who);
int insertMoveNav(char who);
void Sospendi(bool cambioColore);
void algebricNotation(char who, string src, string dest, string primaLettera);
// ultime
void cheScacco(int m, int n, char who);
void cheStallo(char who);
bool isPiecePresent(int m, int n, int x, int y, char color);
bool isPieceIntercept(int m, int n, int x, int y, char who);
bool isPieceDefended(int m, int n, char who);
int checkMoveKing(int m, int n, int x, int y, char who);
void whatColorComputer();
void interpreter(string response);
void readChessParam();
void forzaApertura();
int whatInitialSquare(std::string mossa, char who);
void reStart();
void patteElementari();
void clearNoTouch();
bool searchPieceNoTouch(string src);
void cleanSquare();
void simpleRefresh(sf::RenderWindow *window);
void myRefresh();
// parte undo
void deleteUndo();
void insertUndoMove(char who, string src, string destination, int num, int numWho, ChessPiece pieceCatched, string operation, string srcEnp = "--", string colorCatched = "--");
void undoMove();
void movePiece(string src, string dest);
void movePiece(int srow, int scol, int drow, int dcol);

void setValid00();
void setValid000();
void removePiece(string square);
std::string roomSuggested();
std::string receiveMove(const std::string &room, const std::string &player);
bool sendMove(const std::string &room, const std::string &player, const std::string &move);
int test_main_net();
void checkReceiveRete(char who);
void invioMossaRete();
void checkMate();
#endif