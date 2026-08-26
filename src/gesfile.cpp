
// gesfile.cpp
/*

 Questo sorgente gestisce la lettura da file secondo la notazione ROTN
 Per comprendere la ROTN leggi il manuale scacchigr.txt
 Il sorgente lavora sulla ROTN in varie modalità:
 - per inizializzare la scacchiera con la funzione void init()
 - per il caricamento dei pezzi secondo una posizione caricata attraverso la funzione void load
 - per sospendere unapartita
 - per riprendere una partita attraverso la funzione load
 - per ruotare la scacchiera sfruttando sospensione e ripresa della partita

*/

#include <windows.h>
#include <iostream>
#include <cstdio>
#include "globale.h"
#include <vector>
#include <sstream>
#include "proto.h"
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include "chess.h"
#include <cctype>
#include "engine.h"
#pragma warning(disable : 4267)

using namespace std;

// segnala che i record B o N sono stati elaborati prima di V:
bool isPresentB = false;
int bianco_arrocchi[2] = {0, 0}, nero_arrocchi[2] = {0, 0};
// valgono ognuno {0,0} se sono validi, {1,0} se non valido  arrocco corto, {0,1} se non valido arrocco lungo, {1,1} se non validi entrambi
void WriteGameToFile(string nameFile, string str, bool scrivi)
{
    if (scrivi)
    {

        ofstream file_output(nameFile.c_str(), std::ios::app);
        if (file_output.is_open())
        {
            file_output << str;
            file_output.close();
        }
        else
        {
            cerr << "Impossibile aprire il file\n"
                 << endl;
        }
        // fine if scrivi
    }

    return;
}

void loadFile(int tipoPath, bool cambioColore)
{

    isPresentB = false;
    if (isProblem)
    {
        reg_to_file = false; // con i problemi NON si registra!
    }

    cleanBoard();

    if (!cambioColore)
    {
        int len = 50;
        while (len == 50)
        {

            printf("Dati da inserire\n");
            printf("File : ");
            Sleep(1500);
            callTextToSpeech(string("Inserisci nome del file senza estensione txt, seguito da return\n"));
            cin >> nameProblem;
            cin.ignore();
            len = nameProblem.length();
            if (len > 35)
            {
                callTextToSpeech(string("Warning: lunghezza massima di 35 caratteri\n"));
                len = 50;
            }
        }
    }
    else
    {
        nameProblem = "cambioColore";
    }
    if (!makeConfig(nameProblem, tipoPath))
    {
        callTextToSpeech(string("Avvenuto errore nel caricamento del problema\n"));
        callTextToSpeech(string("Avvenuto errore nel caricamento del problema\n"));
    }

    return;
}

bool makeConfig(string name, int tipoPath)
{
    int status = true; // ok
    ifstream fin;
    string nameWithExtension;

    if (tipoPath == 1) // problemi
    {
        nameWithExtension = "..//problemi//";
    }
    else
    {
        nameWithExtension = "..//sospensioni//";
    }

    nameWithExtension.append(name);
    ;
    nameWithExtension.append(".txt");
    fin.open(nameWithExtension);
    if (fin.is_open())
    {
        string row; // Variabile per contenere ogni riga letta
        isPresentB = false;

        // Leggi il file riga per riga finché ci sono righe
        // ci devono essere tutti i descrittori necessari, quindi, non meno di 6
        int countDescriptors = 0;
        while (getline(fin, row))
        {
            countDescriptors++;
            // qua posso capire se T: è valorizzato
            if (countDescriptors == 1 && (row[0] != 'T' || row.length() < 3))
            {
                callTextToSpeech(string("Il file non contiene il descrittore T: necessario e valorizzato \n"));
                callTextToSpeech(string("Correggi il problema e rilancialo\n"));
                exit(1);
            }
            if (countDescriptors == 2 && (row[0] != 'V' || row.length() < 3))
            {
                callTextToSpeech(string("Il file non contiene il descrittore V necessario e valorizzato\n"));
                callTextToSpeech(string("Correggi il problema e rilancialo\n"));
                exit(1);
            }
            if (countDescriptors == 5 && (row[0] != 'M' || row.length() < 3))
            {
                callTextToSpeech(string("Il file non contiene il descrittore M:  necessario e valorizzato\n"));
                callTextToSpeech(string("Correggi il problema e rilancialo\n"));
                exit(1);
            }
            if (countDescriptors == 6 && (row[0] != 'P' || row.length() < 3))
            {
                callTextToSpeech(string("Il file non contiene il descrittore P: necessario e valorizzato con un intero\n"));
                callTextToSpeech(string("Correggi il problema e rilancialo\n"));
                exit(1);
            }

            // successivo ok
            if (!setPieceIntoSquare(row))
            {
                callTextToSpeech(string("Correggi il problema e rilancialo\n"));
                callTextToSpeech(string("Correggi il problema e rilancialo\n"));
                exit(1);
            }
        }

        if (countDescriptors < 6)
        {
            callTextToSpeech(string("Il file non contiene tutti i descrittori necessari\n"));
            callTextToSpeech(string("Correggi il problema e rilancialo\n"));
            exit(1);
        }
        fin.close();
    }
    else
    {
        // Se il file non si apre (es. non esiste o permessi)
        cerr << "Impossibile aprire il file " << name << std::endl;
        callTextToSpeech(string("Esco per consentirti di selezionare il file giusto\n"));

        printf("Esco per consentirti di selezionare il file giusto\n");
        exit(1);
    }

    if (tipoPath == 2)
    {
        fin.close();
        std::remove(nameWithExtension.c_str());
    }
    return status;
}

