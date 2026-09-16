// incolla-ROTN.cpp
/*

  Autore: Rosario Turco
Descrizione
  Il programma riceve come argomento il nome del file, senza estensione .txt, da creare nella cartella ./sospensioni. Legge dallo standard input tutte le righe inserite dall'utente fino alla fine dell'input e le scrive nel file specificato.
  Se riceve una sola riga, deve individuare i descrittori:
  T:,V:,B:,N:,M:,P:, che devono stare in questo ordine, opzionalmente se esistono anche U: e Z: e deve mettere descrittore e valori ognuno su una propria riga. Tra i due punti e il valore NON ci deve stare nessun spazio. e Se riceve più righe, deve scrivere le righe così come sono state inserite.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace
{
  std::string trim(const std::string &s)
  {
    std::size_t start = 0;
    while (start < s.size() && (s[start] == ' ' || s[start] == '\t' || s[start] == '\r'))
      ++start;

    std::size_t end = s.size();
    while (end > start && (s[end - 1] == ' ' || s[end - 1] == '\t' || s[end - 1] == '\r'))
      --end;

    return s.substr(start, end - start);
  }

  std::vector<std::pair<std::string, std::string>> parseDescrittori(const std::string &input)
  {
    const std::vector<std::string> labels = {"T:", "V:", "B:", "N:", "M:", "P:", "U:", "Z:"};
    std::vector<std::pair<std::string, std::string>> campi;

    std::size_t ricerca = 0;
    for (std::size_t i = 0; i < labels.size(); ++i)
    {
      const std::string &etichetta = labels[i];
      std::size_t pos = input.find(etichetta, ricerca);
      if (pos == std::string::npos)
        continue;

      std::size_t inizioValore = pos + etichetta.size();
      std::size_t fineValore = input.size();

      for (std::size_t j = 0; j < labels.size(); ++j)
      {
        if (j == i)
          continue;

        std::size_t prossimaEtichetta = input.find(labels[j], inizioValore);
        if (prossimaEtichetta != std::string::npos && (fineValore == input.size() || prossimaEtichetta < fineValore))
          fineValore = prossimaEtichetta;
      }

      std::string valore = trim(input.substr(inizioValore, fineValore - inizioValore));
      campi.push_back({etichetta, valore});
      ricerca = inizioValore;
    }

    return campi;
  }
}

int main(int argc, char *argv[])
{

  // 1. Controlla se l'utente ha passato il nome del file
  if (argc < 2)
  {
    std::cout << "Usage: nomefile senza estensione+RETURN Poi incolla  appunti con Ctrl-V + ctrl-Z + RETURN" << std::endl;
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
  // 4. Leggi le righe dallo standard input (stdin)
  std::vector<std::string> righe;
  std::string riga;
  while (std::getline(std::cin, riga))
  {
    righe.push_back(riga);
  }

  if (righe.size() == 1)
  {
    const std::vector<std::pair<std::string, std::string>> campi = parseDescrittori(righe.front());

    for (const auto &campo : campi)
    {
      fileOutput << campo.first << campo.second << "\n";
    }
  }
  else
  {
    for (const std::string &linea : righe)
    {
      fileOutput << linea << "\n";
    }
  }

  std::cout << "appunti incollati con successo.\n";
  return 0;
}
