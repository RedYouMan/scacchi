// pgnWriter.cpp
// Modulo per la lettura dei file di testo, la conversione delle mosse
// e la generazione del testo PGN finale.
// Il codice gestisce la normalizzazione dei contenuti, la creazione della
// struttura dati delle partite e la scrittura del file PGN finale.
#include "pgnWriter.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace
{
    // Percorso del file di testo sorgente che viene trasformato in PGN.
    // Viene usato per eliminare il file TXT originale dopo averlo salvato.
    fs::path pendingTextFile;

    // Controlla se il token corrisponde a un risultato PGN valido.
    bool isPGNResultToken(const std::string &token)
    {
        return token == "1-0" || token == "0-1" || token == "1/2-1/2" || token == "*";
    }

    // Restituisce un risultato PGN se presente all'interno del testo.
    // Usa lo stream per analizzare le parole e riconoscere un token di risultato.
    std::string singlePGNResult(const std::string &result)
    {
        std::istringstream tokens(result);
        std::string token;
        while (tokens >> token)
        {
            if (isPGNResultToken(token))
            {
                return token;
            }
        }
        return "*";
    }

    // Elimina gli spazi finali dell'ultima riga, utile prima di rimuovere
    // eventuali risultati PGN finali in coda al testo di mosse.
    std::string trimTrailingWhitespace(const std::string &text)
    {
        std::string trimmed = text;
        while (!trimmed.empty() && std::isspace(static_cast<unsigned char>(trimmed.back())))
        {
            trimmed.pop_back();
        }
        return trimmed;
    }

    // Rimuove i token di risultato PGN lasciati in fondo alla sequenza di mosse.
    // Ad esempio: "... e4 d5 1-0" diventa "... e4 d5".
    std::string stripTrailingPGNResults(const std::string &moves)
    {
        std::string cleaned = trimTrailingWhitespace(moves);

        while (!cleaned.empty())
        {
            const std::size_t end = cleaned.find_last_not_of(" \t\r\n");
            if (end == std::string::npos)
            {
                return "";
            }

            std::size_t start = cleaned.find_last_of(" \t\r\n", end);
            if (start == std::string::npos)
            {
                start = 0;
            }
            else
            {
                ++start;
            }

            const std::string token = cleaned.substr(start, end - start + 1);
            if (!isPGNResultToken(token))
            {
                return cleaned;
            }

            cleaned.erase(start, end - start + 1);
            cleaned = trimTrailingWhitespace(cleaned);
        }

        return "";
    }
}

// Scrive una riga di log in un file esterno, appendendola alla fine.
// Viene usata per tracciare eventi secondari o messaggi di debug.
void PGNWriter::writeTrace(const std::string &nameFile, const std::string &trace)
{
    std::ofstream out(nameFile, std::ios::app);
    if (!out)
    {
        return;
    }

    out << trace << '\n';
}

// Cerca tutti i file .txt presenti in una directory e li restituisce ordinati.
// Questo permette poi di selezionare i libri di aperture dal filesystem.
std::vector<std::string> PGNWriter::getOpeningsBooks(const std::string &path)
{
    std::vector<std::string> files;
    if (!fs::exists(path) || !fs::is_directory(path))
    {
        return files;
    }

    for (const auto &entry : fs::directory_iterator(path))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".txt")
        {
            files.push_back(entry.path().string());
        }
    }

    std::sort(files.begin(), files.end());
    return files;
}

