// Genera un diagramma SVG di una posizione di scacchi a partire da una FEN.
//
// Compilazione in due modi possibili:
// 1. da Visual Studio Code con F5
// 2. g++ -std=c++17 -O2 make-FEN2SVG.cpp -o make-FEN2SVG

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

    void usage(const char *program)
    {
        std::cerr << "Uso: " << program << " EN|IT \"FEN\" output.svg [\"Didascalia\"]\n"
                  << "Esempio: " << program
                  << " IT \"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\" partita.svg \"1. e4 e5 2. Cf3\"\n";
    }

    char pieceLetter(char piece, bool italian)
    {
        switch (static_cast<char>(std::toupper(static_cast<unsigned char>(piece))))
        {
        case 'K':
            return italian ? 'R' : 'K'; // Re / King
        case 'Q':
            return italian ? 'D' : 'Q'; // Donna / Queen
        case 'R':
            return italian ? 'T' : 'R'; // Torre / Rook
        case 'B':
            return italian ? 'A' : 'B'; // Alfiere / Bishop
        case 'N':
            return italian ? 'C' : 'N'; // Cavallo / Knight
        case 'P':
            return 'P';
        default:
            return piece;
        }
    }
} // namespace

int main(int argc, char *argv[])
{
    std::cout << "make-SVG versione 1.0 - (C) - 2026 Rosario Turco\n";
    if (argc < 4 || argc > 5)
    {
        usage(argv[0]);
        return 1;
    }

    const std::string language = argv[1];
    if (language != "EN" && language != "IT")
    {
        std::cerr << "Lingua non valida: usare EN oppure IT in maiuscolo.\n";
        return 1;
    }
    const bool italian = language == "IT";

    std::array<std::array<char, 8>, 8> board{};
    if (!parseFenBoard(argv[2], board))
    {
        std::cerr << "FEN non valida: la posizione deve contenere 8 righe corrette.\n";
        return 1;
    }

    const std::string caption = argc == 5 ? argv[4] : "";
    const auto lines = wrapText(caption, 72);
    const int textLineHeight = 25;
    const int height = boardY + boardSize + margin +
                       (lines.empty() ? 0 : 28 + static_cast<int>(lines.size()) * textLineHeight);

    std::ofstream out(argv[3], std::ios::binary);
    if (!out)
    {
        std::cerr << "Impossibile creare il file: " << argv[3] << '\n';
        return 1;
    }

    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\""
        << boardSize + 2 * margin << "\" height=\"" << height
        << "\" viewBox=\"0 0 " << boardSize + 2 * margin << ' ' << height << "\">\n"
        << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";

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
                const bool white = std::isupper(static_cast<unsigned char>(piece));
                out << "<text x=\"" << x + squareSize / 2 << "\" y=\""
                    << y + 54 << "\" text-anchor=\"middle\" font-family=\"DejaVu Sans, Arial, sans-serif\""
                    << " font-size=\"48\" font-weight=\"bold\" fill=\""
                    << (white ? "#ffffff" : "#222222")
                    << "\" stroke=\"" << (white ? "#333333" : "#eeeeee")
                    << "\" stroke-width=\"1\" paint-order=\"stroke\">"
                    << pieceLetter(piece, italian)
                    << "</text>\n";
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

    if (!lines.empty())
    {
        int y = boardY + boardSize + 58;
        out << "<text x=\"" << boardX << "\" y=\"" << y
            << "\" font-family=\"Arial, sans-serif\" font-size=\"18\" font-weight=\"bold\">Didascalia</text>\n";
        y += textLineHeight;
        for (const auto &line : lines)
        {
            out << "<text x=\"" << boardX << "\" y=\"" << y
                << "\" font-family=\"Arial, sans-serif\" font-size=\"16\">"
                << xmlEscape(line) << "</text>\n";
            y += textLineHeight;
        }
    }
    out << "</svg>\n";
    return 0;
}