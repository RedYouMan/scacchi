// pgnreader.h
// Modulo per la lettura e gestione dei file PGN
#ifndef PGNREADER_H
#define PGNREADER_H

#include <string>
#include <vector>

struct Game
{
    std::string event;
    std::string site;
    std::string date;
    std::string round;
    std::string white;
    std::string black;
    std::string result;
    std::string moves;
};

class PGNReader
{
private:
    std::vector<Game> games;

public:
    void writeTrace(const std::string &nameFile, const std::string &trace);

    std::vector<std::string> getOpeningsBooks(const std::string &path);
    bool readFile(const std::string &filename);
    void clearGames();
    int getGamesCount() const;
    const std::string *getMoves(int gameIndex) const;
    std::string getInfoMoves(int NumGame);
    const std::string translateMoves(const std::string &moves);

    void displayGame(int gameIndex) const;
};

#endif