bool setPieceIntoSquare(string com)
{
    char color = com[0];
    string bufferInput;

    if (com.length() > 1)
    {
        bufferInput = com.substr(2);
    }
    else
    {
        // non viene valorizzato
        callTextToSpeech(string("Controlla se nel file hai omesso dei descrittori o messo righe vuote o punti e virgola superflui\n"));
        callTextToSpeech(string("Esco per consentirti i controlli sul file\n"));
        exit(1);
    }
    vector<string> mosse;
    stringstream ss(bufferInput); // Crea uno stream dalla stringa
    string segmento;
    bool status = true;
    mosse.clear();
    segmento.clear();

    // Legge dallo stream fino al carattere ';'
    while (getline(ss, segmento, ';'))
    {
        mosse.push_back(segmento);
    }

    string mossa;
    string msg;
    while (mosse.size() > 0)
    {
        mossa = mosse.back();
        mosse.pop_back();
        // La obbligatorietà non viene richiesta per F: ,K:, C:, L:,E: e Z: usate per la sospensione e, quindi, automatiche
        if (color == 'F' || color == 'L' || color == 'K' || color == 'C' || color == 'E' || color == 'Z' || color == 'U')
        {
            // controllo non obbligatorio
        }
        else if (color != 'T' && color != 'B' && color != 'N' && color != 'M' && color != 'P' && color != 'V')
        {
            callTextToSpeech(string("Row non riconosciuta: deve iniziare con T, V, B, N, M, P.\n"));
            return false;
        }
        else
        {
            // printDebug(string("controlli superati\n"));
        }
        char myColor = toupper(color);
        // introdotti regole validazione rotn
        if (myColor == 'T')
        {
            Sleep(2000);
            callTextToSpeech(mossa);
            printf("%s\n", mossa.c_str());
        }
        else if (myColor == 'M')
        {
            if (mossa.empty() || !(mossa[0] == 'B' || mossa[0] == 'N'))
            {
                callTextToSpeech(string("M:risulta non valido, esco per farti correggere il file\n"));
                exit(1);
            }
            startColor = (mossa[0] == 'N' || mossa[0] == 'n' ? 1 : 0);
            msg = "debug: startColor settato a " + to_string(startColor) + "\n";
            printDebug(msg);
        }
        else if (myColor == 'U')
        {

            // Segnalazione di enpassant da gestire - ROS3
            EnP = true;
            callTextToSpeech(string("presenza di enpassant da gestire o meno durante il gioco\n"));

            checkU(mossa);
            if (mossa.length() == 4)
            {
                string parte1 = mossa.substr(0, 2);
                string parte2 = mossa.substr(2, 2);
                pawnEnP.push_back(parte1);
                casellaDaCatturare = parte2;
                printDebug(string("enp step 1\n"));
            }
            else
            {
                string app1 = mossa.substr(0, 2);
                string app2 = mossa.substr(2, 2);
                string app3 = mossa.substr(4, 2);
                casellaDaCatturare = app3;
                pawnEnP.push_back(app1);
                pawnEnP.push_back(app2);
                printDebug(string("enpassant step 2\n"));
            }
        }
        else if (myColor == 'Z')
        {

            string mes = "numero di semimosse pari a " + mossa + "\n";
            callTextToSpeech(mes);
        }
        else if (myColor == 'L')
        {
            level = atoi(mossa.c_str());
        }
        else if (myColor == 'K')
        {
            if (mossa.empty())
            {
                printf("Attenzione: non è stato possibile rilevare il livello di skill, verrà usato quello di default\n");
                callTextToSpeech(string("Attenzione: non è stato possibile rilevare il livello di skill, esco per farti correggere il file\n"));
                exit(1);
            }

            for (int i = 0; i < mossa.length(); i++)
            {
                if (!(mossa[i] >= '0' && mossa[i] <= '9'))
                {
                    callTextToSpeech(string("In k: Presenza di caratteri non numerici, esco per farti correggere\n"));
                    exit(1);
                }
            }
            skill = atoi(mossa.c_str());
        }
        else if (myColor == 'C')
        {
            if (mossa.length() == 0 || mossa.empty())
            {
                callTextToSpeech(string("C:risulta non valido, esco per farti correggere\n"));
                exit(1);
            }
            allMovesToComputer = mossa;
        }
        else if (myColor == 'E')
        {
            stockfish_color = mossa;
            if (!isEngineRunning())
            {
                if (start())
                {
                    callTextToSpeech(string("attivato il computer a giocare\n"));
                    // occorre settare lo skill
                    if (skill >= 0)
                    {
                        setSkill(skill);
                    }
                }
            }
        }
        else if (myColor == 'P')
        {
            // rilevo l'ultimo valore di mossa
            numMove = atoi(mossa.c_str());
            msg = "Ultimo Numero mossa " + to_string(numMove) + "\n";
            printDebug(msg);
        }
        else if (myColor == 'F')
        {
            if (mossa.empty())
            {
                callTextToSpeech(string("Non ci sarà la registrazione\n"));
            }
            else
            {
                reg_to_file = true;
                nameFile = mossa;
                msg = "La registrazione prosegue su file " + nameFile + "\n";
                printf("%s", msg.c_str());
            }
        }

        else if (myColor == 'V')
        {
            if (mossa.empty() || !(mossa[0] == 'B' || mossa[0] == 'N'))
            {
                callTextToSpeech(string("V:risulta non valido, esco per farti correggere il file\n"));
                exit(1);
            }
            if (isPresentB)
            {
                callTextToSpeech(string("Warning: il record V: deve essere posizionato prima di quelli B: e N:\n"));
                printf("Esco per consentirti di modificare il file\n");
                exit(1);
            }
            if (mossa[0] == 'B')
            {
                puntoDiVista = "Bianco";
            }
            else
            {
                puntoDiVista = "Nero";
            }
            // printf("Hai il punto di vista %s\n", puntoDiVista.c_str());
        }
        else
        {
            isPresentB = true;
            setPiece(myColor, mossa);
        }
    }
    // posizione sicura per settare validi gli arrocchi
    // anche se ripetuta almeno 4 volte Entra solo in un if per volta.
    if (status == true)
    {
        viewValid();
    }
    return status;
}

