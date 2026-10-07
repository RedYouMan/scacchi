

// Genera un diagramma SVG di una posizione di scacchi a partire da una FEN.
//
// Compilazione in due modi possibili:
// 1. da Visual Studio Code con F5
// 2. g++ -std=c++17 -O2 make-fen.cpp -o make-fen

#include <array>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace
{
    constexpr int squareSize = 72;
    constexpr int boardX = 54;
    constexpr int boardY = 34;
    constexpr int boardSize = 8 * squareSize;
    constexpr int margin = 54;

    bool isPiece(char c)
    {
        switch (static_cast<char>(std::tolower(static_cast<unsigned char>(c))))
        {
        case 'k':
        case 'q':
        case 'r':
        case 'b':
        case 'n':
        case 'p':
            return true;
        default:
            return false;
        }
    }

    bool parseFenBoard(const std::string &fen, std::array<std::array<char, 8>, 8> &board)
    {
        const auto firstSpace = fen.find(' ');
        const std::string placement = fen.substr(0, firstSpace);
        std::istringstream ranks(placement);
        std::string rank;
        int row = 0;

        while (std::getline(ranks, rank, '/'))
        {
            if (row >= 8)
                return false;
            int col = 0;
            for (char c : rank)
            {
                if (c >= '1' && c <= '8')
                {
                    col += c - '0';
                }
                else if (isPiece(c))
                {
                    if (col >= 8)
                        return false;
                    board[row][col++] = c;
                }
                else
                {
                    return false;
                }
                if (col > 8)
                    return false;
            }
            if (col != 8)
                return false;
            ++row;
        }
        return row == 8;
    }

    std::string xmlEscape(const std::string &text)
    {
        std::string escaped;
        for (char c : text)
        {
            switch (c)
            {
            case '&':
                escaped += "&amp;";
                break;
            case '<':
                escaped += "&lt;";
                break;
            case '>':
                escaped += "&gt;";
                break;
            case '"':
                escaped += "&quot;";
                break;
            case '\'':
                escaped += "&apos;";
                break;
            default:
                escaped += c;
            }
        }
        return escaped;
    }

    std::vector<std::string> wrapText(const std::string &text, std::size_t width)
    {
        std::istringstream words(text);
        std::vector<std::string> lines;
        std::string word, line;
        while (words >> word)
        {
            if (!line.empty() && line.size() + 1 + word.size() > width)
            {
                lines.push_back(line);
                line.clear();
            }
            if (!line.empty())
                line += ' ';
            line += word;
        }
        if (!line.empty())
            lines.push_back(line);
        return lines;
    }

    void writePieceSymbol(std::ostream &out, char piece)
    {
        const bool white = std::isupper(static_cast<unsigned char>(piece));
        const char kind = static_cast<char>(std::tolower(static_cast<unsigned char>(piece)));
        const char side = white ? 'w' : 'b';
        out << "<symbol id=\"" << side << kind << "\" viewBox=\"0 0 48 56\">\n"
            << "<g stroke=\"" << (white ? "#303030" : "#eeeeee")
            << "\" stroke-width=\"2\" stroke-linejoin=\"round\" stroke-linecap=\"round\" fill=\""
            << (white ? "#fffdf2" : "#242424") << "\">\n";

        switch (kind)
        {
        case 'p':
            out << "<circle cx=\"24\" cy=\"15\" r=\"6\"/><path d=\"M18 23 Q24 19 30 23 L34 42 H14 Z\"/>\n";
            break;
        case 'r':
            out << "<path d=\"M13 10 H19 V16 H23 V10 H29 V16 H35 V10 H40 L38 24 H14 Z\"/>"
                << "<path d=\"M17 24 H35 L32 43 H16 Z\"/>\n";
            break;
        case 'n':
            out << "<path d=\"M12 43 Q13 34 19 29 L15 21 Q22 20 25 24 Q28 14 39 13 L37 21 Q43 26 38 32 L34 43 Z\"/>"
                << "<circle cx=\"34\" cy=\"22\" r=\"1.5\" fill=\"" << (white ? "#303030" : "#eeeeee") << "\"/>\n";
            break;
        case 'b':
            out << "<path d=\"M24 8 Q35 17 28 25 Q34 29 34 43 H14 Q14 29 20 25 Q13 17 24 8 Z\"/>"
                << "<path d=\"M20 18 L28 25\" fill=\"none\"/>\n";
            break;
        case 'q':
            out << "<path d=\"M13 22 L10 12 L19 18 L24 8 L29 18 L38 12 L35 22 L32 43 H16 Z\"/>"
                << "<circle cx=\"10\" cy=\"11\" r=\"2.5\"/><circle cx=\"24\" cy=\"7\" r=\"2.5\"/><circle cx=\"38\" cy=\"11\" r=\"2.5\"/>\n";
            break;
        case 'k':
            out << "<path d=\"M21 7 H27 V13 H33 V19 H27 V23 Q35 27 34 43 H14 Q13 27 21 23 V19 H15 V13 H21 Z\"/>"
                << "<path d=\"M18 31 H30\" fill=\"none\"/>\n";
            break;
        }
        out << "<path d=\"M12 43 H36 L40 50 H8 Z\"/><path d=\"M8 51 H40\" fill=\"none\"/>\n"
            << "</g></symbol>\n";
    }

    void usage(const char *program)
    {
        std::cerr << "Uso: " << program << " \"FEN\" output.svg\n"
                  << "Esempio: " << program
                  << " \"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\" partita.svg\n";
    }
} // namespace

