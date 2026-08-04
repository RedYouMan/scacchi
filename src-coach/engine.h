// engine.h
#ifndef ENGINE
#define ENGINE
#include <iostream>
#include <string>
using namespace std;
// Function prototypes
void handle_sigint(int sig);
bool start();
void sendCommand(const string &command);
void stop();
bool readConfigEngine(string &exePath);
string getOutputMove();
string getUciok();

bool isEngineRunning();
string getPositionFen(char who);
string getPositionFenBlackStart0();
string getPositionFenBlackStart7();
string getPositionFenLast(char who);
void evalPosition();
void setSkill(int skill);
void setParam(int maxThreads, int maxHashTable, int multiPV);
void getAsciiArt_withFen();
bool isSettingParam();
bool isReady();
void risolviProblema(int start);
string evalWithStop(string stockfish_color);
float evalStock();
#endif