
// coachGame.cpp
/*
 il progetto coachGame serve ad analizzare una partita di scacchi e generare un report con commenti e best move suggerite da Stockfish.
 In input vengono forniti: il nome del file da esaminare e il colore del giocatore da analizzare.
 L'analisi viene condotta su una scacchiera virtuale interna, tenendo conto delle posizioni generate da ogni giocatore con la sua mossa tratta dal file registrato e tradotte in FEN.
 Ad ogni mossa viene presa la valutazione della posizione suggerita da stockfish.
 Si considerano i valori delta delle valutazioni prima e dopo la mossa.
Se tale delta supera in valore assoluto
 le soglie fissate(0.5, 1.0, 1.5), si genera un commento e la best move consigliata al posto di quella fatta.
 Se la partita prosegue però occorre andare oltre la best move e rimettersi sul flusso originario.
 In altri termini deve essere analizzata tutta la partita.

*/
/*


 Autore : Rosario Turco
*/
#ifndef _HAS_STD_BYTE
#define _HAS_STD_BYTE 0
#endif

#pragma warning(disable : 4244)

#include <cstddef>
#define NOMINMAX
#include <windows.h>
#undef max
#undef min
#include "proto.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "engine.h"
#include "myad.h"
#include "mossa_analizzata.h"
#include <vector>

namespace
{
    struct FenState
    {
        char board[8][8];
        char sideToMove;
        std::string castling;
        std::string enPassant;
        int halfmove;
        int fullmove;
    };

    std::string normalizeMove(const std::string &move)
    {
        std::string cleaned = move;
        cleaned.erase(std::remove_if(cleaned.begin(), cleaned.end(), [](char c)
                                     {
                                         unsigned char uc = static_cast<unsigned char>(c);
                                         return c == '+' || c == '#' || c == '!' || c == '?' || std::iscntrl(uc); }),
                      cleaned.end());
        return cleaned;
    }

    bool isWhitePiece(char ch)
    {
        return std::isupper(static_cast<unsigned char>(ch));
    }

    bool isBlackPiece(char ch)
    {
        return std::islower(static_cast<unsigned char>(ch));
    }

    bool sameColor(char a, char b)
    {
        return (isWhitePiece(a) && isWhitePiece(b)) || (isBlackPiece(a) && isBlackPiece(b));
    }

    bool isInside(int row, int col)
    {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }

    std::string indexToSquare(int row, int col)
    {
        std::string sq;
        sq.push_back(static_cast<char>('a' + col));
        sq.push_back(static_cast<char>('1' + (7 - row)));
        return sq;
    }

    bool squareToIndex(const std::string &sq, int &row, int &col)
    {
        if (sq.size() != 2)
            return false;
        col = static_cast<int>(sq[0] - 'a');
        row = 7 - (sq[1] - '1');
        return isInside(row, col);
    }

    FenState parseFEN(const std::string &fen)
    {
        FenState state{};
        for (int r = 0; r < 8; ++r)
            for (int c = 0; c < 8; ++c)
                state.board[r][c] = '.';

        std::istringstream iss(fen);
        std::string placement;
        std::string side;
        std::string castling;
        std::string enPassant;
        std::string halfmove;
        std::string fullmove;
        iss >> placement >> side >> castling >> enPassant >> halfmove >> fullmove;

        int row = 0;
        int col = 0;
        for (char ch : placement)
        {
            if (ch == '/')
            {
                if (row >= 7 || col != 8)
                    break;
                ++row;
                col = 0;
            }
            else if (std::isdigit(static_cast<unsigned char>(ch)))
            {
                const int empty = ch - '0';
                if (empty < 1 || col + empty > 8)
                    break;
                col += empty;
            }
            else
            {
                // Evita una scrittura oltre board[8][8] se una FEN generata
                // da una notazione non valida e' incompleta o malformata.
                if (row >= 0 && row < 8 && col >= 0 && col < 8)
                    state.board[row][col++] = ch;
                else
                    break;
            }
        }

        state.sideToMove = side.empty() ? 'w' : side[0];
        state.castling = castling.empty() ? "-" : castling;
        state.enPassant = enPassant.empty() ? "-" : enPassant;
        // Una FEN incompleta non deve provocare invalid_argument/out_of_range
        // (il problema si manifestava durante l'analisi delle mosse nere).
        try
        {
            state.halfmove = halfmove.empty() ? 0 : std::stoi(halfmove);
        }
        catch (...)
        {
            state.halfmove = 0;
        }
        try
        {
            state.fullmove = fullmove.empty() ? 1 : std::stoi(fullmove);
        }
        catch (...)
        {
            state.fullmove = 1;
        }
        if (state.halfmove < 0)
            state.halfmove = 0;
        if (state.fullmove < 1)
            state.fullmove = 1;
        return state;
    }

