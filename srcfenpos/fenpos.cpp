
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>
#include <sstream>
using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 6)
    {
        cout << "fenpos v.1.8 - Usage: fenpos <textProblem in double quotes> <Who> <numMove> <numProblem> <fenstring in double quotes>" << endl;
        cout << "fenpos non valida una fen ma la trasforma in formato rotn" << endl;
        return 1;
    }
    int numero = 0, numMove = 0, numProblem = 0;

    string fen, who, textProblem;
    textProblem = argv[1];
    who = argv[2];
    who[0] = toupper(who[0]);
    numero = stoi(argv[3]);
    numMove = (who == "B" || who == "b") ? numero - 1 : numero;
    numProblem = stoi(argv[4]);
    fen = argv[5];

    // === SPLIT FEN ===
    stringstream ss(fen);
    string fields[6];
    for (int i = 0; i < 6 && ss >> fields[i]; i++)
        ;

    string pezzi = fields[0];
    string castling = fields[2].empty() ? "-" : fields[2];
    string enpassant = fields[3].empty() ? "-" : fields[3];
    // =================

    string whitePieces = "", blackPieces = "";
    int row = 8, col = 0;

    map<char, char> pieces = {
        {'P', 'P'}, {'N', 'C'}, {'B', 'A'}, {'R', 'T'}, {'Q', 'D'}, {'K', 'R'}, {'p', 'P'}, {'n', 'C'}, {'b', 'A'}, {'r', 'T'}, {'q', 'D'}, {'k', 'R'}};

    // === PARSING PEZZI ===
    for (char c : pezzi)
    {
        if (c == '/')
        {
            row--;
            col = 0;
        }
        else if (isdigit(c))
        {
            col += (c - '0');
        }
        else if (pieces.count(c))
        {
            char piece = pieces[c];
            char column = 'a' + col;
            string position = string(1, piece) + column + to_string(row);
            if (isupper(c))
                whitePieces += position + ";";
            else
                blackPieces += position + ";";
            col++;
        }
    }
    if (!whitePieces.empty())
        whitePieces.pop_back(); // tolgo l'ultimo ;
    if (!blackPieces.empty())
        blackPieces.pop_back(); // tolgo l'ultimo ;

    string result1 = "B:" + whitePieces;
    string result2 = "N:" + blackPieces;

    // === LOGICA ARROCCHI DEFINITIVA CON ; PRIMA ===
    bool wK = false, wQ = false, bK = false, bQ = false;

    if (castling != "-")
    {
        for (char c : castling)
        {
            if (c == 'K')
                wK = true;
            if (c == 'Q')
                wQ = true;
            if (c == 'k')
                bK = true;
            if (c == 'q')
                bQ = true;
        }
    }

    // chiudo i pezzi con ; e poi aggiungo NS/NL
    if (!wK || !wQ)
    {
        if (!whitePieces.empty())
            result1 += ";"; // <-- QUI la chiusura
        if (!wK)
            result1 += "NS;";
        if (!wQ)
            result1 += "NL;";
    }

    if (!bK || !bQ)
    {
        if (!blackPieces.empty())
            result2 += ";"; // <-- QUI la chiusura
        if (!bK)
            result2 += "NS;";
        if (!bQ)
            result2 += "NL;";
    }
    // =============================================

    string nameFile = "prob" + to_string(numProblem) + ".txt";
    ofstream outFile(nameFile.c_str());
    outFile << "T:" << textProblem << endl;
    outFile << "V:" << who << ";" << endl;
    outFile << result1 << endl;
    outFile << result2 << endl;
    outFile << "M:" << who << ";" << endl;
    outFile << "P:" << numMove << ";" << endl;
    outFile.close();

    return 0;
}
