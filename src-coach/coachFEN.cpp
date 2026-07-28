// coach.cpp
/*
Il programma riceve in input una FEN valida e un intero che rappresenta la profondità di analisi (compresa tra 15 e 20).
Se la profondità non è indicata viene assunto 15.
Se la fen e la profondità sono valide,
- starta stockfish utilizzando le funzionalità di engine4.cpp
-invia a stockfish la FEN
- ottiene da stockfish la best move e con sintesi vocale dice la miglior mossa
- chiude la console di stockfish

*/

// coach.cpp
#include "proto.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "engine.h"
#include "myad.h"

static std::vector<std::string> splitString(const std::string &text, char sep)
{
    std::vector<std::string> result;
    std::stringstream ss(text);
    std::string item;
    while (std::getline(ss, item, sep))
    {
        result.push_back(item);
    }
    return result;
}

static bool validateFenPiecePlacement(const std::string &placement)
{
    std::vector<std::string> ranks = splitString(placement, '/');
    if (ranks.size() != 8)
        return false;

    for (const std::string &rank : ranks)
    {
        if (rank.empty())
            return false;

        int squares = 0;
        for (char c : rank)
        {
            if (c >= '1' && c <= '8')
            {
                squares += c - '0';
            }
            else
            {
                char piece = std::tolower(static_cast<unsigned char>(c));
                if (piece != 'p' && piece != 'n' && piece != 'b' && piece != 'r' && piece != 'q' && piece != 'k')
                    return false;
                squares += 1;
            }
            if (squares > 8)
                return false;
        }
        if (squares != 8)
            return false;
    }
    return true;
}

static bool parseInteger(const std::string &text, int &value)
{
    try
    {
        size_t pos = 0;
        value = std::stoi(text, &pos);
        return pos == text.size();
    }
    catch (...)
    {
        return false;
    }
}

static bool isValidFen(const std::string &fen)
{
    std::vector<std::string> fields = splitString(fen, ' ');
    if (fields.size() != 6)
        return false;

    if (!validateFenPiecePlacement(fields[0]))
        return false;

    if (fields[1] != "w" && fields[1] != "b")
        return false;

    const std::string &castling = fields[2];
    if (castling != "-")
    {
        bool seenK = false, seenQ = false, seenk = false, seenq = false;
        for (char c : castling)
        {
            switch (c)
            {
            case 'K':
                if (seenK)
                    return false;
                seenK = true;
                break;
            case 'Q':
                if (seenQ)
                    return false;
                seenQ = true;
                break;
            case 'k':
                if (seenk)
                    return false;
                seenk = true;
                break;
            case 'q':
                if (seenq)
                    return false;
                seenq = true;
                break;
            default:
                return false;
            }
        }
    }

    const std::string &enPassant = fields[3];
    if (enPassant != "-")
    {
        if (enPassant.size() != 2)
            return false;
        if (enPassant[0] < 'a' || enPassant[0] > 'h')
            return false;
        if (enPassant[1] != '3' && enPassant[1] != '6')
            return false;
    }

    int halfmove = 0;
    if (!parseInteger(fields[4], halfmove) || halfmove < 0)
        return false;

    int fullmove = 0;
    if (!parseInteger(fields[5], fullmove) || fullmove < 1)
        return false;

    return true;
}

static bool parseDepth(const std::string &text, int &depth)
{
    if (!parseInteger(text, depth))
        return false;
    return depth >= 15 && depth <= 20;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Uso: coachFEN \"<FEN con doppi apici>\"" << std::endl;
        return 1;
    }

    // banner di inizio
    std::cout << "CoachFEN (C) 2026 - Rosario Turco" << std::endl;
    std::string fen = argv[1];
    int depth = 15;
    // depth is fixed to 15

    if (!isValidFen(fen))
    {
        std::cerr << "FEN non valida." << std::endl;
        return 1;
    }

    if (!start())
    {
        std::cerr << "Impossibile avviare il motore di scacchi." << std::endl;
        return 1;
    }

    sendCommand("position fen " + fen + "\n");
    sendCommand("go depth " + std::to_string(depth) + "\n");

    std::string bestMove = getOutputMove();
    if (bestMove.empty() || bestMove == "1")
    {
        std::cerr << "Errore nell'analisi del motore." << std::endl;
        callTextToSpeech("Errore nell analisi del motore");
        stop();
        return 1;
    }

    std::string message = "La migliore mossa e' " + bestMove;
    // std::cout << message << std::endl;
    callTextToSpeech(message);
    message = "Attendi valutazione ...";
    callTextToSpeech(message);

    evalPosition();
    stop();
    return 0;
}