    std::string buildFEN(const FenState &state)
    {
        std::ostringstream oss;
        for (int r = 0; r < 8; ++r)
        {
            int empty = 0;
            for (int c = 0; c < 8; ++c)
            {
                char ch = state.board[r][c];
                if (ch == '.')
                {
                    ++empty;
                }
                else
                {
                    if (empty > 0)
                        oss << empty;
                    empty = 0;
                    oss << ch;
                }
            }
            if (empty > 0)
                oss << empty;
            if (r < 7)
                oss << '/';
        }

        oss << ' ' << state.sideToMove << ' ' << state.castling << ' ' << state.enPassant << ' '
            << state.halfmove << ' ' << state.fullmove;
        return oss.str();
    }

    void removeCastlingRight(std::string &rights, char right)
    {
        rights.erase(std::remove(rights.begin(), rights.end(), right), rights.end());
        if (rights.empty())
            rights = "-";
    }

    bool isPathClear(const FenState &state, int fromRow, int fromCol, int toRow, int toCol)
    {
        int dr = (toRow > fromRow) ? 1 : ((toRow < fromRow) ? -1 : 0);
        int dc = (toCol > fromCol) ? 1 : ((toCol < fromCol) ? -1 : 0);
        int r = fromRow + dr;
        int c = fromCol + dc;
        while (r != toRow || c != toCol)
        {
            if (state.board[r][c] != '.')
                return false;
            r += dr;
            c += dc;
        }
        return true;
    }

    bool isPawnMovePossible(const FenState &state, int fromRow, int fromCol, int toRow, int toCol, bool whiteMove,
                            bool isCapture, bool allowEnPassant)
    {
        int dir = whiteMove ? -1 : 1;
        if (fromCol == toCol)
        {
            if (isCapture)
                return false;
            if (state.board[toRow][toCol] != '.')
                return false;
            if (toRow == fromRow + dir)
                return true;
            if ((whiteMove && fromRow == 6 && fromRow + 2 * dir == toRow) || (!whiteMove && fromRow == 1 && fromRow + 2 * dir == toRow))
            {
                if (std::abs(fromRow + dir) < 0 || std::abs(fromRow + dir) >= 8)
                    return false;
                return state.board[fromRow + dir][fromCol] == '.' && state.board[fromRow + 2 * dir][fromCol] == '.';
            }
            return false;
        }

        if (std::abs(toRow - fromRow) != 1 || std::abs(toCol - fromCol) != 1)
            return false;
        if (state.board[toRow][toCol] != '.')
            return true;
        if (allowEnPassant && state.enPassant != "-" && state.enPassant == indexToSquare(toRow, toCol))
            return true;
        return false;
    }

    bool isPieceMovePossible(const FenState &state, int fromRow, int fromCol, int toRow, int toCol, char pieceType)
    {
        char piece = state.board[fromRow][fromCol];
        char target = state.board[toRow][toCol];
        if (target != '.' && sameColor(piece, target))
            return false;

        int dr = toRow - fromRow;
        int dc = toCol - fromCol;
        switch (pieceType)
        {
        case 'N':
            return (std::abs(dr) == 2 && std::abs(dc) == 1) || (std::abs(dr) == 1 && std::abs(dc) == 2);
        case 'B':
            return std::abs(dr) == std::abs(dc) && isPathClear(state, fromRow, fromCol, toRow, toCol);
        case 'R':
            return (dr == 0 || dc == 0) && isPathClear(state, fromRow, fromCol, toRow, toCol);
        case 'Q':
            return ((dr == 0 || dc == 0) || std::abs(dr) == std::abs(dc)) && isPathClear(state, fromRow, fromCol, toRow, toCol);
        case 'K':
            return std::max(std::abs(dr), std::abs(dc)) == 1;
        default:
            return false;
        }
    }

    bool findSourceSquare(const FenState &state, int toRow, int toCol, char pieceType, bool whiteMove,
                          bool isCapture, const std::string &prefix, int &fromRow, int &fromCol)
    {
        char pieceChar = whiteMove ? static_cast<char>(std::toupper(static_cast<unsigned char>(pieceType)))
                                   : static_cast<char>(std::tolower(static_cast<unsigned char>(pieceType)));
        std::vector<std::pair<int, int>> candidates;

        for (int r = 0; r < 8; ++r)
        {
            for (int c = 0; c < 8; ++c)
            {
                if (state.board[r][c] != pieceChar)
                    continue;

                bool matches = false;
                if (pieceType == 'P')
                {
                    matches = isPawnMovePossible(state, r, c, toRow, toCol, whiteMove, isCapture, true);
                }
                else
                {
                    matches = isPieceMovePossible(state, r, c, toRow, toCol, pieceType);
                }

                if (!matches)
                    continue;

                if (!prefix.empty())
                {
                    if (pieceType == 'P')
                    {
                        if (prefix.size() == 1 && std::isalpha(static_cast<unsigned char>(prefix[0])) && c != static_cast<int>(prefix[0] - 'a'))
                            continue;
                    }
                    else if (prefix.size() == 1)
                    {
                        if (std::isalpha(static_cast<unsigned char>(prefix[0])) && c != static_cast<int>(prefix[0] - 'a'))
                            continue;
                        if (std::isdigit(static_cast<unsigned char>(prefix[0])) && r != 7 - (prefix[0] - '1'))
                            continue;
                    }
                }

                candidates.push_back({r, c});
            }
        }

        if (candidates.empty())
            return false;

        fromRow = candidates[0].first;
        fromCol = candidates[0].second;
        return true;
    }
} // namespace