// Legge un file di testo e lo trasforma in una partita PGN in memoria.
// Vengono puliti caratteri non validi, rimosso il risultato finale e salvato
// il contenuto come sequenza di mosse nella struttura Game.
bool PGNWriter::readFile(const std::string &filename)
{
    pendingTextFile.clear();
    std::ifstream input(filename);
    if (!input)
    {
        return false;
    }

    games.clear();
    Game game;
    game.event = "Partita";
    game.site = "Localita'";
    game.date = "2025.01.01";
    game.round = "1";
    game.white = "Bianco";
    game.black = "Nero";
    game.whiteElo.clear();
    game.blackElo.clear();
    game.eco.clear();
    game.result = "*";

    std::stringstream buffer;
    buffer << input.rdbuf();
    std::string content = buffer.str();

    // Pulisce il testo da caratteri di controllo, CR, EOF e altri byte non
    // validi per un file PGN/ASCII standard.
    std::string clean;
    clean.reserve(content.size());
    for (char c : content)
    {
        if (c == '\r')
        {
            continue;
        }
        if (static_cast<unsigned char>(c) == 0x1A)
        {
            continue;
        }
        if (static_cast<unsigned char>(c) < 0x20 && c != '\n' && c != '\t')
        {
            continue;
        }
        clean += c;
    }

    clean = stripTrailingPGNResults(clean);
    game.moves = clean;
    games.push_back(game);
    pendingTextFile = fs::path(filename);
    return true;
}

// Svuota l'array delle partite caricate in memoria.
void PGNWriter::clearGames()
{
    games.clear();
}

// Numero totale di partite presenti nella struttura interna.
int PGNWriter::getGamesCount() const
{
    return static_cast<int>(games.size());
}

// Restituisce un puntatore alle mosse della partita richiesta.
const std::string *PGNWriter::getMoves(int gameIndex) const
{
    if (gameIndex < 0 || gameIndex >= static_cast<int>(games.size()))
    {
        return nullptr;
    }

    return &games[gameIndex].moves;
}

// Variante semplice per ottenere le mosse di una partita dato l'indice.
std::string PGNWriter::getInfoMoves(int NumGame)
{
    if (NumGame < 0 || NumGame >= getGamesCount())
    {
        return "";
    }

    return games[NumGame].moves;
}

// Normalizza la stringa delle mosse, eliminando spazi multipli e lasciando
// uno spazio tra token consecutivi. Inoltre, mappa le iniziali dei pezzi
// secondo una convenzione locale (es. A->B, C->N, ...), usata nel processo
// di conversione da testo grezzo a formato PGN.
std::string PGNWriter::translateMoves(const std::string &moves)
{
    std::string normalized;
    normalized.reserve(moves.size());

    bool previousSpace = true;
    for (char ch : moves)
    {
        if (std::isspace(static_cast<unsigned char>(ch)))
        {
            if (!previousSpace)
            {
                normalized += ' ';
                previousSpace = true;
            }
        }
        else
        {
            normalized += ch;
            previousSpace = false;
        }
    }

    if (!normalized.empty() && normalized.back() == ' ')
    {
        normalized.pop_back();
    }

    // Traduci solo la lettera iniziale del pezzo; le case (a-h) restano minuscole.
    for (std::size_t start = 0; start < normalized.size();)
    {
        std::size_t end = normalized.find(' ', start);
        if (end == std::string::npos)
        {
            end = normalized.size();
        }

        std::size_t piece = start;
        while (piece < end && std::isdigit(static_cast<unsigned char>(normalized[piece])))
        {
            ++piece;
        }
        while (piece < end && normalized[piece] == '.')
        {
            ++piece;
        }

        if (piece < end)
        {
            switch (normalized[piece])
            {
            case 'A':
                normalized[piece] = 'B';
                break;
            case 'C':
                normalized[piece] = 'N';
                break;
            case 'D':
                normalized[piece] = 'Q';
                break;
            case 'R':
                normalized[piece] = 'K';
                break;
            case 'T':
                normalized[piece] = 'R';
                break;
            default:
                break;
            }
        }

        start = end + 1;
    }

    return normalized;
}

// Stampa a console i dettagli principali di una partita caricata in memoria.
void PGNWriter::displayGame(int gameIndex) const
{
    if (gameIndex < 0 || gameIndex >= static_cast<int>(games.size()))
    {
        std::cout << "Indice della partita non valido.\n";
        return;
    }

    const Game &game = games[gameIndex];
    std::cout << "Evento: " << game.event << '\n';
    std::cout << "Sito: " << game.site << '\n';
    std::cout << "Data: " << game.date << '\n';
    std::cout << "Round: " << game.round << '\n';
    std::cout << "Bianco: " << game.white << '\n';
    std::cout << "Nero: " << game.black << '\n';
    std::cout << "Risultato: " << game.result << '\n';
    std::cout << "Mosse: " << game.moves << '\n';
}

