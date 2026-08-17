

Scacchi-it è un progetto italiano, sviluppato da Rosario Turco, una piattaforma online e offline gratuita. 
E' nato per realizzare un gioco adatto e fruibile dai non vedenti, che devono principalmente ascoltare per  giocare, attraverso uno screen reader come NVDA (open source). 
Tuttavia si è introdotta anche una grafica riproducente la scacchiera, in modo che i familiari possono aiutare e giocare col non vedente.

Il gioco sollecita tutte le componenti cognitive della persona, migliorando l'utilizzo del ragionamento e calcolo delle varie alternative, mantenendo attiveme moria visiva e astratta,

Scacchi-it è orientato a:
- non vedenti e vedenti
- Windows 10 o superiori 
- lingua italiana 
E' costituito da tre parti:
- motore grafico di navigazione
- motore di controllo regole
- motore di gioco

Il motore di gioco (chess engine) con cui le prime due parti sono testatte
è stockfish, scaricabile da github e distribuito liberamente. 
Nel programma si da la possibilità di configurare anche altre eventuali
versioni migliorative di stockfish.

I primi due motori consentono l'utilizzo del chess engine per:
- giocare contro il computer
- giocare da soli sia bianco che nero
- registrare le partite non giocate col computer
- risolvere dei problemi da soli
- risolvere un problema col computer
- scegliere il lato da cui giocare
- sospendere una partita, col computer o meno
- riprendere una partita
- conoscere la valutazione della posizione durante la sospensione della partita col computer
- ottenere alla fine della sospensione del cumputer una analisi di almeno le prime 'multiPV' principali varianti che si possono intraprendere
- caricare problemi scritti da se stessi con la rot notation
- fare undo delle mosse
- forzare l'apertura desiderata e farla giocare al computer
- giocare online su reti diverse come Internet
- validare e incollare una FEN a Scacchi-it
E' possibile fare anche un minimo di tuning del chess engine per sfruttare al meglio le risorse del proprioo computer (Vedi tuning.txt). 
Infine si può usare un orologio per allenarsi col computer, o con amici ad esempio, allenarsi per partite reali lampo o di torneo


Per gli sviluppatori molte altre informazioni sono nel file per-gli-sviluppatori.

Il manuale è scacchigr.txt. E' stato scelto di farlo come file di testo e non pdf, per il semplice motivo che un file di testo è la migliore accessibilità possibile su browser per un non vedente.
