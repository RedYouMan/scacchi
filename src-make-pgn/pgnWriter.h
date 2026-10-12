
// pgnWriter.h
// Modulo per la scrittura e gestione dei file PGN.
// Contiene la definizione della struttura dei dati del gioco e i metodi utili
// per leggere una partita in notazione italiana e trasformarla in formato PGN.
#ifndef PGNWRITER_H
#define PGNWRITER_H

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
    std::string whiteElo;
    std::string blackElo;
    std::string eco;
    std::string result;
    std::string moves;
};

class PGNWriter
{
private:
    std::vector<Game> games;

public:
    // Scrive una riga di log in un file esterno.
    void writeTrace(const std::string &nameFile, const std::string &trace);

    // Restituisce la lista dei file presenti in una cartella.
    std::vector<std::string> getOpeningsBooks(const std::string &path);

    // Legge un file di testo contenente una partita e la memorizza in memoria.
    bool readFile(const std::string &filename);

    // Elimina tutte le partite lette.
    void clearGames();

    // Restituisce il numero di partite caricate.
    int getGamesCount() const;

    // Restituisce il puntatore alle mosse della partita richiesta.
    const std::string *getMoves(int gameIndex) const;

    // Restituisce le mosse della partita indicata.
    std::string getInfoMoves(int NumGame);

    // Trasforma la notazione algebrica italiana in notazione PGN standard.
    std::string translateMoves(const std::string &moves);

    // Stampa a video la partita richiesta.
    void displayGame(int gameIndex) const;

    // Costruisce il contenuto completo di una partita in formato PGN.
    std::string buildPGN(const Game &game) const;

    // Crea un file PGN, ma se esiste gia' non lo sovrascrive.
    bool writePGNFile(const std::string &path, const std::string &fileName, const Game &game) const;
};

#endif