std::string formatStockfishEval(double eval)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << std::showpos << eval;
    return oss.str();
}

bool parsePieceLetter(char ch, char &pieceType)
{
    switch (std::toupper(static_cast<unsigned char>(ch)))
    {
    case 'D':
        pieceType = 'Q';
        return true;
    case 'T':
        pieceType = 'R';
        return true;
    case 'C':
        pieceType = 'N';
        return true;
    case 'A':
        pieceType = 'B';
        return true;
    case 'R':
        pieceType = 'K';
        return true;
    case 'Q':
    case 'N':
    case 'B':
    case 'K':
        pieceType = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
        return true;
    default:
        return false;
    }
}

// prototipi locali
std::string applyMoveToFEN(const std::string &baseFen, const std::string &moveInput, char colore);
std::string whatIsNewFEN(const std::string &moveInput, char colore);

// FEN globale da inizio gioco
std::string FENStart = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
static std::string FENCurrent;

std::string askBestMoveFromPosition(const std::string &fen, char coloreMossa)
{
    // evalStock() lascia il motore in modalita' di ricerca.  In precedenza
    // qui si inviava "stop" seguito da "isready" prima di ogni ricerca; se
    // il lettore dell'output aveva gia' consumato readyok, il ciclo poteva
    // restare bloccato indefinitamente (in particolare durante l'analisi del
    // nero, quando la ricerca della best move avviene a meta' partita).
    // La best move va cercata dal lato che deve effettuare la mossa, non dal
    // colore del giocatore analizzato. La FEN passata deve quindi contenere
    // il lato corretto (w per il bianco, b per il nero).
    // ``b`` means white (bianco), while ``n`` means black (nero).  Do not
    // merely warn when a temporary FEN has the other side: Stockfish would
    // then analyse the opponent's move and the following read could wait for
    // a bestmove that does not belong to the requested position.  Normalize
    // the side field before sending the position to the engine.
    const char expectedSide = (coloreMossa == 'b') ? 'w' : 'b';
    FenState bestMoveState = parseFEN(fen);
    if (bestMoveState.sideToMove != expectedSide)
    {
        bestMoveState.sideToMove = expectedSide;
        bestMoveState.fullmove = std::max(1, bestMoveState.fullmove);
    }
    const std::string positionFen = buildFEN(bestMoveState);

    sendCommand("stop\n");
    Sleep(100);
    sendCommand("position fen " + positionFen + "\n");
    sendCommand("go depth 15\n");

    std::string bestMove;
    for (int attempt = 0; attempt < 100; ++attempt)
    {
        bestMove = getOutputMove();
        if (!bestMove.empty() && bestMove != "1")
        {
            // Quando getOutputMove() riceve "bestmove", Stockfish ha gia'
            // terminato la ricerca.  Non inviare un ulteriore "stop": puo'
            // restare nella coda UCI e interferire con il successivo
            // position/go, soprattutto quando la posizione e' quella del
            // nero a meta' partita.
            return bestMove;
        }

        Sleep(150);
    }

    // Qui la ricerca non ha prodotto una risposta valida: interrompila prima
    // di restituire il controllo al ciclo principale.
    sendCommand("stop\n");
    Sleep(200);

    return bestMove;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Uso: coachGame\"<nome file partita con estensione> <colore (es. b o n)>" << std::endl;
        return 1;
    }

    // banner di inizio
    std::cout << "CoachGame (C) 2026 versione 1.0 - Rosario Turco" << std::endl;
    Sleep(6000);

    callTextToSpeech(string("Benvenuto in CoachGame, il tuo assistente per l'analisi delle partite di scacchi. Attendi che venga completata l'analisi nel file report.txt\n"));
    std::string file_game = "..//registrazioni//" + std::string(argv[1]);
    char colore = tolower(argv[2][0]);
    if (colore != 'b' && colore != 'n')
    {
        std::cout << "Colore non valido. Usa 'b' per bianco o 'n' per nero." << std::endl;
        return 1;
    }

    FENCurrent = FENStart;

    std::ifstream myfile(file_game);
    if (!myfile.is_open())
    {
        std::cout << "Errore: impossibile aprire il file " << file_game << std::endl;
        return 1;
    }

    std::string line;
    std::getline(myfile, line);
    myfile.close();

    std::vector<std::string> whiteMoves;
    std::vector<std::string> blackMoves;
    std::istringstream iss(line);
    std::string token;
    bool expectBlackMove = false;

    while (iss >> token)
    {
        // Gestisce sia "3.e4 e5" sia il formato PGN con il numero separato:
        // "3. e4 e5". La vecchia versione perdeva la mossa bianca in questo
        // secondo caso e, dalla terza mossa, disallineava tutta la partita.
        std::size_t dotPos = token.find('.');
        if (dotPos != std::string::npos)
        {
            std::string moveText = token.substr(dotPos + 1);
            if (moveText.empty())
            {
                expectBlackMove = false;
                continue;
            }
            whiteMoves.push_back(moveText);
            expectBlackMove = true;
        }
        else if (expectBlackMove)
        {
            // Ignora eventuali risultati PGN, che non sono mosse.
            if (token != "1-0" && token != "0-1" && token != "1/2-1/2" && token != "*")
                blackMoves.push_back(token);
            expectBlackMove = false;
        }
        else
        {
            // Mossa bianca successiva a un numero di mossa separato.
            if (token != "1-0" && token != "0-1" && token != "1/2-1/2" && token != "*")
            {
                whiteMoves.push_back(token);
                expectBlackMove = true;
            }
        }
    }

    if (whiteMoves.empty())
    {
        std::cerr << "Nessuna mossa trovata nel file " << file_game << std::endl;
        return 1;
    }
    const size_t num_moves = std::min(whiteMoves.size(), blackMoves.size());
    if (num_moves == 0)
    {
        std::cerr << "Il file non contiene coppie di mosse valide: " << file_game << std::endl;
        return 1;
    }

    std::vector<MossaAnalizzata> game(num_moves);
    std::vector<std::string> reportComments;
    std::vector<std::string> reportBestMoves;
    std::vector<double> reportDeltas;

    if (!start())
    {
        std::cerr << "Impossibile avviare il motore di scacchi." << std::endl;
        return 1;
    }

    // CICLO PRINCIPALE DI ANALISI: per ogni coppia di mosse (bianco + nero) della partita
    std::string fenPosizioneCorrente = FENStart;
    std::string msg;
    size_t mosse_processate = 0; // Traccia quante mosse sono state effettivamente processate
    for (size_t i = 0; i < num_moves; ++i)
    {
        std::string fenPrimaMossaBianco;
        std::string fenPrimaMossaNero;

        try
        {
            // ========== FASE 1: IMPOSTAZIONE DATI DELLA MOSSA ==========
            // std::cout << "\n========== MOSSA " << (i + 1) << " ==========" << std::endl;
            // std::cout << "Mosse da file - Bianco: " << whiteMoves[i] << ", Nero: " << blackMoves[i] << std::endl;

            game[i].set_n_mossa(static_cast<int>(i + 1));
            msg.clear();
            Sleep(1000);
            msg = "Impostazione dellamossa " + to_string(i + 1);
            callTextToSpeech(msg);
            game[i].set_alu_bianco(whiteMoves[i]);
            game[i].set_alu_nero(blackMoves[i]);
            Sleep(1000);

            msg.clear();
            msg = "Fase 2: analisi mossa del bianco\n";
            // callTextToSpeech(msg);
            //  ========== FASE 2: ANALISI MOSSA DEL BIANCO ==========
            //  La FEN da usare per il bianco è sempre la posizione corrente prima della mossa del bianco.
            //  std::cout << "FEN prima mossa bianco: " << fenPosizioneCorrente << std::endl;
            fenPrimaMossaBianco = fenPosizioneCorrente;
            std::string fenDopoBianco = applyMoveToFEN(fenPrimaMossaBianco, game[i].get_alu_bianco(), 'b');
            // std::cout << "FEN dopo mossa bianco: " << fenDopoBianco << std::endl;

            // Validazione: verifica che la FEN sia valida (deve iniziare il turno del nero)
            if (fenDopoBianco.find(" b ") == std::string::npos)
            {
                std::cerr << "ERRORE: FEN non valida dopo mossa del bianco " << i + 1 << ": " << game[i].get_alu_bianco() << std::endl;
                std::cerr << "FEN generata: " << fenDopoBianco << std::endl;
                std::cerr << "FEN deve contenere ' b ' per il turno del nero" << std::endl;
                break; // Ferma l'analisi
            }

            game[i].set_stock_bianco(fenDopoBianco);
            FENCurrent = fenDopoBianco;

            // Comando UCI: posiziona il motore sulla FEN della posizione dopo la mossa del bianco
            // Termina sempre l'eventuale ricerca precedente prima di riusare
            // il motore. Senza questo stop Stockfish può restituire il
            // bestmove della posizione precedente (il problema si manifesta
            // soprattutto quando si passa dalla mossa bianca a quella nera).
            sendCommand("stop\n");
            Sleep(100);
            sendCommand("position fen " + fenDopoBianco + "\n");

            // Comando UCI: analizza la posizione a profondità 15
            sendCommand("go depth 15\n");

            // Recupera la valutazione di Stockfish e la salva (diviso 100 per ottenere centesimi di pedone)
            double evalB = evalStock();
            // std::cout << "Valutazione dopo mossa bianco: " << evalB << std::endl;
            game[i].set_eval_prima_b(evalB);
            Sleep(1000);

            msg.clear();
            msg = "Fase 3: analisi mossa del nero\n";
            // callTextToSpeech(msg);
            //  ========== FASE 3: ANALISI MOSSA DEL NERO ==========
            //  La FEN da usare per il nero è sempre la posizione dopo la mossa del bianco corrente.
            //  std::cout << "FEN prima mossa nero: " << fenDopoBianco << std::endl;
            //  Usa la variabile dichiarata fuori dal blocco try: la FEN deve
            //  rimanere disponibile anche nella fase di ricerca della best
            //  move. Una dichiarazione locale qui la nascondeva, lasciando
            //  vuota la FEN usata per le mosse nere errate e desincronizzando
            //  il motore UCI alla mossa successiva.
            fenPrimaMossaNero = fenDopoBianco;

            std::string fenDopoNero = applyMoveToFEN(fenPrimaMossaNero, game[i].get_alu_nero(), 'n');
            // std::cout << "FEN dopo mossa nero: " << fenDopoNero << std::endl;

            // Validazione: verifica che la FEN sia valida (deve iniziare il turno del bianco)
            if (fenDopoNero.find(" w ") == std::string::npos)
            {
                std::cerr << "ERRORE: FEN non valida dopo mossa del nero " << i + 1 << ": " << game[i].get_alu_nero() << std::endl;
                std::cerr << "FEN generata: " << fenDopoNero << std::endl;
                std::cerr << "FEN deve contenere ' w ' per il turno del bianco" << std::endl;
                break; // Ferma l'analisi
            }

            game[i].set_stock_nero(fenDopoNero);
            FENCurrent = fenDopoNero;
            // Comando UCI: posiziona il motore sulla FEN della posizione dopo la mossa del nero
            sendCommand("stop\n");
            Sleep(100);
            sendCommand("position fen " + fenDopoNero + "\n");

            // Comando UCI: analizza la posizione a profondità 15
            sendCommand("go depth 15\n");

            // Recupera la valutazione di Stockfish dopo la mossa del nero
            double evalN = evalStock();
            // std::cout << "Valutazione dopo mossa nero: " << evalN << std::endl;
            game[i].set_eval_prima_n(evalN);
            //  Aggiorniamo la posizione corrente per la prossima iterazione.
            fenPosizioneCorrente = fenDopoNero;
            Sleep(1000);
        }
        catch (const std::exception &e)
        {
            std::cerr << "\n!!! ECCEZIONE CATTURATA DURANTE MOSSA " << (i + 1) << " !!!" << std::endl;
            std::cerr << "Tipo: " << typeid(e).name() << std::endl;
            std::cerr << "Messaggio: " << e.what() << std::endl;
            std::cerr << "Mosse processate finora: " << mosse_processate << std::endl;
            if (i > 0)
            {
                std::cerr << "Ultima FEN corretta: " << fenPosizioneCorrente << std::endl;
            }
            std::cerr << "Mossa bianco problematica: " << (i < whiteMoves.size() ? whiteMoves[i] : "N/A") << std::endl;
            std::cerr << "Mossa nero problematica: " << (i < blackMoves.size() ? blackMoves[i] : "N/A") << std::endl;
            break;
        }
        catch (...)
        {
            std::cerr << "\n!!! ECCEZIONE SCONOSCIUTA DURANTE MOSSA " << (i + 1) << " !!!" << std::endl;
            std::cerr << "Mosse processate finora: " << mosse_processate << std::endl;
            break;
        }

        msg.clear();
        msg = "Fase 4: calcolo delta\n";
        // callTextToSpeech(msg);
        //  ========== FASE 4: CALCOLO DEL DELTA DI VALUTAZIONE ==========
        //  Determina la valutazione PRIMA della mossa del giocatore analizzato
        double evalBeforeMove = 0.0;
        if (colore == 'b')
        {
            // Se analizziamo il BIANCO: eval prima = eval dopo il nero della mossa precedente
            // (oppure 0 se è la prima mossa del bianco)
            evalBeforeMove = (i == 0) ? 0.0 : game[i - 1].get_eval_prima_n();
        }
        else
        {
            // Se analizziamo il NERO: eval prima = eval dopo il bianco della mossa corrente
            // Invertire il segno perché Stockfish calcola dal punto di vista del bianco
            evalBeforeMove = -game[i].get_eval_prima_b();
        }

        // Valutazione DOPO la mossa del giocatore analizzato
        double currentEval = 0.0;
        if (colore == 'b')
        {
            currentEval = game[i].get_eval_prima_b();
        }
        else
        {
            // Invertire il segno per il nero
            currentEval = -game[i].get_eval_prima_n();
        }

        // Conserva il delta con il segno per il report; il valore assoluto viene
        // usato solo per classificare l'entita della variazione.
        double signedDelta = currentEval - evalBeforeMove;
        double delta = std::fabs(signedDelta);
        std::string commentoScritto;

        std::string spokenMoveText = "Mossa numero " + std::to_string(i + 1) + ". Mossa del bianco: " +
                                     game[i].get_alu_bianco() + ". Mossa del nero: " + game[i].get_alu_nero();
        // callTextToSpeech(spokenMoveText);
        Sleep(1000);
        msg.clear();
        msg = "Classificazione della mossa\n";
        // ========== FASE 5: CLASSIFICAZIONE DELLA MOSSA IN BASE AL DELTA ==========
        callTextToSpeech(msg);

        // Basato sul delta di valutazione, classifichiamo la qualità della mossa
        if (delta >= 0.5 && delta < 1.0)
        {
            commentoScritto = "Mossa sbagliata: " + (colore == 'b' ? game[i].get_alu_bianco() : game[i].get_alu_nero());
        }
        else if (delta >= 1.0 && delta < 1.5)
        {
            commentoScritto = "Mossa molto sbagliata: " + (colore == 'b' ? game[i].get_alu_bianco() : game[i].get_alu_nero());
        }
        else if (delta >= 1.5)
        {
            commentoScritto = "Mossa pessima: " + (colore == 'b' ? game[i].get_alu_bianco() : game[i].get_alu_nero());
        }
        msg.clear();

        if (!commentoScritto.empty())
        {
            Sleep(2000);
            msg = "Ricerca della best move\n";
            callTextToSpeech(msg);
        }
        // ========== FASE 6: RICERCA DELLA BEST MOVE ==========
        // Se la mossa è stata classificata come errata, cerchiamo quale sarebbe stata la miglior mossa.
        // Per il nero la FEN da usare è la posizione prima della mossa del nero, cioè la FEN dopo il bianco.
        std::string bestMove;
        // Conserva la FEN corrente (posizione dopo entrambe le mosse della coppia)
        // PRIMA di qualsiasi operazione di ricerca della best move
        std::string fenPosizioneCorrenteSalvata = fenPosizioneCorrente;
        std::string fenCurrentSalvata = FENCurrent;

        if (!commentoScritto.empty())
        {
            Sleep(2000);
            std::string fenDaUsare = (colore == 'b') ? fenPrimaMossaBianco : fenPrimaMossaNero;
            std::cout << commentoScritto << " (delta: " << delta << ")" << std::endl;

            // Richiedi al motore la best move dalla posizione corretta, con il lato giusto da muovere
            // 'colore' indica il giocatore analizzato: b = mossa bianca,
            // n = mossa nera. La partita continua poi dalla FEN salvata.
            bestMove = askBestMoveFromPosition(fenDaUsare, colore);
            if (!bestMove.empty() && bestMove != "1")
            {
                std::cout << "Best move suggerita: " << bestMove << std::endl;
            }
            else
            {
                std::cout << "Nessuna best move suggerita." << std::endl;
            }

            // RIPRISTINO COMPLETO dello stato precedente la ricerca della best move
            // Questo è CRITICO per continuare l'analisi dal punto giusto
            fenPosizioneCorrente = fenPosizioneCorrenteSalvata;
            FENCurrent = fenCurrentSalvata;
            sendCommand("position fen " + fenPosizioneCorrente + "\n");
            Sleep(100);
            sendCommand("stop\n");
            Sleep(100);
        }

        // La ricerca della best move viene eseguita una sola volta per
        // iterazione. Una seconda ricerca nella stessa iterazione lasciava
        // nel buffer UCI il bestmove precedente e poteva bloccare
        // getOutputMove(), impedendo di proseguire con la partita.

        // Salva il commento e la best move nei vettori per il report finale
        reportComments.push_back(commentoScritto.empty() ? "Nessun commento" : commentoScritto);
        reportBestMoves.push_back(bestMove.empty() || bestMove == "1" ? "N/A" : bestMove);
        reportDeltas.push_back(signedDelta);
        mosse_processate++; // Incrementa il contatore delle mosse processate
    }

    std::string reportFileName = "..//registrazioni//report.txt";
    std::ofstream reportFile(reportFileName);

    if (!reportFile.is_open())
    {
        std::cerr << "Errore: impossibile creare il file di report " << reportFileName << std::endl;
        return 1;
    }
    printf("Salvataggio report\n");
    reportFile << "Report della partita: " << file_game << std::endl;
    reportFile << "Colore del giocatore analizzato: " << (colore == 'b' ? "Bianco" : "Nero") << std::endl;
    reportFile << "--------------------------------------------------" << std::endl;

    for (size_t j = 0; j < mosse_processate; ++j)
    {
        reportFile << "Mossa " << game[j].get_n_mossa() << ":" << std::endl;
        reportFile << "  Mossa Bianco: " << game[j].get_alu_bianco() << ", FEN: " << game[j].get_stock_bianco()
                   << ", Valutazione Stockfish: " << formatStockfishEval(game[j].get_eval_prima_b()) << std::endl;
        reportFile << "  Mossa Nero: " << game[j].get_alu_nero() << ", FEN: " << game[j].get_stock_nero()
                   << ", Valutazione Stockfish: " << formatStockfishEval(game[j].get_eval_prima_n()) << std::endl;
        reportFile << "  Delta valutazione: "
                   << (j < reportDeltas.size() ? formatStockfishEval(reportDeltas[j]) : "+0.00") << std::endl;
        reportFile << "  Commento: " << (j < reportComments.size() ? reportComments[j] : "Nessun commento") << std::endl;
        reportFile << "  Best move consigliata: " << (j < reportBestMoves.size() ? reportBestMoves[j] : "N/A") << std::endl;
        reportFile << "--------------------------------------------------" << std::endl;
    }

    reportFile.close();
    std::cout << "Report della partita scritto in: " << reportFileName << std::endl;
    return 0;
}

