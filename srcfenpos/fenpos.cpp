// fenpos.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>
#include <sstream>
using namespace std;

bool searchPawn(string fen, string sq)
{
    char colT = sq[0], rowT = sq[1];
    stringstream ss(fen);
    string p;
    ss >> p;
    int r = 8, c = 0;
    for (char x : p)
    {
        if (x == '/')
        {
            r--;
            c = 0;
        }
        else if (isdigit(x))
        {
            c += x - '0';
        }
        else
        {
            if (('a' + c) == colT && ('0' + r) == rowT)
                return x == 'P' || x == 'p';
            c++;
        }
    }
    return false;
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        cout << "fenpos v.2.2 - Usage: fenpos <testo in doppi apici > numero posizione> <fen in doppi apici" << endl;
        cout << "fenpos non fa la validazione della fen ma la trasforma in formato rotn" << endl;
        return 1;
    }

    int semimosse;
    string textProblem = argv[1];
    int numProblem = stoi(argv[2]);
    string fen = argv[3];

    stringstream ss(fen);
    string fields[6];
    string who;
    for (int i = 0; i < 6 && ss >> fields[i]; i++)
        ;
    string pezzi = fields[0];
    string appo_who = fields[1];
    string castling = fields[2].empty() ? "-" : fields[2];
    string enpassant = fields[3].empty() ? "-" : fields[3];
    semimosse = stoi(fields[4]);
    int numero = stoi(fields[5]);

    string whitePieces = "", blackPieces = "";
    int row = 8, col = 0;
    map<char, char> pieces = {{'P', 'P'}, {'N', 'C'}, {'B', 'A'}, {'R', 'T'}, {'Q', 'D'}, {'K', 'R'}, {'p', 'P'}, {'n', 'C'}, {'b', 'A'}, {'r', 'T'}, {'q', 'D'}, {'k', 'R'}};

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
        whitePieces.pop_back();
    if (!blackPieces.empty())
        blackPieces.pop_back();

    string result1 = "B:" + whitePieces;
    string result2 = "N:" + blackPieces;

    if (appo_who == "b" || appo_who == "B")
    {
        who = "N";
    }
    else if (appo_who == "W" || appo_who == "w")
    {
        who = "B";
    }
    else
    {
        cout << "Errore: non trovato chi muove\n";
        exit(1);
    }
    int numMove = (who == "B") ? numero - 1 : numero;

    bool wK = false, wQ = false, bK = false, bQ = false;
    if (castling != "-")
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
    if (!wK || !wQ)
    {
        if (!whitePieces.empty())
            result1 += ";";
        if (!wK)
            result1 += "NS;";
        if (!wQ)
            result1 += "NL;";
    }
    if (!bK || !bQ)
    {
        if (!blackPieces.empty())
            result2 += ";";
        if (!bK)
            result2 += "NS;";
        if (!bQ)
            result2 += "NL;";
    }

    string nameFile = "pos_" + to_string(numProblem) + ".txt";
    ofstream outFile(nameFile.c_str());
    outFile << "T:" << textProblem << endl;
    outFile << "V:" << who << ";" << endl;
    outFile << result1 << endl
            << result2 << endl;
    outFile << "M:" << who << ";" << endl
            << "P:" << numMove << ";" << endl;

    if (enpassant != "-")
    {
        char num_casa = (enpassant[1] == '3') ? '4' : (enpassant[1] == '6') ? '5'
                                                                            : '0';
        string totaleU;
        if (num_casa != '0')
        {
            if (enpassant[0] > 'a')
            {
                string pawn1;
                pawn1 += enpassant[0] - 1;
                pawn1 += num_casa;
                if (searchPawn(fen, pawn1))
                    totaleU += pawn1;
            }
            if (enpassant[0] < 'h')
            {
                string pawn2;
                pawn2 += enpassant[0] + 1;
                pawn2 += num_casa;
                if (searchPawn(fen, pawn2))
                    totaleU += pawn2;
            }
        }

        // aggiungere la casa del pedone da catturare.
        string casaDaCatturare = "";
        casaDaCatturare.append(enpassant);
        if (enpassant[1] == '3')
        {
            casaDaCatturare[1] = '4';
        }
        if (enpassant[1] == '6')
        {
            casaDaCatturare[1] = '5';
        }
        totaleU += casaDaCatturare;
        outFile << "U:" << totaleU << endl;
    }
    if (semimosse > 0)
    {
        string semiM = to_string(semimosse);
        outFile << "Z:" << semiM << endl;
    }

    outFile.close();
    return 0;
}