void setPiece(char color, string mossa)
{
    string extendedColor = (color == 'B' ? "Bianco" : "Nero");
    string extendedColor_f = (color == 'B' ? "Bianca" : "Nera");

    // ROS1 Settaggio se necessario di bianco_arrocchi e nero_arrocchi (globali)
    if (mossa[0] == 'N' && mossa[1] == 'S' && color == 'B')
    {
        bianco_arrocchi[0] = 1;
        printf("Bianco_arrocchi[0] settato a 1\n");
        return;
    }
    if (mossa[0] == 'N' && mossa[1] == 'L' && color == 'B')
    {
        bianco_arrocchi[1] = 1;
        printf("Bianco_arrocchi[1] settato a 1\n");
        return;
    }
    // parte nera
    if (mossa[0] == 'N' && mossa[1] == 'S' && color == 'N')
    {
        nero_arrocchi[0] = 1;
        printf("Nero_arrocchi[0] settato a 1\n");
        return;
    }
    if (mossa[0] == 'N' && mossa[1] == 'L' && color == 'N')
    {
        nero_arrocchi[1] = 1;
        printf("Nero_arrocchi[1] settato a 1\n");
        return;
    }

    // segue il for

    if (mossa.length() < 3 || mossa.length() > 3)
    {

        // ROS1 - solo NC e NL sono ammessi
        if (mossa[0] == 'N' && (mossa[1] == 'C' || mossa[1] == 'L'))
        {

            // tutto ok
        }
        else
        {

            callTextToSpeech(string("Lunghezza della posizione diversa da quella necessaria\n"));
            printf("Esco per consentirti di verificare il file\n");
            exit(1);
        }
    }
    // pre-elaborazione maiuscole e minuscole e numeri
    string posizione = mossa;

    posizione[0] = toupper(mossa[0]);
    posizione[1] = tolower(mossa[1]);
    posizione[2] = mossa[2];
    if (!(posizione[0] == 'A' || posizione[0] == 'T' || posizione[0] == 'C' || posizione[0] == 'D' || posizione[0] == 'R' || posizione[0] == 'P'))
    {
        callTextToSpeech(string("qualche pezzo ha una lettera errata\n"));
        cout << "Esco per consentirti i controlli sul file\n";
        exit(1);
    }

    if (!(posizione[1] >= 'a' && posizione[1] <= 'h'))
    {
        callTextToSpeech(string("qualche posizione non ha un carattere ammissibile dopo la lettera del pezzo\n"));
        cout << "Esco per consentirti i controlli sul file\n";
        exit(1);
    }

    if (posizione[1] > 'h' || posizione[1] < 'a')
    {
        callTextToSpeech(string("le lettere dopo la lettera del pezzo devono essere comprese tra a ed h\n"));
        cout << "Esco per consentirti i controlli sul file\n";
        exit(1);
    }

    if (!(posizione[2] >= '0' && posizione[2] <= '9'))
    {
        callTextToSpeech(string("Non esiste un numerico  dopo la lettera della casella\n"));
        cout << "Esco per consentirti i controlli sul file\n";
        exit(1);
    }
    //  interpretazione pezzi
    string nomePezzo;
    string nameSquare = posizione.substr(1);
    ChessSquare casella = ChessSquare(nameSquare.c_str());
    char shortColor = (color == 'N' ? 'B' : 'W');
    if (posizione[0] == 'P')
    {
        nomePezzo = "Pedone" + extendedColor;
        Pawn pedone = Pawn(nomePezzo.c_str(), shortColor);
        casella.setChessPiece(pedone);
    }
    if (posizione[0] == 'T')
    {
        nomePezzo = "Torre" + extendedColor_f;
        Rook torre = Rook(nomePezzo.c_str(), shortColor);
        casella.setChessPiece(torre);
    }
    if (posizione[0] == 'C')
    {
        nomePezzo = "Cavallo" + extendedColor;
        Knight cavallo = Knight(nomePezzo.c_str(), shortColor);
        casella.setChessPiece(cavallo);
    }
    if (posizione[0] == 'A')
    {
        nomePezzo = "Alfiere" + extendedColor;
        Bishop alfiere = Bishop(nomePezzo.c_str(), shortColor);
        casella.setChessPiece(alfiere);
    }
    if (posizione[0] == 'D')
    {
        nomePezzo = "Donna" + extendedColor_f;
        Queen donna = Queen(nomePezzo.c_str(), shortColor);
        casella.setChessPiece(donna);
    }
    if (posizione[0] == 'R')
    {
        nomePezzo = "Re" + extendedColor;
        King re = King(nomePezzo.c_str(), shortColor);
        casella.setChessPiece(re);
    }
    casella.setBusySquare(true);
    ChessUtility utility;
    vector<int> Index = utility.getIndexPuntoDiVista(nameSquare, puntoDiVista);
    int i = Index.front();
    int j = Index.back();
    chessBoard[i][j] = casella;
    return;
}

