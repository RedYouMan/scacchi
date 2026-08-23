# **Scacchi-it**

Scacchi-it è un progetto italiano, sviluppato da Rosario Turco.
E' una piattaforma online e offline gratuita.
E' nato per realizzare un gioco adatto e fruibile dai non vedenti, che devono principalmente ascoltare per giocare, attraverso uno screen reader come NVDAo JASW.
Ha tuttavia anche una grafica riproducente la scacchierae i pezzi, in modo che gli amici ed i familiari possono giocare col non vedente.

Il gioco sollecita tutte le componenti cognitive della persona, migliorando l'utilizzo del ragionamento e calcolo delle varie alternative, mantenendo attive me moria visiva e astratta,

Adatto al gioco , lo studio, l'analisi e la didattica che coinvolge, in modo paritario e concreto sia vedenti e non vedenti, realizzando, con alto valore sociale ed etico, la inclusione digitale e l'autonomia con la giusta dignità dei non vedenti.
La possibilità di utilizzarlo anche offline e gratuitamente, estende democraticamente anche il suo utilizzo a famiglie meno abbienti e rendendo gli studiosi indipendenti da ulteriori problemi di latenza della rete.
Particolarmente utile anche a scuole che hanno problemi di utilizzo internet.

Scacchi-it permette anche di giocare online con amici, parenti su stessa rete o su vpn Hamachi gratuitamente.

Scacchi-it è, quindi, uno strumento tiflologico orientato a:

- non vedenti e vedenti
- Windows 10 o superiori
- lingua italiana
- studio, gioco, lezioni didattiche

## **Parti costitutive**

E' costituito da almeno tre parti:

- motore grafico di navigazione in SFML (Simple & Fast Multimedia Library)
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
- ottenere alla fine della sospensione del computer una analisi di almeno le prime 'multiPV' principali varianti che si possono intraprendere
- caricare problemi scritti da se stessi con la rot notation
- fare undo delle mosse(didattica)
- forzare l'apertura desiderata e farla giocare al computer
- giocare online su stessa rete o su vpn Hamachi per reti diverse come Internet
- validare e incollare una FEN a Scacchi-it
  - studiare le aperture con un Opennings Trainer e un coach
    E' possibile fare anche un minimo di tuning del chess engine per sfruttare al meglio le risorse del proprioo computer (Vedi tuning.txt).
    Infine si può usare un orologio per allenarsi col computer, o con amici ad esempio, allenarsi per partite reali lampo o di torneo

Il manuale è scacchigr.txt. E' stato scelto di farlo come file di testo e non pdf, per il semplice motivo che un file di testo è la migliore accessibilità possibile su browser per un non vedente.

## **Conclusioni**

Ecco cosa afferma sul progetto la AI di Google .

Valore Sociale ed Etico
Inclusione reale: Permette a persone vedenti e non vedenti di confrontarsi sullo stesso piano grazie anotazioni e interfacce accessibili.Risultati correlati
Democratizzazione del sapere: Essendo gratuito e offline, azzera le barriere economiche e di connettività, garantendo il diritto al gioco e allo svago.
Autonomia: Offre uno strumento di svago e studio autogestito per chi vive situazioni di disabilità sensoriale.Risultati correlati
Reputazione e Autorevolezza
Open data e condivisione: L'uso di notazioni testuali trasparenti (come la rot notation) eleva la serietà di questi progetti a livello scientifico e open source.Risultati correlati
Progettazione partecipata: La nascita di software curati da programmatori ed esperti con disabilità visiva garantisce risposte tecniche reali ed evita soluzioni di facciata.
Valore Didattico
Apprendimento inclusivo: Ottimo per spiegare le regole, studiare aperture e risolvere problemi tattici senza dipendere da grafica complessa o barriere visive.Risultati correlati
Sviluppo cognitivo: Stimola la memoria di lavoro, il calcolo combinatorio e il pensiero astratto, competenze fondamentali soprattutto nel training visivo-spaziale e di memorizzazione alla cieca.
