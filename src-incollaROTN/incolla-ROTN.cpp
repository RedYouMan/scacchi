// incolla-ROTN.cpp
/*

  Autore: Rosario Turco
Descrizione
  Il programma riceve come argomento il nome del file, senza estensione .txt, da creare nella cartella ./sospensioni. Legge dallo standard input tutte le righe inserite dall'utente fino alla fine dell'input e le scrive nel file specificato.
*/
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char *argv[])
{
  // 1. Controlla se l'utente ha passato il nome del file
  if (argc < 2)
  {
    std::cout << "Usage: nomefile senza estensione+RETURN Poi appunti-da-incollare + ctrl-Z + RETURN" << std::endl;
    return 1;
  }

  // 2. Prendi il nome del file dagli argomenti
  std::string path = "./sospensioni/";
  std::string nomeFile = path + argv[1] + ".txt";

  // 3. Apri il file in scrittura
  std::ofstream fileOutput(nomeFile);
  if (!fileOutput)
  {
    std::cerr << "Errore: Impossibile creare il file " << nomeFile << "\n";
    return 1;
  }

  // 4. Leggi le righe dallo standard input (stdin) e scrivile nel file
  std::string riga;
  while (std::getline(std::cin, riga))
  {
    fileOutput << riga << "\n";
  }

  std::cout << "appunti incollati con successo.\n";
  return 0;
}