void init()
{
    string com;
    int cont = 1;
    com.clear();
    cleanBoard();
    isPresentB = false;

    while (cont < 8)
    {
        if (cont == 1)
        {
            com = "T:...\n";
        }
        if (cont == 2)
        {
            if (puntoDiVista == "Bianco")
            {
                com = "V:B;";
            }
            else
            {
                com = "V:N;";
            }
        }
        if (cont == 3)
        {
            com = "B:Pa2;Pb2;Pc2;Pd2;Pe2;Pf2;Pg2;Ph2;Ta1;Cb1;Ac1;Dd1;Re1;Af1;Cg1;Th1;";
        }

        if (cont == 4)
        {
            com = "N:Pa7;Pb7;Pc7;Pd7;Pe7;Pf7;Pg7;Ph7;Ta8;Cb8;Ac8;Dd8;Re8;Af8;Cg8;Th8;";
        }
        if (cont == 5)
        {
            com = "M:B;";
        }

        if (cont == 6)
        {
            com = "P:0;";
        }
        if (cont == 7)
        {
            com = "F:";
        }
        cont++;
        if (setPieceIntoSquare(com) == false)
        {
            callTextToSpeech(string("Problemi nella predisposizione della scacchiera!\n"));
            exit(1);
        }
    }
    return;
}