std::string applyMoveToFEN(const std::string &baseFen, const std::string &moveInput, char colore)
{
    /* funzione che riceve la mossa di lunghezza 3 o 2 caratteri in input di tipo algebrico italiano e deve restituire la FEN della posizione dopo la mossa.
       La funzione lavora sempre a partire dalla FEN passata in input, evitando dipendenze dalla variabile globale FENCurrent.
     */
    std::string moveText = moveInput;
    std::size_t pos = moveText.find_last_not_of("+#");

    if (pos != std::string::npos)
    {
        std::size_t newpos = pos + 1;
        moveText.erase(newpos);
    }

    std::string fenBase = baseFen.empty() ? FENCurrent : baseFen;
    FenState state = parseFEN(fenBase);
    bool whiteMove = (colore == 'b');
    std::string move = normalizeMove(moveText);
    std::string FENstock;

    // Il campo side-to-move della FEN in ingresso non deve prevalere sul
    // colore dichiarato della mossa. Questo è importante anche quando la FEN
    // arriva dopo una ricerca della best move e il motore ha usato una
    // posizione temporanea.
    auto finishFen = [&]() -> std::string
    {
        state.sideToMove = whiteMove ? 'b' : 'w';
        if (state.sideToMove == 'b')
            ++state.fullmove;
        FENstock = buildFEN(state);
        FENCurrent = FENstock;
        return FENstock;
    };

    if (move == "00" || move == "0-0" || move == "O-O" || move == "OO")
    {
        int kingRow = whiteMove ? 7 : 0;
        int rookCol = 7;
        int kingCol = 4;
        int targetKingCol = 6;
        int targetRookCol = 5;
        if (state.board[kingRow][kingCol] == (whiteMove ? 'K' : 'k') &&
            state.board[kingRow][rookCol] == (whiteMove ? 'R' : 'r') &&
            state.board[kingRow][5] == '.' && state.board[kingRow][6] == '.' &&
            ((whiteMove && state.castling.find('K') != std::string::npos) || (!whiteMove && state.castling.find('k') != std::string::npos)))
        {
            state.board[kingRow][kingCol] = '.';
            state.board[kingRow][rookCol] = '.';
            state.board[kingRow][targetKingCol] = whiteMove ? 'K' : 'k';
            state.board[kingRow][targetRookCol] = whiteMove ? 'R' : 'r';
            if (whiteMove)
            {
                removeCastlingRight(state.castling, 'K');
                removeCastlingRight(state.castling, 'Q');
            }
            else
            {
                removeCastlingRight(state.castling, 'k');
                removeCastlingRight(state.castling, 'q');
            }
            state.enPassant = "-";
            state.halfmove += 1;
            return finishFen();
        }
    }
    else if (move == "000" || move == "0-0-0" || move == "O-O-O" || move == "OOO")
    {
        int kingRow = whiteMove ? 7 : 0;
        int rookCol = 0;
        int kingCol = 4;
        int targetKingCol = 2;
        int targetRookCol = 3;
        if (state.board[kingRow][kingCol] == (whiteMove ? 'K' : 'k') &&
            state.board[kingRow][rookCol] == (whiteMove ? 'R' : 'r') &&
            state.board[kingRow][1] == '.' && state.board[kingRow][2] == '.' && state.board[kingRow][3] == '.' &&
            ((whiteMove && state.castling.find('Q') != std::string::npos) || (!whiteMove && state.castling.find('q') != std::string::npos)))
        {
            state.board[kingRow][kingCol] = '.';
            state.board[kingRow][rookCol] = '.';
            state.board[kingRow][targetKingCol] = whiteMove ? 'K' : 'k';
            state.board[kingRow][targetRookCol] = whiteMove ? 'R' : 'r';
            if (whiteMove)
            {
                removeCastlingRight(state.castling, 'K');
                removeCastlingRight(state.castling, 'Q');
            }
            else
            {
                removeCastlingRight(state.castling, 'k');
                removeCastlingRight(state.castling, 'q');
            }
            state.enPassant = "-";
            state.halfmove += 1;
            return finishFen();
        }
    }

    std::string promo;
    std::size_t eqPos = move.find('=');
    if (eqPos != std::string::npos)
    {
        promo = move.substr(eqPos + 1);
        move = move.substr(0, eqPos);
    }

    bool isCapture = false;
    std::size_t xPos = move.find('x');
    if (xPos == std::string::npos)
        xPos = move.find('X');
    if (xPos != std::string::npos)
    {
        isCapture = true;
        move.erase(xPos, 1);
    }

    char pieceType = 'P';
    std::string body = move;
    if (!body.empty())
    {
        char firstChar = static_cast<char>(body[0]);
        char parsedPiece = '\0';
        if (parsePieceLetter(firstChar, parsedPiece))
        {
            pieceType = parsedPiece;
            body = body.substr(1);
        }
    }

    if (body.size() < 2)
    {
        return finishFen();
    }

    std::string destination = body.substr(body.size() - 2, 2);
    std::string prefix = body.size() > 2 ? body.substr(0, body.size() - 2) : "";
    int destRow = -1;
    int destCol = -1;
    if (!squareToIndex(destination, destRow, destCol))
    {
        return finishFen();
    }

    int fromRow = -1;
    int fromCol = -1;
    if (!findSourceSquare(state, destRow, destCol, pieceType, whiteMove, isCapture, prefix, fromRow, fromCol))
    {
        state.sideToMove = whiteMove ? 'b' : 'w';
        if (state.sideToMove == 'b')
            ++state.fullmove;
        return finishFen();
    }

    char movingPiece = state.board[fromRow][fromCol];
    char capturedPiece = state.board[destRow][destCol];
    bool isEnPassant = false;
    if (pieceType == 'P' && isCapture && capturedPiece == '.' && state.enPassant == destination)
        isEnPassant = true;

    if (isEnPassant)
    {
        state.board[fromRow][destCol] = '.';
    }

    state.board[fromRow][fromCol] = '.';
    state.board[destRow][destCol] = movingPiece;
    if (pieceType == 'P' && !promo.empty())
        state.board[destRow][destCol] = whiteMove ? static_cast<char>(std::toupper(static_cast<unsigned char>(promo[0])))
                                                  : static_cast<char>(std::tolower(static_cast<unsigned char>(promo[0])));

    if (movingPiece == (whiteMove ? 'K' : 'k'))
    {
        if (whiteMove)
        {
            removeCastlingRight(state.castling, 'K');
            removeCastlingRight(state.castling, 'Q');
        }
        else
        {
            removeCastlingRight(state.castling, 'k');
            removeCastlingRight(state.castling, 'q');
        }
    }
    if (movingPiece == (whiteMove ? 'R' : 'r'))
    {
        if (whiteMove)
        {
            if (fromRow == 7 && fromCol == 0)
                removeCastlingRight(state.castling, 'Q');
            if (fromRow == 7 && fromCol == 7)
                removeCastlingRight(state.castling, 'K');
        }
        else
        {
            if (fromRow == 0 && fromCol == 0)
                removeCastlingRight(state.castling, 'q');
            if (fromRow == 0 && fromCol == 7)
                removeCastlingRight(state.castling, 'k');
        }
    }
    if (capturedPiece != '.' && ((capturedPiece == 'R' && destRow == 7 && destCol == 0) || (capturedPiece == 'R' && destRow == 7 && destCol == 7) ||
                                 (capturedPiece == 'r' && destRow == 0 && destCol == 0) || (capturedPiece == 'r' && destRow == 0 && destCol == 7)))
    {
        if (destRow == 7 && destCol == 0)
            removeCastlingRight(state.castling, std::isupper(static_cast<unsigned char>(capturedPiece)) ? 'Q' : 'q');
        if (destRow == 7 && destCol == 7)
            removeCastlingRight(state.castling, std::isupper(static_cast<unsigned char>(capturedPiece)) ? 'K' : 'k');
        if (destRow == 0 && destCol == 0)
            removeCastlingRight(state.castling, std::isupper(static_cast<unsigned char>(capturedPiece)) ? 'Q' : 'q');
        if (destRow == 0 && destCol == 7)
            removeCastlingRight(state.castling, std::isupper(static_cast<unsigned char>(capturedPiece)) ? 'K' : 'k');
    }

    if (pieceType == 'P' && std::abs(destRow - fromRow) == 2)
        state.enPassant = indexToSquare((fromRow + destRow) / 2, destCol);
    else
        state.enPassant = "-";

    if (pieceType == 'P' || capturedPiece != '.')
        state.halfmove = 0;
    else
        ++state.halfmove;

    return finishFen();
}