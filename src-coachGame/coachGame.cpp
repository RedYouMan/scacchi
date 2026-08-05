// coachGame.cpp

#include "proto.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <fstream>
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
                                     { return c == '+' || c == '#' || c == '!' || c == '?'; }),
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
                ++row;
                col = 0;
            }
            else if (std::isdigit(static_cast<unsigned char>(ch)))
            {
                col += ch - '0';
            }
            else
            {
                state.board[row][col++] = ch;
            }
        }

        state.sideToMove = side.empty() ? 'w' : side[0];
        state.castling = castling;
        state.enPassant = enPassant;
        state.halfmove = std::stoi(halfmove);
        state.fullmove = std::stoi(fullmove);
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
                return state.board[fromRow + dir][fromCol] == '.' && state.board[fromRow + 2 * dir][fromCol] == '.';
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
        int bestRow = -1;
        int bestCol = -1;
        bool found = false;

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

                bestRow = r;
                bestCol = c;
                found = true;
                break;
            }
            if (found)
                break;
        }

        if (!found)
            return false;

        fromRow = bestRow;
        fromCol = bestCol;
        return true;
    }
} // namespace

// prototipo locale
std::string whatIsNewFEN(const std::string &moveInput, char colore);

// FEN globale da inizio gioco
std::string FENStart = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
static std::string FENCurrent;
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Uso: coachGame\"<nome file partita> <colore (es. b o n)>" << std::endl;
        return 1;
    }

    // banner di inizio
    std::cout << "CoachGame (C) 2026 - Rosario Turco" << std::endl;
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
        std::size_t dotPos = token.find('.');
        if (dotPos != std::string::npos && dotPos + 1 < token.size())
        {
            std::string moveText = token.substr(dotPos + 1);
            if (!moveText.empty())
            {
                whiteMoves.push_back(moveText);
                expectBlackMove = true;
            }
        }
        else if (expectBlackMove)
        {
            blackMoves.push_back(token);
            expectBlackMove = false;
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
    double previousEval = 0.0;
    bool havePreviousEval = true;
    std::string fenPrimaMossa;

    if (!start())
    {
        std::cerr << "Impossibile avviare il motore di scacchi." << std::endl;
        return 1;
    }

    for (size_t i = 0; i < num_moves; ++i)
    {
        game[i].set_n_mossa(static_cast<int>(i + 1));
        game[i].set_alu_bianco(whiteMoves[i]);
        game[i].set_alu_nero(blackMoves[i]);
        fenPrimaMossa = FENCurrent;

        game[i].set_stock_bianco(whatIsNewFEN(game[i].get_alu_bianco(), 'b'));
        FENCurrent = game[i].get_stock_bianco();
        sendCommand("position fen " + FENCurrent + "\n");
        sendCommand("go depth 15\n");
        game[i].set_eval_prima_b(evalStock());

        game[i].set_stock_nero(whatIsNewFEN(game[i].get_alu_nero(), 'n'));
        FENCurrent = game[i].get_stock_nero();
        sendCommand("position fen " + FENCurrent + "\n");
        sendCommand("go depth 15\n");
        game[i].set_eval_prima_n(evalStock());

        if (havePreviousEval)
        {
            double currentEval = (colore == 'b') ? game[i].get_eval_prima_b() : game[i].get_eval_prima_n();
            double delta = std::fabs(currentEval - previousEval);
            std::string commentoVocale;
            std::string commentoScrittto;

            if (delta >= 0.5 && delta < 1.0)
            {
                commentoVocale = "Questa mossa è sbagliata";
                commentoScrittto = "Mossa sbagliata: " + (colore == 'b' ? game[i].get_alu_bianco() : game[i].get_alu_nero());
            }
            else if (delta >= 1.0 && delta < 1.5)
            {
                commentoVocale = "Questa mossa è molto sbagliata";
                commentoScrittto = "Mossa molto sbagliata: " + (colore == 'b' ? game[i].get_alu_bianco() : game[i].get_alu_nero());
            }
            else if (delta >= 1.5)
            {
                commentoVocale = "Questa mossa è pessima";
                commentoScrittto = "Mossa pessima: " + (colore == 'b' ? game[i].get_alu_bianco() : game[i].get_alu_nero());
            }

            if (!commentoScrittto.empty())
            {
                std::cout << commentoScrittto << " (delta: " << delta << ")" << std::endl;
                callTextToSpeech(commentoVocale);
                sendCommand("position fen " + fenPrimaMossa + "\n");
                sendCommand("go depth 15\n");
                std::string bestMove = getOutputMove();
                if (!bestMove.empty() && bestMove != "1")
                {
                    std::cout << "Best move suggerita: " << bestMove << std::endl;
                    callTextToSpeech("La migliore risposta è " + bestMove);
                }
            }

            previousEval = (colore == 'b') ? game[i].get_eval_prima_b() : game[i].get_eval_prima_n();
        }
    }

    std::string reportFileName = "..//registrazioni//" + std::string(argv[1]) + "-report.txt";
    std::ofstream reportFile(reportFileName);

    if (!reportFile.is_open())
    {
        std::cerr << "Errore: impossibile creare il file di report " << reportFileName << std::endl;
        return 1;
    }

    reportFile << "Report della partita: " << file_game << std::endl;
    reportFile << "Colore del giocatore analizzato: " << (colore == 'b' ? "Bianco" : "Nero") << std::endl;
    reportFile << "--------------------------------------------------" << std::endl;

    for (size_t j = 0; j < game.size(); ++j)
    {
        reportFile << "Mossa " << game[j].get_n_mossa() << ":" << std::endl;
        reportFile << "  Mossa Bianco: " << game[j].get_alu_bianco() << ", FEN: " << game[j].get_stock_bianco() << ", Valutazione: " << game[j].get_eval_prima_b() << std::endl;
        reportFile << "  Mossa Nero: " << game[j].get_alu_nero() << ", FEN: " << game[j].get_stock_nero() << ", Valutazione: " << game[j].get_eval_prima_n() << std::endl;
        reportFile << "--------------------------------------------------" << std::endl;
    }

    reportFile.close();
    std::cout << "Report della partita scritto in: " << reportFileName << std::endl;
    return 0;
}