void cleanBoard()
{

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            chessBoard[i][j].setBusySquare(false);
        }
    }
    return;
}

void Sospendi(bool cambioColore)
{

    // permette di sospendere una partita
    // salvando i dati su file
    // in modo da poterla riprendere in seguito
    string nomeDelFile;
    char lettera = 'Z';
    string path = "..//sospensioni//";

    if (!cambioColore)
    {
        int len = 50;
        while (len > 35)
        {
            printf("Dati da inserire\n");
            printf("File : ");
            Sleep(1500);
            callTextToSpeech(string("Inserisci nome del file di sospensione senza estensione\n"));

            cin >> nomeDelFile;
            cin.ignore();
            len = nomeDelFile.length();
            if (len > 35)
            {
                callTextToSpeech(string("max lunghezza del file 30 caratteri\n"));
                len = 50;
            }
            size_t ext = nomeDelFile.find('.');
            if (ext != std::string::npos)
            {
                printf("Non devi mettere la estensione .txt nel nome del file, ripeti il nome\n");
                len = 50;
            }
        }
        path.append(nomeDelFile);
        nomeDelFile = path;
        nomeDelFile.append(".txt");
        if (!stockfish_color.empty())
        {
            evalPosition();
        }
    }
    else
    {
        nomeDelFile = "..//sospensioni//cambiocolore.txt";
    }
    ofstream file_output(nomeDelFile);
    if (file_output.is_open())
    {

        // scandisce la scacchiera con 2 cicli di 2 for per pezzi bianchi e poi pezzi neri e predispone i record
        if (cambioColore)
        {
            file_output << "T:cambio punto di vista in corso\n";
        }
        else
        {
            file_output << "T: Ripresa file in corso" << endl;
        }
        string com = "V:";
        com.push_back(puntoDiVista[0]);
        file_output << com << endl;
        com.clear();

        // primo ciclo per i bianchi

        com.append("B:");
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (chessBoard[i][j].getBusySquare() == true && chessBoard[i][j].getChessPiece().getColorPiece() == 'W')
                {
                    lettera = chessBoard[i][j].getChessPiece().getNamePiece()[0];
                    com.push_back(lettera);
                    com.append(chessBoard[i][j].getSquareName());
                    com.append(";");
                }
            }
        }
        file_output << com << endl;
        // ripeto per i neri
        com.clear();
        // ciclo sui neri
        com.append("N:");
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (chessBoard[i][j].getBusySquare() == true && chessBoard[i][j].getChessPiece().getColorPiece() == 'B')
                {
                    lettera = chessBoard[i][j].getChessPiece().getNamePiece()[0];
                    com.push_back(lettera);
                    com.append(chessBoard[i][j].getSquareName());
                    com.append(";");
                }
            }
        }
        file_output << com << endl;

        // i record M:,  P:,F:,L:,E:,C:,Z:
        com.clear();
        com.append("M:");
        char color;
        color = (startColor == 0 ? 'B' : 'N');
        com = "M:";
        com.push_back(color);
        file_output << com << endl;
        com.clear();
        if (color == 'P')
        {

            // numMove--;
            com = "P:" + to_string(numMove);
        }
        else
        {
            com = "P:" + to_string(numMove);
        }
        file_output << com << endl;
        com.clear();

        if (isEngineRunning())
        {
            com = "L:" + to_string(level);
            file_output << com << endl;

            com = "K:" + to_string(skill);
            file_output << com << endl;

            com.clear();
            com = "C:" + allMovesToComputer;
            file_output << com << endl;
            com.clear();
            com = "E:" + stockfish_color;
            file_output << com << endl;
            com.clear();
        }

        if (reg_to_file)
        {
            com = "F:" + nameFile;
        }
        else
        {
            com = "F:";
        }

        file_output << com << endl;

        // gestione enpasssant
        if (EnP)
        {
            com.clear();
            if (pawnEnP.size() == 2)
            {
                com = "U:" + pawnEnP.front() + pawnEnP.back() + casellaDaCatturare + ";";
            }
            else
            {
                com = "U:" + pawnEnP.back() + casellaDaCatturare + ";";
            }
            file_output << com << endl;
        }
        file_output.close();
        printf("operazione completata, puoi proseguire con altre funzionalità\n");
    }
    else
    {
        cerr << "Impossibile aprire il file\n"
             << endl;
    }

    return;
}

