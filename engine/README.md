# Scacchi-it

Questo repository non include il motore Stockfish per motivi di licenza (GPLv3).

### Installazione del motore:

1. Scarica Stockfish da: https://stockfishchess.org/download/
2. Metti il file .exe nella cartella: engine/
3. Apri il file di configurazione engine.cnf in cartella bin e inserisci il nome esatto del eseguibile che hai scaricato.

Nota 1: Se unzippi stockfish nella cartella engineti creerà una ulteriore cartella stockfish all'interno della quale vi sarà l'eseguibile. In tal caso il file di configurazione engine.cnf nella bin ha già
il path corretto, devi controllare solo il nome dell'eseguibile.

Al riavvio l'applicazione funzionerà.

Nota 2: il runtime di Scacchi-it controlla l'esistenza di stockfish in cartella engine e sottocartelle, se al primo avvio non trovasse stockfish, l'installer che Scacchi-it dispone provvederà a scaricare la versione di default preferita.

Licenza Stockfish: GPLv3 - https://github.com/official-stockfish/Stockfish