// Genera la stringa PGN completa a partire da un oggetto Game.
// Vengono scritte le tag [Event], [Site], [Date], [Round], [White], [Black],
// [Result], [WhiteElo], [BlackElo], [ECO], seguite dalla riga di mosse.
std::string PGNWriter::buildPGN(const Game &game) const
{
    std::ostringstream out;
    const std::string event = game.event.empty() ? "" : game.event;
    const std::string site = game.site.empty() ? "" : game.site;
    const std::string date = game.date.empty() ? "" : game.date;
    const std::string round = game.round.empty() ? "" : game.round;
    const std::string white = game.white.empty() ? "" : game.white;
    const std::string black = game.black.empty() ? "" : game.black;
    const std::string whiteElo = game.whiteElo.empty() ? "" : game.whiteElo;
    const std::string blackElo = game.blackElo.empty() ? "" : game.blackElo;
    const std::string eco = game.eco.empty() ? "" : game.eco;
    const std::string result = singlePGNResult(game.result);
    std::string moves = stripTrailingPGNResults(game.moves);
    while (!moves.empty() && (moves.back() == '\n' || moves.back() == '\r'))
    {
        moves.pop_back();
    }
    moves = trimTrailingWhitespace(moves);

    out << "[Event \"" << event << "\"]\n";
    out << "[Site \"" << site << "\"]\n";
    out << "[Date \"" << date << "\"]\n";
    out << "[Round \"" << round << "\"]\n";
    out << "[White \"" << white << "\"]\n";
    out << "[Black \"" << black << "\"]\n";
    out << "[Result \"" << result << "\"]\n";
    out << "[WhiteElo \"" << whiteElo << "\"]\n";
    out << "[BlackElo \"" << blackElo << "\"]\n";
    out << "[ECO \"" << eco << "\"]\n\n";

    if (!moves.empty())
    {
        out << moves << "  " << result << '\n';
    }
    else
    {
        out << result << '\n';
    }
    return out.str();
}

// Salva una partita in un file PGN, eventualmente chiedendo conferma se il file
// esiste già. Dopo il salvataggio elimina il file TXT sorgente per evitare
// duplicati o file residui.
bool PGNWriter::writePGNFile(const std::string &path, const std::string &fileName, const Game &game) const
{
    fs::path directory(path);
    fs::path fullPath = directory / fileName;

    const bool fileExists = fs::exists(fullPath);
    if (fileExists)
    {
        std::cout << "Il file PGN esiste gia': " << fullPath.string()
                  << ". Vuoi proseguire? (s/n): ";
        std::string answer;
        if (!(std::cin >> answer))
        {
            return false;
        }
        std::transform(answer.begin(), answer.end(), answer.begin(),
                       [](unsigned char c)
                       { return static_cast<char>(std::tolower(c)); });
        if (answer != "s" && answer != "si" && answer != "y" && answer != "yes")
        {
            return false;
        }
    }

    std::ofstream output(fullPath, std::ios::out | std::ios::app);
    if (!output)
    {
        return false;
    }

    if (fileExists && fs::is_regular_file(fullPath) && fs::file_size(fullPath) > 0)
    {
        output << '\n';
    }

    output << buildPGN(game);
    output.close();
    if (!output)
    {
        return false;
    }

    std::cout << "Predisposto file: " << fullPath.string() << '\n';

    // Rimuove soltanto il TXT appena elaborato, dopo averlo salvato nel PGN.
    if (!pendingTextFile.empty() && pendingTextFile.extension() == ".txt")
    {
        std::error_code error;
        const bool removed = fs::remove(pendingTextFile, error);
        if (removed || (!error && !fs::exists(pendingTextFile)))
        {
            pendingTextFile.clear();
        }
        else
        {
            std::cerr << "Impossibile cancellare il file TXT: "
                      << pendingTextFile.string() << '\n';
        }
    }

    return true;
}
