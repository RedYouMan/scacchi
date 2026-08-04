// mossa_analizzata.cpp

#include <iostream>
#include <string>
#include "mossa_analizzata.h"
// questa parte implementa solo il metodo set  di commento
// mentre costruttore, distruttore, set e get a causa della loro semplicità sono implementati direttamente nella dichiarazione della classe

void MossaAnalizzata::set_commento(const std::string &comment, char color)
{
    // Aggiungi il colore al commento
    if (color == 'b') // bianco
    {
        commento = "Bianco: " + comment;
    }
    else if (color == 'n') // nero
    {
        commento = "Nero: " + comment;
    }
    else
    {
        commento = comment; // Nessun colore specificato
    }
}
