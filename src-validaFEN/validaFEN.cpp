// validaFEN.cpp
/*
Il programma riceve una stringa e verifica campo per campo che sia una valida FEN
Il programma ritorna 0 se ok, 1 se NOK
il tutto accompagnato da messaggio sul tipo di errore o di messaggio di FEN corretta.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>
#include <sstream>
#include <vector>
using namespace std;

bool validaFEN(const string &fen)
{
    if (fen.empty())
    {
        cout << "Errore: FEN vuota" << endl;
        return false;
    }

    vector<string> campi;
    stringstream ss(fen);
    string campo;

    // Dividi la FEN in campi separati da spazi
    while (ss >> campo)
    {
        campi.push_back(campo);
    }

    // Verifica che ci siano esattamente 6 campi
    if (campi.size() != 6)
    {
        cout << "Errore: FEN deve contenere 6 campi separati da spazi" << endl;
        return false;
    }

    // Campo 1: Posizione dei pezzi
    const string &posizione = campi[0];
    vector<string> righe;
    stringstream ss1(posizione);
    string riga;
    int contaRighe = 0;

    // Dividi la posizione in 8 righe
    while (getline(ss1, riga, '/'))
    {
        contaRighe++;
        if (contaRighe > 8)
        {
            cout << "Errore: La posizione deve contenere esattamente 8 righe" << endl;
            return false;
        }

        int contaCase = 0;
        for (char c : riga)
        {
            if (isdigit(c))
            {
                int n = c - '0';
                if (n < 1 || n > 8)
                {
                    cout << "Errore: I numeri nella posizione devono essere da 1 a 8" << endl;
                    return false;
                }
                contaCase += n;
            }
            else if (c == 'K' || c == 'Q' || c == 'R' || c == 'B' || c == 'N' || c == 'P' ||
                     c == 'k' || c == 'q' || c == 'r' || c == 'b' || c == 'n' || c == 'p')
            {
                contaCase++;
            }
            else
            {
                cout << "Errore: Carattere non valido nella posizione: " << c << endl;
                return false;
            }
        }

        if (contaCase != 8)
        {
            cout << "Errore: Ogni riga deve contenere esattamente 8 case" << endl;
            return false;
        }
    }

    if (contaRighe != 8)
    {
        cout << "Errore: La posizione deve contenere esattamente 8 righe" << endl;
        return false;
    }

    // Campo 2: Turno (w o b)
    const string &turno = campi[1];
    if (turno != "w" && turno != "b")
    {
        cout << "Errore: Il turno deve essere 'w' (bianco) o 'b' (nero)" << endl;
        return false;
    }

    // Campo 3: Arrocchi disponibili
    const string &arrocchi = campi[2];
    if (arrocchi == "-")
    {
        // Nessun arrocco disponibile, va bene
    }
    else
    {
        for (char c : arrocchi)
        {
            if (c != 'K' && c != 'Q' && c != 'k' && c != 'q')
            {
                cout << "Errore: Arrocchi non validi. Devono essere K, Q, k, q o '-'" << endl;
                return false;
            }
        }
    }

    // Campo 4: En passant
    const string &enPassant = campi[3];
    if (enPassant != "-")
    {
        if (enPassant.length() != 2)
        {
            cout << "Errore: En passant non valido" << endl;
            return false;
        }
        if (enPassant[0] < 'a' || enPassant[0] > 'h')
        {
            cout << "Errore: En passant colonna non valida" << endl;
            return false;
        }
        if (enPassant[1] != '3' && enPassant[1] != '6')
        {
            cout << "Errore: En passant riga deve essere 3 o 6" << endl;
            return false;
        }
    }

    // Campo 5: Contatore semimosse
    const string &semimosse = campi[4];
    for (char c : semimosse)
    {
        if (!isdigit(c))
        {
            cout << "Errore: Contatore semimosse deve essere un numero" << endl;
            return false;
        }
    }

    // Campo 6: Numero mossa
    const string &numeroMossa = campi[5];
    for (char c : numeroMossa)
    {
        if (!isdigit(c))
        {
            cout << "Errore: Numero mossa deve essere un numero" << endl;
            return false;
        }
    }

    return true;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "validaFEN v.1.0 - Usage: validaFEN <fen NO doppi apici>" << endl;
        return 1;
    }

    // Concatenate all arguments to rebuild the FEN in case it was not quoted
    string fen;
    for (int i = 1; i < argc; ++i)
    {
        if (i > 1)
            fen += ' ';
        fen += argv[i];
    }

    if (validaFEN(fen))
    {
        cout << "FEN valida: " << fen << endl;
        return 0;
    }
    else
    {
        cout << "FEN non valida" << endl;
        return 1;
    }
}
