// simpleEditor.cpp
/*
Autore: Rosario Turco
Descrizione
 Questo editor testuale aiuta a scrivere rapidamente file con specifica formale ROTN ed è adatto anche a istruttori di scacchi non vedenti.
 Permette di creare un database testuale di posizioni.
 Si consiglia che il path del database
 sia al di fuori della cartella di Scacchi-it.
Questo per non appesantire Scacchi-it e anche per quando si reinstalla una nuova versione si è già a posto.
 Il programma in partenza legge il path dal file di configurazione ste.cnf
 In ciclo chiede:
 1. la cartella di classificazione che aggiunge al path letto dal file di configurazione
 2. se la cartella di classificazione non esiste inpath+cartella di classificazione la crea
 3. il nome del file da creare in path+cartella di classificazione
 4. Il tema della posizione
 5. Da che parte è ruotata la scacchiera (B o  N)
 6. apre il file in scrittura
 7. scrive T: + il tema e chiude con ';' e va a capo
8.scrive V: col carattere ricevuto e chiude con ';' e va a capo
9 chiede il numero di pezzi bianchi
10. prepara la stringa con B: iniziale
 11 in ciclo per ogni pezzo bianco chiede il tipo (R, D, T, A, C, P)
 12 chiede la casa dove mettere il pezzo
 13 aggiunge alla stringa che inizia con b:tipo e casa seguito da';'
14. quando è terminato il numero di pezzi scrive la stringa creata nel file e fa return
15. si ripete tutti i punti da 9 a 14 per i pezzi neri
16 chiede chi ha il tratto (B o N) e scrive M:+il valore seguito da ';' e return
17. chiede l'ultimo numero di mossa giocato dal bianco e scrive P: seguito dal numero e da ';'
18. Chiede se occorre mettere l'enpassant con U: se SI (anche S o s o si )chiede le case dei pedoni che possono catturare il pedone, seguito dalla casa del pedone catturabile al massimo possono essere 6 caratteri e minimo quattro caratteri di case.In particolare se sono 6 caratteri si deve controllare che l'ultima casa sia compresa tra le prime due Va controllato che sono case della scacchiera
19 Chiede il numero di mosse per le 50 mosse: un intero tra 0 in suda mettere come z:seguito dal valore. Se 0 la Z: non si mette.
20chiude il file dando il messaggio di aver creat il file.
*/

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <cctype>

namespace fs = std::filesystem;

