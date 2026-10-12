
// make_pgn.cpp
// Programma che converte una serie di file .txt, ciascuno contenente i dati di
// una partita, in un unico archivio PGN. Per ogni partita vengono richiesti i
// valori delle intestazioni (evento, sito, data, round, giocatori, Elo, ECO,
// risultato) e poi viene scritto il record PGN completo. Le partite elaborate
// vengono eliminate dalla cartella di origine dopo la conversione.
//
// Il programma è pensato per un flusso guidato da prompt interattivo: l'utente
// inserisce la cartella contenente i file .txt, il nome del giocatore e i dati
// della singola partita. Il file di output viene creato con un nome basato sul
// nome del giocatore e i giochi vengono aggiunti in append se il file PGN
// esiste già.

#include "pgnWriter.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

// Normalizza il nome del file di output: mantiene solo caratteri validi per un
// nome PGN, sostituisce gli spazi con trattini bassi e usa un nome di fallback
// se la stringa risultante dovesse essere vuota.
static std::string sanitizePGNName(const std::string &name)
{
    std::string result;
    for (char ch : name)
    {
        if (std::isalnum(static_cast<unsigned char>(ch)) || ch == '_' || ch == '-' || ch == ' ')
        {
            if (ch == ' ')
            {
                result += '_';
            }
            else
            {
                result += ch;
            }
        }
    }

    if (result.empty())
    {
        return "giocatore";
    }

    return result + ".pgn";
}

// Escapa i caratteri speciali all'interno dei valori delle intestazioni PGN.
// In particolare, i doppi apici vengono convertiti in \" e i controlli non
// ammessi vengono rimossi per evitare file PGN invalidi.
static std::string escapePGNText(const std::string &value)
{
    std::string escaped;
    escaped.reserve(value.size());

    for (char ch : value)
    {
        if (ch == '"')
        {
            escaped += "\\\"";
        }
        else if (static_cast<unsigned char>(ch) < 0x20 && ch != '\n' && ch != '\t')
        {
            continue;
        }
        else
        {
            escaped += ch;
        }
    }

    return escaped;
}

// Rimuove i caratteri di controllo non ammessi nelle intestazioni PGN,
// mantenendo però newline e tabulazione qualora presenti.
static std::string normalizePGNHeaderValue(const std::string &value)
{
    std::string normalized;
    normalized.reserve(value.size());
    for (char ch : value)
    {
        if (static_cast<unsigned char>(ch) < 0x20 && ch != '\n' && ch != '\t')
        {
            continue;
        }
        normalized += ch;
    }
    return normalized;
}

// Scrive una singola partita nel formato PGN, includendo i campi di intestazione
// e la sequenza di mosse. Se l'ultima parte delle mosse contiene già un risultato,
// viene rimosso per evitare duplicazioni nello storico PGN.
static void appendPGNGame(std::ofstream &out, const Game &game)
{
    if (!out.good())
    {
        return;
    }

    const std::string event = normalizePGNHeaderValue(game.event);
    const std::string site = normalizePGNHeaderValue(game.site);
    const std::string date = normalizePGNHeaderValue(game.date);
    const std::string round = normalizePGNHeaderValue(game.round);
    const std::string white = normalizePGNHeaderValue(game.white);
    const std::string black = normalizePGNHeaderValue(game.black);
    const std::string whiteElo = normalizePGNHeaderValue(game.whiteElo);
    const std::string blackElo = normalizePGNHeaderValue(game.blackElo);
    const std::string eco = normalizePGNHeaderValue(game.eco);
    const std::string result = normalizePGNHeaderValue(game.result.empty() ? "*" : game.result);

    // ROS: evita di riportare un risultato già presente in coda alle mosse.
    std::string moves = game.moves;
    while (!moves.empty())
    {
        while (!moves.empty() && std::isspace(static_cast<unsigned char>(moves.back())))
        {
            moves.pop_back();
        }
        const std::size_t tokenStart = moves.find_last_of(" \t\r\n");
        const std::size_t start = tokenStart == std::string::npos ? 0 : tokenStart + 1;
        const std::string token = moves.substr(start);
        if (token == "1-0" || token == "0-1" || token == "1/2-1/2" || token == "*")
        {
            moves.erase(start);
        }
        else
        {
            break;
        }
    }

    out << "[Event \"" << escapePGNText(event) << "\"]\n"
        << "[Site \"" << escapePGNText(site) << "\"]\n"
        << "[Date \"" << escapePGNText(date) << "\"]\n"
        << "[Round \"" << escapePGNText(round) << "\"]\n"
        << "[White \"" << escapePGNText(white) << "\"]\n"
        << "[Black \"" << escapePGNText(black) << "\"]\n"
        << "[Result \"" << escapePGNText(result) << "\"]\n"
        << "[WhiteElo \"" << escapePGNText(whiteElo) << "\"]\n"
        << "[BlackElo \"" << escapePGNText(blackElo) << "\"]\n"
        << "[ECO \"" << escapePGNText(eco) << "\"]\n\n"
        << moves << "\n"
        << (game.result.empty() ? "*" : game.result) << "\n";
}

