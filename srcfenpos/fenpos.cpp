// fenpos.cpp
/*
il programma ricevuta, in argc e argv, una stringa fen, la traduce in una stringa
del formato:
b: che indica i pezzi del Bianco oppure N: che indica i pezzi del Nero,
seguita da un elenco separato da punti e virgole,
dove ogni elemento dell'elenco è formato da
prima lettera in italiano che rappresenta il pezzo
seconda lettera minuscola che rappresenta la colonna della scacchiera
terza lettera numerica da 1 a 8 che rappresenta la riga della scacchiera
Si devono ottenere due stringhe: una per i pezzi del Bianco e una per i pezzi del Nero.
Il file prob{numero}.txt deve avere un tracciato cosi costituito:
T: testo del problema
V: B o N a seconda dle punto di vista che conviene avere per risolvere il problema: se il problema è per il Bianco o per il Nero
B:elenco dei pezzi del Bianco
N:elenco dei pezzi del Nero
U: elenco dei pezzi che possono fare enpassant e catturare un pedone avversario, con la casa in cui si trova il pedone che può fare enpassant e la casa in cui si trova il pedone da catturare.Tale record esiste solo se nella fen esiste la segnalazione di enpassant (terz'ultimo campo)
M: B o N a seconda del colore che deve muovere
p: il numero di mossa da cui iniziare, ottenuto dalla fen e diminuito di 1
Va fatta particolare attenzione se nella fen position il terz'ultimo campo, dedicato all'enpassant, è diverso da "-" ma valorizzato con una casa.
Non sempre esiste un pedone avversario che possa utilizzare la catture, quando un pedone viene mosso di due passi. Questo dipende dallo standard se vecchio o nuovo. Nel vecchio standard si riporta sempre il campo enpassant quando un pedone viene mosso di due passi. Nel nuov standard, invece, viene riportato solo se esiste un pedonavversario che ne possa usufruire.

Nll'ipotesi che ci sia un pedone avversario che ne possa usufuire, la casa indicata dal campo enpassant è la casa in cui può andare un pedone avversario catturando un pedone che si trova oltre di una casa. Ad esempio se tale casa vale c3 significa che è stato mossa in colonna c un pedone di 2 passi rispetto alla casa di partenza (c4) ed esiste un pedone avversario affianco che può catturarlo e andare in c3.

Analogamente si potrebbe fare esempio di un pedone nero che può essere catturato per enpassant.
In particolare nel file prob{numero}.txt prima di M: va aggiunto il record "U:" seguito dalla casa (o anche due case) in cui c'è il pedone che può fare enpassant e catturare il pedone avversario segnalato in quel campo.

Ad esempio se c fosse:
un pedone bianco in a4 e uno bianco in c4 e il pedone da catturare fosse in b4 allora si avrebbe:
U:a4c4b4

Se invece c'è un solo pedone bianco in a4 e un pedone nero da catturare in b4 allora

U:a4b4
*/
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

#include <fstream>

int main(int argc, char *argv[])
{
    if (argc < 6)
    {
        cout << "Usage: fenpos <textProblem between double quotes> <Who_string> <numMove> <numProblem> <fenstring>"
             << endl;
        return 1;
    }
    int numero = 0, numMove = 0, numProblem = 0;
    string fen, who, textProblem;
    textProblem.clear();
    fen.clear();
    who.clear();
    textProblem = argv[1];
    who = argv[2];
    numero = stoi(argv[3]);
    if (who == "B" || who == "b")
    {
        numMove = numero - 1;
    }
    else if (who == "N" || who == "n")
    {
        numMove = numero;
    }
    else
    {
        std::cout << "Error who - hints: B or N" << std::endl;
        exit(1);
    }
    numProblem = stoi(argv[4]);
    fen = argv[5];
    string result1 = "", result2 = "";
    string whitePieces = "";
    string blackPieces = "";

    int row = 8;
    int col = 0;

    map<char, char> pieces = {
        {'P', 'P'}, {'N', 'C'}, {'B', 'A'}, {'R', 'T'}, {'Q', 'D'}, {'K', 'R'}, {'p', 'P'}, {'n', 'C'}, {'b', 'A'}, {'r', 'T'}, {'q', 'D'}, {'k', 'R'}};

    for (char c : fen)
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
            {
                whitePieces += position + ";";
            }
            else
            {
                blackPieces += position + ";";
            }
            col++;
        }
    }

    if (!whitePieces.empty())
        whitePieces.pop_back();
    if (!blackPieces.empty())
        blackPieces.pop_back();

    result1 = "B:" + whitePieces;
    result2 = "N:" + blackPieces;
    // cout << result << endl;
    string nameFile = "prob" + to_string(numProblem) + ".txt";
    ofstream outFile(nameFile.c_str(), std::ios::trunc);
    outFile << "T:" + textProblem << endl;
    string V = "V:";
    V.push_back(toupper(who[0]));
    V.append(";");
    outFile << V << endl;
    outFile << result1 << endl;
    outFile << result2 << endl;

    /*
    devo innanzitutto
    1. verificare se il campo enpassant della fen è diverso da "-"
    2. se è diverso lo salvo in una variabile. Esso costtuisce la casa in cui un pedone avversario può andare per enpassant.
    3. devo verificare se esiste un pedone avversario che possa usufruire di tale enpassant.
    4. se esiste il pedone avversario devo creare il record U: contenente l'elenco dei pedoni avversari che possono andare nella casa indicata dal campo enpassant seguita dalla casa dove si trova il pedone da catturare
    5. altrimenti non viene messo il record U:

    */

    size_t pos = fen.find(' ');

    if (pos != string::npos)
    {
        size_t pos2 = fen.find(' ', pos + 1);
        if (pos2 != string::npos)
        {
            size_t pos3 = fen.find(' ', pos2 + 1);
            if (pos3 != string::npos)
            {
                string enpassant = fen.substr(pos2 + 1, pos3 - pos2 - 1);
                if (enpassant != "-")
                {
                    char targetColumn = enpassant[0];
                    int targetRow = enpassant[1] - '0';
                    char opponentPawn = (who == "B") ? 'p' : 'P';
                    char opponentColumnLeft = targetColumn - 1;
                    char opponentColumnRight = targetColumn + 1;

                    string U = "U:";
                    bool hasEnpassantMove = false;

                    for (char c : fen)
                    {
                        if (c == opponentPawn)
                        {
                            int pieceRow = row;
                            int pieceCol = col;
                            if ((pieceCol == opponentColumnLeft || pieceCol == opponentColumnRight) && pieceRow == targetRow)
                            {
                                U += string(1, c) + to_string(pieceCol) + to_string(pieceRow) + enpassant;
                                hasEnpassantMove = true;
                            }
                        }
                        if (c == '/')
                        {
                            row--;
                            col = 0;
                        }
                        else if (isdigit(c))
                        {
                            col += (c - '0');
                        }
                        else
                        {
                            col++;
                        }
                    }

                    if (hasEnpassantMove)
                    {
                        outFile << U << endl;
                    }
                }
            }
        }
    }

    string M = "M:";
    M.push_back(toupper(who[0]));
    M.append(";");
    outFile << M << endl;

    outFile << "P:" + to_string(numMove) + ";" << endl;
    outFile.close();

    return 0;
}