void viewValid()
{
    // set e verifica
    //  problema settaggio arrocco valido
    // ROS1 Deve controllare bianco_arrocchi e nero_arrocchi

    ChessUtility utility;
    string appo = "a8";
    vector<int> Ind_a8 = utility.getIndexPuntoDiVista(appo, puntoDiVista);
    appo = "h8";
    vector<int> Ind_h8 = utility.getIndexPuntoDiVista(appo, puntoDiVista);
    appo = "a1";
    vector<int> Ind_a1 = utility.getIndexPuntoDiVista(appo, puntoDiVista);
    appo = "h1";
    vector<int> Ind_h1 = utility.getIndexPuntoDiVista(appo, puntoDiVista);

    int i_a8 = Ind_a8.front();
    int j_a8 = Ind_a8.back();
    int i_h8 = Ind_h8.front();
    int j_h8 = Ind_h8.back();
    int i_a1 = Ind_a1.front();
    int j_a1 = Ind_a1.back();
    int i_h1 = Ind_h1.front();
    int j_h1 = Ind_h1.back();
    // mi serve per i 2 re anche e1 ed e8

    appo = "e8";
    vector<int> Ind_e8 = utility.getIndexPuntoDiVista(appo, puntoDiVista);
    appo = "e1";
    vector<int> Ind_e1 = utility.getIndexPuntoDiVista(appo, puntoDiVista);
    int i_e1 = Ind_e1.front();
    int j_e1 = Ind_e1.back();
    int i_e8 = Ind_e8.front();
    int j_e8 = Ind_e8.back();

    // verifichiamo se esistono sulla scacchieracome coppia re+torre

    if (chessBoard[i_a8][j_a8].getBusySquare() == true && chessBoard[i_a8][j_a8].getChessPiece().getTypePiece() == ROOK && chessBoard[i_a8][j_a8].getChessPiece().getColorPiece() == 'B')
    {
        if (chessBoard[i_e8][j_e8].getBusySquare() == true && chessBoard[i_e8][j_e8].getChessPiece().getTypePiece() == KING && chessBoard[i_e8][j_e8].getChessPiece().getColorPiece() == 'B')
        {
            chessBoard[i_a8][j_a8].setValid000(true);
        }
    }
    // h8
    if (chessBoard[i_h8][j_h8].getBusySquare() == true && chessBoard[i_h8][j_h8].getChessPiece().getTypePiece() == ROOK && chessBoard[i_h8][j_h8].getChessPiece().getColorPiece() == 'B')
    {
        if (chessBoard[i_e8][j_e8].getBusySquare() == true && chessBoard[i_e8][j_e8].getChessPiece().getTypePiece() == KING && chessBoard[i_e8][j_e8].getChessPiece().getColorPiece() == 'B')
        {
            chessBoard[i_h8][j_h8].setValid00(true);
        }
    }
    // a1
    if (chessBoard[i_a1][j_a1].getBusySquare() == true && chessBoard[i_a1][j_a1].getChessPiece().getTypePiece() == ROOK && chessBoard[i_a1][j_a1].getChessPiece().getColorPiece() == 'W')
    {
        if (chessBoard[i_e1][j_e1].getBusySquare() == true && chessBoard[i_e1][j_e1].getChessPiece().getTypePiece() == KING && chessBoard[i_e1][j_e1].getChessPiece().getColorPiece() == 'W')
        {
            chessBoard[i_a1][j_a1].setValid000(true);
        }
    }

    // h1
    if (chessBoard[i_h1][j_h1].getBusySquare() == true && chessBoard[i_h1][j_h1].getChessPiece().getTypePiece() == ROOK && chessBoard[i_h1][j_h1].getChessPiece().getColorPiece() == 'W')
    {
        if (chessBoard[i_e1][j_e1].getBusySquare() == true && chessBoard[i_e1][j_e1].getChessPiece().getTypePiece() == KING && chessBoard[i_e1][j_e1].getChessPiece().getColorPiece() == 'W')
        {
            chessBoard[i_h1][j_h1].setValid00(true);
        }
    }
    // printf("valori di bianco_arrocchi: %d %d\n", bianco_arrocchi[0], bianco_arrocchi[1]);
    // printf("valori di nero_arrocchi: %d %d\n", nero_arrocchi[0], nero_arrocchi[1]);
    if (bianco_arrocchi[0] == 1)
    {
        chessBoard[i_h1][j_h1].setValid00(false);
    }
    if (bianco_arrocchi[1] == 1)
    {
        chessBoard[i_a1][j_a1].setValid000(false);
    }

    if (nero_arrocchi[0] == 1)
    {
        chessBoard[i_h8][j_h8].setValid00(false);
    }

    if (nero_arrocchi[1] == 1)
    {
        chessBoard[i_a8][j_a8].setValid000(false);
    }
    return;
}
void checkU(string mossa)
{

    int len = mossa.length();

    // controllo lunghezze
    if (len != 4 && len != 6)
    {
        callTextToSpeech(string("Il descrittore U: non ha la lunghezza adeguata. Esco per farti verificare\n"));
        exit(1);
    }
    // isalnum
    for (int i = 0; i < mossa.length(); i++)
    {
        if (!isalnum(mossa[i]))
        {
            callTextToSpeech(string("Presenza di caratteri non alfanumerici\n"));
            exit(1);
        }
    }

    if (len == 4)
    {

        if (mossa[1] != mossa[3])
        {
            callTextToSpeech(string("U:Case non sulla stessa riga. Esco per farti controllare\n"));
            exit(1);
        }
    }

    if (len == 6)
    {
        if (mossa[1] != mossa[3] && mossa[1] != mossa[5])
        {
            callTextToSpeech(string("U: Case non sulla stessa riga . Esco per farti controllare\n"));
            exit(1);
        }

        // controllo pedone da prendere
        if (!(mossa[4] < mossa[2] && mossa[4] > mossa[0]))
        {
            callTextToSpeech(string("U: Casa del pedone da catturare non valida in quella posizione. Esco per farti controllare\n"));
            exit(1);
        }
    }
    // verifico se esistono i pedoni
    string casa1 = mossa.substr(0, 2);
    string casa2 = mossa.substr(2, 2);

    string casa3;
    if (len == 6)
    {
        casa3 = mossa.substr(4, 2);
    }
    ChessUtility utility;
    vector<int> casa1Ind = utility.getIndexPuntoDiVista(casa1, puntoDiVista);
    vector<int> casa2Ind = utility.getIndexPuntoDiVista(casa2, puntoDiVista);
    if (!(chessBoard[casa1Ind.front()][casa1Ind.back()].getBusySquare() == true && chessBoard[casa1Ind.front()][casa1Ind.back()].getChessPiece().getTypePiece() == PAWN && chessBoard[casa2Ind.front()][casa2Ind.back()].getBusySquare() == true && chessBoard[casa2Ind.front()][casa2Ind.back()].getChessPiece().getTypePiece() == PAWN))
    {
        callTextToSpeech(string("U: alcuni pedoni non esistono per enpassant. Esco per farti controllare\n"));
        exit(1);
    }
    if (len == 6)
    {
        vector<int> casa3Ind = utility.getIndexPuntoDiVista(casa3, puntoDiVista);
        if (!(chessBoard[casa3Ind.front()][casa3Ind.back()].getBusySquare() == true && chessBoard[casa3Ind.front()][casa3Ind.back()].getChessPiece().getTypePiece() == PAWN))
        {
            callTextToSpeech(string("U: alcuni pedoni non esistono per enpassant. Esco per farti controllare\n"));
            exit(1);
        }
    }
    return;
}