std::string whatIsNewFEN(const std::string &moveInput, char colore)
{
    /* funzione che riceve la mossa di lunghezza 3 o 2  caratteri in input di tipo algebrico italiano e deve restituire la variabile FENstock che vienecalcolata a partire da mossa e FENCurrent. Diciamo che FENCurrent è la FEN della posizione prima della mossa e FENstock è la FEN della posizione dopo la mossa. La funzione deve aggiornare anche la variabile globale FENCurrent con il valore di FENstock. La funzione deve essere implementata in modo da gestire tutte le possibili mosse legali, comprese le promozioni, gli arroccchi e le catture en passant. La funzione deve anche aggiornare correttamente il numero di mosse e il colore del giocatore da muovere.
     */
    // se 00 allora è arrocco corto mentre 000 arrocco lungo
    // char colore vale b o n
    /* se mossa in input ha alla fine i caratteri + o # vanno esclusi */
    std::string moveText = moveInput;
    std::size_t pos = moveText.find_last_not_of("+#");

    if (pos != std::string::npos)
    {
        // Cancella dal carattere successivo fino alla fine della stringa
        std::size_t newpos = pos + 1;
        moveText.erase(newpos);
    }
    FenState state = parseFEN(FENCurrent);
    bool whiteMove = (colore == 'b');
    std::string move = normalizeMove(moveText);
    std::string FENstock;

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
            state.sideToMove = whiteMove ? 'b' : 'w';
            if (state.sideToMove == 'b')
                ++state.fullmove;
            FENstock = buildFEN(state);
            FENCurrent = FENstock;
            return FENstock;
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
            state.sideToMove = whiteMove ? 'b' : 'w';
            if (state.sideToMove == 'b')
                ++state.fullmove;
            FENstock = buildFEN(state);
            FENCurrent = FENstock;
            return FENstock;
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
    if (!body.empty() && std::string("NBRQK").find(std::toupper(static_cast<unsigned char>(body[0]))) != std::string::npos)
    {
        pieceType = static_cast<char>(std::toupper(static_cast<unsigned char>(body[0])));
        body = body.substr(1);
    }

    std::string destination = body.substr(body.size() - 2, 2);
    std::string prefix = body.substr(0, body.size() - 2);
    int destRow = -1;
    int destCol = -1;
    if (!squareToIndex(destination, destRow, destCol))
    {
        FENstock = buildFEN(state);
        FENCurrent = FENstock;
        return FENstock;
    }

    int fromRow = -1;
    int fromCol = -1;
    if (!findSourceSquare(state, destRow, destCol, pieceType, whiteMove, isCapture, prefix, fromRow, fromCol))
    {
        FENstock = buildFEN(state);
        FENCurrent = FENstock;
        return FENstock;
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

    state.sideToMove = whiteMove ? 'b' : 'w';
    if (state.sideToMove == 'b')
        ++state.fullmove;

    FENstock = buildFEN(state);
    FENCurrent = FENstock;
    return FENstock;
}