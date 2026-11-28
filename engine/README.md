# Scacchi-it

Questo repository non include il motore Stockfish per motivi di licenza (GPLv3).

Nota : il runtime di Scacchi-it controlla l'esistenza di stockfish in cartella engine e sottocartelle, se al primo avvio non trovasse stockfish, l'installer che Scacchi-it dispone provvederà a scaricare la versione di default preferita.

Se l'installer fallisse, occorre installare stockfish manualmente.

### Installazione manuale di stockfish

1. Scarica Stockfish da: https://stockfishchess.org/download/
2. se hai un exe soltanto Metti il file .exe nella cartella: engine/
3. Apri il file di configurazione engine.cnf in cartella bin e inserisci il nome esatto del eseguibile che hai scaricato.

Se, invece, hai scaricato uno zip:
Se unzippi stockfish nella cartella engine ti creerà qui una ulteriore cartella stockfish all'interno della quale vi sarà l'eseguibile. In tal caso il file di configurazione engine.cnf nella bin ha già il path corretto, devi controllare solo il nome dell'eseguibile.

Al riavvio l'applicazione funzionerà.

Si raccomanda di provare il comando Ctrl Z per verificare se stockfish gioca. 
Fai Alt-Tab e rispondi alle domande (1 per dire che stockfish deve giocare col bianco e livello 10).
Appena stockfish gioca la sua mossa in bianco, quando sei sul nero puoi fare Ctrl Q avendo verificato che tutto è a posto.

Qualora stockfish scaricato non funzionasse controlla se il file di configurazione nella cartella bin è correttamente scritto.
 
Licenza Stockfish: GPLv3 - https://github.com/official-stockfish/Stockfish