int main(int argc, char *argv[])
{
    std::cout << "make-FEN2SVG versione 1.1 - (C) - 2026 Rosario Turco\n";
    // Permette di avviare il programma con F5 anche senza argomenti di lancio.
    const std::string defaultFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    const char *fen = argc == 1 ? defaultFen.c_str() : (argc == 3 ? argv[1] : nullptr);
    const char *outputPath = argc == 1 ? "diagramma.svg" : (argc == 3 ? argv[2] : nullptr);

    if (argc != 1 && argc != 3)
    {
        usage(argv[0]);
        return 1;
    }

    std::array<std::array<char, 8>, 8> board{};
    if (!parseFenBoard(fen, board))
    {
        std::cerr << "FEN non valida: la posizione deve contenere 8 righe corrette.\n";
        return 1;
    }

    const int height = boardY + boardSize + margin;

    std::ofstream out(outputPath, std::ios::binary);
    if (!out)
    {
        std::cerr << "Impossibile creare il file: " << outputPath << '\n';
        return 1;
    }

    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\""
        << boardSize + 2 * margin << "\" height=\"" << height
        << "\" viewBox=\"0 0 " << boardSize + 2 * margin << ' ' << height << "\">\n"
        << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n"
        << "<defs>\n";
    for (char piece : std::string("KQRBNPkqrbnp"))
        writePieceSymbol(out, piece);
    out << "</defs>\n";

    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            const int x = boardX + col * squareSize;
            const int y = boardY + row * squareSize;
            const bool dark = (row + col) % 2 == 1;
            out << "<rect x=\"" << x << "\" y=\"" << y << "\" width=\""
                << squareSize << "\" height=\"" << squareSize << "\" fill=\""
                << (dark ? "#769656" : "#eeeed2") << "\"/>\n";
            const char piece = board[row][col];
            if (piece != '\0')
            {
                const char side = std::isupper(static_cast<unsigned char>(piece)) ? 'w' : 'b';
                const char kind = static_cast<char>(std::tolower(static_cast<unsigned char>(piece)));
                out << "<use href=\"#" << side << kind << "\" x=\"" << x + 12
                    << "\" y=\"" << y + 8 << "\" width=\"48\" height=\"56\"/>\n";
            }
        }
    }

    for (int i = 0; i < 8; ++i)
    {
        const int center = boardX + i * squareSize + squareSize / 2;
        const int rankY = boardY + i * squareSize + squareSize / 2 + 5;
        out << "<text x=\"" << boardX - 20 << "\" y=\"" << rankY
            << "\" text-anchor=\"middle\" font-family=\"Arial\" font-size=\"14\">"
            << 8 - i << "</text>\n"
            << "<text x=\"" << center << "\" y=\"" << boardY + boardSize + 20
            << "\" text-anchor=\"middle\" font-family=\"Arial\" font-size=\"14\">"
            << static_cast<char>('a' + i) << "</text>\n";
    }

    out << "</svg>\n";
    return 0;
}