// pgnreader.cpp
#pragma warning(disable : 4244 4267)

#include <cctype>
#include "pgnreader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <filesystem>
#pragma warning(disable : 4267)

using namespace std;

void PGNReader::writeTrace(const std::string &nameFile, const std::string &trace)
{
    std::ofstream traceFile(nameFile, std::ios::app);
    if (traceFile.is_open())
    {
        traceFile << trace << std::endl;
        traceFile.close();
    }
    else
    {
        std::cerr << "Error opening file: " << nameFile << std::endl;
    }
}

std::vector<std::string> PGNReader::getOpeningsBooks(const std::string &path)
{
    std::vector<std::string> pgnFiles;
    try
    {
        for (const auto &entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".pgn")
            {
                pgnFiles.push_back(entry.path().string());
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Errore nella lettura della directory: " << e.what() << std::endl;
    }
    return pgnFiles;
}

void PGNReader::clearGames()
{
    games.clear();
}

std::string PGNReader::getInfoMoves(int NumGame)
{
    if (NumGame < 0 || NumGame >= static_cast<int>(games.size()))
    {
        return "";
    }

    const Game &game = games[NumGame];
    std::string info = "Evento: " + game.event + "\n Luogo: " + game.site + "\n Data: " + game.date +
                       "\n Round: " + game.round + "\n Bianco: " + game.white +
                       "\n Nero: " + game.black + "\n Risultato: " + game.result;
    return info;
}

bool PGNReader::readFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return false;
    }

    Game currentGame;
    std::string line;

    while (std::getline(file, line))
    {
        // CORREZIONE: Trim spazi iniziali e finali
        if (line.empty())
        {
            if (!currentGame.event.empty())
            {
                games.push_back(currentGame);
                currentGame = Game();
            }
            continue;
        }

        // CORREZIONE: Verifica che la linea non sia vuota prima di accedere a line[0]
        if (!line.empty() && line[0] == '[')
        {
            size_t closePos = line.find(']');
            // CORREZIONE: Estrai la chiave tra '[' e lo spazio seguente
            size_t spacePos = line.find(' ');
            std::string key = line.substr(1, spacePos - 1);
            // CORREZIONE: Estrai il valore tra i doppi apici
            size_t firstQuote = line.find('"');
            size_t lastQuote = line.rfind('"');
            std::string value = line.substr(firstQuote + 1, lastQuote - firstQuote - 1);

            if (key == "Event")
                currentGame.event = value;
            else if (key == "Site")
                currentGame.site = value;
            else if (key == "Date")
                currentGame.date = value;
            else if (key == "Round")
                currentGame.round = value;
            else if (key == "White")
                currentGame.white = value;
            else if (key == "Black")
                currentGame.black = value;
            else if (key == "Result")
                currentGame.result = value;
        }
        else if (!line.empty() && line[0] != '%')
        {
            currentGame.moves += line + " ";
        }
    }

    if (!currentGame.event.empty())
    {
        games.push_back(currentGame);
    }

    file.close();
    return true;
}

int PGNReader::getGamesCount() const
{
    return games.size();
}

const std::string *PGNReader::getMoves(int gameIndex) const
{
    if (gameIndex < 0 || gameIndex >= static_cast<int>(games.size()))
    {
        return nullptr;
    }
    return &games[gameIndex].moves;
}

void PGNReader::displayGame(int gameIndex) const
{
    if (gameIndex < 0 || gameIndex >= static_cast<int>(games.size()))
    {
        return;
    }

    const Game &game = games[gameIndex];
    std::cout << "Event: " << game.event << std::endl;
    std::cout << "Press enter to continue...";
    std::cin.get();
    std::cout << "Site: " << game.site << std::endl;
    std::cout << "Press enter to continue...";
    std::cin.get();
    std::cout << "Date: " << game.date << std::endl;
    std::cout << "Press enter to continue...";
    std::cin.get();
    std::cout << "Round: " << game.round << std::endl;
    std::cout << "Press enter to continue...";
    std::cin.get();
    std::cout << "White: " << game.white << std::endl;
    std::cout << "Press enter to continue...";
    std::cin.get();
    std::cout << "Black: " << game.black << std::endl;
    std::cout << "Press enter to continue...";
    std::cin.get();
    std::cout << "Result: " << game.result << std::endl;
    std::cout << "Press enter to continue...";
    std::cin.get();
    std::cout << "Moves: " << game.moves << std::endl;
}

// la funzione translateMoves prende in input una stringa di mosse in notazione algebrica standard e restituisce una stringa con le mosse tradotte nella notazione usata dal programma, sostituendo i pezzi con le lettere corrispondenti e gestendo i casi speciali di arrocco.
// Gestisce le catture (Axb4 -> Ab4), le promozioni (e8=Q -> e8D), e le catture di pedone (exf4 -> f4)
// Gestisce le catture (Axb4 -> Ab4), le promozioni (e8=Q -> e8D), e le catture di pedone (exf4 -> f4)
// inoltre le catture del tipo Axb4 diventano Ab4, e le promozioni del tipo e8=Q diventano e8D (rimuovendo l'= e sostituendo il pezzo promosso), mentre una cattura di pedone da casa e3 a casa f4 del tipo exf4 diventa una mossa di spostamento di casa come f4, eliminando la x e la casa di partenza del pedone.
// Gestisce le catture (Axb4 -> Ab4), le promozioni (e8=Q -> De8 sostituendo a Q la lettera in italiano), e le catture di pedone (exf4 -> f4)
// inoltre le catture del tipo Axb4 diventano Ab4, e le promozioni del tipo e8=Q diventano De8, mentre una cattura di pedone da casa e3 a casa f4 del tipo exf diventa una mossa di spostamento di casacome f4, eliminando la x e la casa di partenza del pedone.Infine gli enpassant sono spostamento di casa

const std::string PGNReader::translateMoves(const std::string &moves)
{
    std::string translatedMoves = moves;
    size_t pos = 0;
    int semi_moves = 0;

    // Conta semimosse e sostituisci O-O-O e O-O
    while (pos < translatedMoves.length())
    {
        if (translatedMoves[pos] == ' ' || translatedMoves[pos] == '\n')
        {
            semi_moves++;
        }

        if (translatedMoves.substr(pos, 5) == "O-O-O")
        {
            bool isWhite = (semi_moves % 2) == 0;
            std::string replacement = isWhite ? "Kc1" : "Kc8";
            translatedMoves.replace(pos, 5, replacement);
            pos += replacement.length();
        }
        else if (translatedMoves.substr(pos, 3) == "O-O")
        {
            bool isWhite = (semi_moves % 2) == 0;
            std::string replacement = isWhite ? "Kg1" : "Kg8";
            translatedMoves.replace(pos, 3, replacement);
            pos += replacement.length();
        }
        else
        {
            pos++;
        }
    }

    // Traduci i pezzi
    for (size_t i = 0; i < translatedMoves.length(); ++i)
    {
        char c = translatedMoves[i];
        switch (c)
        {
        case 'K':
            translatedMoves[i] = 'R';
            break;
        case 'Q':
            translatedMoves[i] = 'D';
            break;
        case 'R':
            translatedMoves[i] = 'T';
            break;
        case 'B':
            translatedMoves[i] = 'A';
            break;
        case 'N':
            translatedMoves[i] = 'C';
            break;
        default:
            break;
        }
    }

    return translatedMoves;
}