static std::string readLine(const std::string &prompt)
{
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

static int readNumber(const std::string &prompt)
{
    for (;;)
    {
        std::string value = readLine(prompt);
        try
        {
            std::size_t used = 0;
            int number = std::stoi(value, &used);
            if (used == value.size() && number >= 0)
                return number;
        }
        catch (...)
        {
        }
        std::cout << "Inserire un numero valido.\n";
    }
}

static char upperInitial(const std::string &value)
{
    return value.empty() ? '\0' : static_cast<char>(std::toupper(static_cast<unsigned char>(value.front())));
}

static std::string upperAnswer(std::string value)
{
    for (char &character : value)
        character = static_cast<char>(std::toupper(static_cast<unsigned char>(character)));
    return value;
}

static std::string lowerInitial(std::string value)
{
    if (!value.empty())
        value.front() = static_cast<char>(
            std::tolower(static_cast<unsigned char>(value.front())));
    return value;
}

static std::string readSquare()
{
    for (;;)
    {
        std::string square = readLine("Casa: ");
        if (square.size() == 2)
        {
            const char file = static_cast<char>(
                std::tolower(static_cast<unsigned char>(square[0])));
            if (file >= 'a' && file <= 'h' && square[1] >= '1' && square[1] <= '8')
            {
                square[0] = file;
                return square;
            }
        }
        std::cout << "Casa non valida. Inserire una casa da a1 a h8.\n";
    }
}

static void readEnPassant(std::ofstream &output)
{
    for (;;)
    {
        const std::string answer = upperAnswer(readLine(
            "Inserire l'en passant (si/no): "));
        if (answer == "NO" || answer == "N")
            return;
        if (answer == "SI" || answer == "S")
            break;
        std::cout << "Rispondere SI oppure NO.\n";
    }

    int pawnCount;
    for (;;)
    {
        pawnCount = readNumber("Numero di pedoni che possono catturare (1/2): ");
        if (pawnCount == 1 || pawnCount == 2)
            break;
        std::cout << "Inserire 1 oppure 2.\n";
    }

    std::string value = "U:";
    if (pawnCount == 1)
    {
        for (;;)
        {
            const std::string pawn = readSquare();
            const std::string captured = readSquare();

            // Le due case devono essere contigue sulla stessa traversa.
            // La casa catturabile puo' precedere o seguire quella del pedone.
            if (pawn[1] == captured[1] &&
                std::abs(pawn[0] - captured[0]) == 1)
            {
                value += pawn + captured;
                break;
            }
            std::cout << "Le due case devono essere contigue, senza colonne "
                         "di separazione, sulla stessa traversa. Riprovare.\n";
        }
    }
    else
    {
        for (;;)
        {
            const std::string first = readSquare();
            const std::string second = readSquare();
            const std::string captured = readSquare();

            // Con due pedoni, la casa catturabile deve essere esattamente
            // a meta tra le prime due: stessa traversa e file adiacenti.
            const bool centered =
                first[1] == second[1] && second[1] == captured[1] &&
                std::abs(first[0] - second[0]) == 2 &&
                captured[0] == static_cast<char>((first[0] + second[0]) / 2);
            if (centered)
            {
                value += first + second + captured;
                break;
            }
            std::cout << "La terza casa deve stare esattamente tra le prime due "
                         "case, senza colonne di distanza. Riprovare.\n";
        }
    }
    output << value << ";\n";
}

static void readPieces(std::ofstream &output, char colour)
{
    int count;
    const std::string prompt = std::string("Numero di pezzi ") +
                               (colour == 'B' ? "bianchi: " : "neri: ");
    for (;;)
    {
        count = readNumber(prompt);
        if (count >= 1 && count <= 16)
            break;
        std::cout << "Il numero di pezzi deve essere compreso tra 1 e 16.\n";
    }

    output << colour << ':';
    for (int i = 0; i < count; ++i)
    {
        std::string type;
        for (;;)
        {
            type = readLine("Tipo del pezzo (R,D,T,A,C,P): ");
            const char pieceType = upperInitial(type);
            if (type.size() == 1 &&
                (pieceType == 'R' || pieceType == 'D' || pieceType == 'T' ||
                 pieceType == 'A' || pieceType == 'C' || pieceType == 'P'))
            {
                type.assign(1, pieceType);
                break;
            }
            std::cout << "Tipo di pezzo non valido. Usare R, D, T, A, C oppure P.\n";
        }
        std::string square = readSquare();
        output << upperInitial(type) << lowerInitial(square) << ';';
    }
    for (const char *castle : {"NS", "NL"})
    {
        bool present = false;
        for (;;)
        {
            const std::string answer = upperAnswer(readLine(
                std::string("E' presente arrocco ambiguo ") +
                (castle[1] == 'S' ? "corto" : "lungo") + " (si/no): "));
            if (answer == "SI" || answer == "S")
            {
                present = true;
                break;
            }
            if (answer == "NO" || answer == "N")
                break;
            std::cout << "Rispondere SI oppure NO.\n";
        }
        if (present)
            output << castle << ';';
    }
    output << '\n';
}

static void showLatestFile(const fs::path &directory)
{
    fs::path latestFile;
    fs::file_time_type latestTime;
    bool found = false;

    for (const fs::directory_entry &entry : fs::directory_iterator(
             directory, fs::directory_options::skip_permission_denied))
    {
        std::error_code error;
        if (!entry.is_regular_file(error) || error)
            continue;

        const fs::file_time_type modified = entry.last_write_time(error);
        if (error)
            continue;

        if (!found || modified > latestTime)
        {
            latestFile = entry.path();
            latestTime = modified;
            found = true;
        }
    }

    if (found)
        std::cout << "Ultimo file creato: " << latestFile.filename().string() << '\n';
    else
        std::cout << "Nessun file presente nella cartella di classificazione.\n";
}

int main()
{

    // Banner
    std::cout << "simpleEditor ROTN (C) 2026 - versione 1.3 - Rosario Turco\n";
    try
    {
        std::ifstream config("ste.cnf");
        if (!config)
        {
            std::cerr << "Impossibile aprire ste.cnf.\n";
            return 1;
        }

        std::string databasePath;
        std::getline(config, databasePath);
        while (!databasePath.empty() &&
               (databasePath.back() == '\r' || databasePath.back() == '\n'))
            databasePath.pop_back();
        if (databasePath.empty())
        {
            std::cerr << "Il percorso nel file ste.cnf e' vuoto.\n";
            return 1;
        }

        const fs::path configuredPath(databasePath);
        if (!fs::exists(configuredPath))
        {
            std::cerr << "Il percorso configurato in ste.cnf non esiste. "
                         "Configurare correttamente il file ste.cnf e riprovare.\n";
            return 1;
        }

        const fs::path classification =
            configuredPath / readLine("Cartella di classificazione: ");
        if (fs::exists(classification))
        {
            if (!fs::is_directory(classification))
            {
                std::cerr << "Il percorso di classificazione non e' una directory.\n";
                return 1;
            }
            showLatestFile(classification);
        }
        else
        {
            fs::create_directories(classification);
        }
        fs::path file;
        for (;;)
        {
            const std::string fileName = readLine("Nome del file con estensione txt: ");
            file = classification / fileName;

            if (file.extension() != ".txt")
            {
                std::cerr << "Errore: il file deve avere estensione .txt.\n";
                continue;
            }
            if (fs::exists(file))
            {
                std::cerr << "Errore: il file esiste '. Inserire un altro nome.\n";
                continue;
            }
            break;
        }

        std::ofstream output(file);
        if (!output)
        {
            std::cerr << "Impossibile creare il file.\n";
            return 1;
        }

        output << "T:" << readLine("Tema: ") << ";\n";
        output << "V:" << upperInitial(readLine("Rotazione della scacchiera (B/N): ")) << ";\n";
        readPieces(output, 'B');
        readPieces(output, 'N');
        output << "M:" << upperInitial(readLine("Chi ha il tratto (B/N): ")) << ";\n";
        output << "P:" << readNumber("Ultimo numero di mossa giocato dal bianco: ") << ";\n";
        readEnPassant(output);
        const int fiftyMoveCount = readNumber(
            "Numero di mosse per la regola delle 50 mosse (0 per omettere Z): ");
        if (fiftyMoveCount > 0)
            output << "Z:" << fiftyMoveCount << ";\n";
        std::cout << "Creato il file: " << file.string() << '\n';
    }
    catch (const std::exception &error)
    {
        std::cerr << "Errore: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