// Elimina i file .txt già processati dalla cartella di lavoro, lasciando solo il
// PGN eventualmente creato dall'utente.
static void deleteRemainingTxtFiles(const fs::path &directory)
{
    std::error_code ec;
    for (const auto &entry : fs::directory_iterator(directory, ec))
    {
        if (ec)
        {
            break;
        }

        if (entry.is_regular_file(ec) && entry.path().extension() == ".txt")
        {
            if (fs::remove(entry.path(), ec))
            {
                std::cout << "File TXT eliminato: " << entry.path().filename() << '\n';
            }
            else if (ec)
            {
                std::cerr << "Impossibile eliminare: " << entry.path() << '\n';
            }
        }
    }
}

int main()
{
    // Banner iniziale e descrizione operativa del programma.
    std::cout << "make-pgn 1.1 (C) 2026 - Rosario Turco\n";
    std::cout << "Questo programma legge i file .txt di una cartella, raccoglie i dati delle partite e genera un file PGN unico. "
                 "Dopo l'elaborazione i file sorgente vengono eliminati dalla cartella di origine.\n";
    std::cout << "Inserire il percorso della cartella contenente i file .txt: ";
    std::string folderPath;
    std::getline(std::cin, folderPath);

    fs::path directory(folderPath);
    if (!fs::exists(directory) || !fs::is_directory(directory))
    {
        std::cerr << "Cartella non valida.\n";
        return 1;
    }

    std::vector<fs::path> txtFiles;
    for (const auto &entry : fs::directory_iterator(directory))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".txt")
        {
            txtFiles.push_back(entry.path());
        }
    }

    std::sort(txtFiles.begin(), txtFiles.end());
    if (txtFiles.empty())
    {
        std::cout << "Nessun file .txt trovato nella cartella.\n";
        return 0;
    }

    std::string playerName;
    std::cout << "Nome del giocatore in formato PGN (es. "
                 "Giorgio Torre"
                 "): ";
    std::getline(std::cin, playerName);
    if (playerName.empty())
    {
        playerName = "Giocatore";
    }

    std::string outputName = sanitizePGNName(playerName);
    fs::path pgnPath = directory / outputName;
    bool pgnExists = fs::exists(pgnPath);

    for (const auto &txtFile : txtFiles)
    {
        std::cout << "File considerato: " << txtFile.filename() << '\n';

        PGNWriter writer;
        if (!writer.readFile(txtFile.string()))
        {
            std::cerr << "Impossibile leggere il file: " << txtFile << '\n';
            continue;
        }

        std::string event = "", site = "", date = "", round = "", white = "", black = "", whiteElo = "", blackElo = "", eco = "", result = "";
        std::cout << "Evento: ";
        std::getline(std::cin, event);
        std::cout << "Sede: ";
        std::getline(std::cin, site);
        std::cout << "Data (YYYY.MM.DD): ";
        std::getline(std::cin, date);
        std::cout << "Round: ";
        std::getline(std::cin, round);
        std::cout << "Nome del Bianco: ";
        std::getline(std::cin, white);
        std::cout << "Nome del Nero: ";
        std::getline(std::cin, black);
        std::cout << "White Elo: ";
        std::getline(std::cin, whiteElo);
        std::cout << "Black Elo: ";
        std::getline(std::cin, blackElo);
        std::cout << "ECO: ";
        std::getline(std::cin, eco);
        std::cout << "Risultato (*, 1-0, 0-1, 1/2-1/2): ";
        std::getline(std::cin, result);

        if (result.empty())
        {
            result = "*";
        }

        Game game;
        game.event = event;
        game.site = site;
        game.date = date;
        game.round = round;
        game.white = white;
        game.black = black;
        game.whiteElo = whiteElo;
        game.blackElo = blackElo;
        game.eco = eco;
        game.result = result;
        game.moves = writer.translateMoves(writer.getInfoMoves(0));
        if (!game.moves.empty() && game.moves.back() != '0' && game.moves.back() != '1' && game.moves.back() != '2' && game.moves.back() != '*' && game.moves.back() != '"')
        {
            const std::string finalResult = game.result == "*" ? "*" : game.result;
            if (finalResult != "*")
            {
                game.moves += " " + finalResult;
            }
        }

        if (pgnExists)
        {
            std::ofstream pgnFile(pgnPath, std::ios::app);
            if (!pgnFile)
            {
                std::cerr << "Impossibile aprire il file PGN in append: " << pgnPath << '\n';
                deleteRemainingTxtFiles(directory); // modificato
                return 1;
            }

            if (fs::file_size(pgnPath) > 0)
            {
                pgnFile << '\n';
            }

            appendPGNGame(pgnFile, game);
        }
        else
        {
            if (!writer.writePGNFile(directory.string(), outputName, game))
            {
                std::cerr << "Impossibile creare il file PGN: " << pgnPath << '\n';
                deleteRemainingTxtFiles(directory); // modificato
                return 1;
            }
            pgnExists = fs::exists(pgnPath);
        }

        // std::cout << "Creato file PGN: " << pgnPath << '\n';
        std::cout << "Partita presa dal file: " << txtFile.filename() << '\n';
    }

    deleteRemainingTxtFiles(directory); // modificato
    return 0;
}
