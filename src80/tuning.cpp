/*

Questo programmaconsente di leggere il file di configurazione
chessParam.cnf per il massimo numero di threads e la dimensione
della hash table del chess engine
*/
#include <iostream>
#include <fstream>
#include "engine.h"
#include "globale.h"
#include "proto.h"

using namespace std;
// lettura chessParam.cnf
void readChessParam()
{

        /*
        maxThreads al massimo è da porre  al  numero  di core disponibili.
        maxHashTable è un intero espresso in Mega byte. Si consiglia a max la metà memoria libera disponibile.
        MultiPV è un intero per dire all'engine di restituire le prime 'multiPV' linee principali migliori per ogni analisi

*/

        ifstream configFile("chessparam.cnf");

        if (configFile.is_open())
        {
                string msg;
                string nome = "..//logdir//aiuto.txt";

                configFile >> maxThreads;
                msg.clear();
                msg = "maxTreads=" + to_string(maxThreads) + "\n";
                WriteGameToFile(nome, msg, false);

                configFile >> maxHashTable;
                msg.clear();
                msg = "maxHashTable=" + to_string(maxHashTable) + "\n";
                WriteGameToFile(nome, msg, false);

                configFile >> multiPV;
                msg.clear();
                msg = "multiPV=" + to_string(multiPV) + "\n";
                WriteGameToFile(nome, msg, false);

                configFile.close();
                if (maxThreads > 0)
                {
                        // cout << "Settato maxThreads a " << maxThreads << endl;
                        // cout << "Settato maxHashTable a " << maxHashTable << endl;
                }

                if (multiPV > 0)
                {
                        // cout << "Settato MultiPV a " << multiPV << endl;
                }
        }
        else
        {
                cout << "File chessparam.cnf non trovato, setting non eseguito" << endl;
        }
        return;
}