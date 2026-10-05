---
layout: post
title: "Quali sono i progressi che si possono fare con Scacchi-it? Non è un'app di primo livello da abbandonare. È un ecosistema che scala fino a Maestro"
description: "Scacchi-it non è un'app di primo livello da abbandonare dopo due mesi. È un ecosistema tiflologico italiano gratis e portable che scala fino a Maestro. Analisi neuroscientifica con teoria del carico cognitivo di Sweller, chunking, Flow, deliberate practice, metodo Woodpecker tiflologico con ROTN per non vedenti autodidatti e test Capablanca"
keywords: "Scacchi-it, ROTN, Readable Ordered Text Notation, scacchi non vedenti, tiflologia, metodo Woodpecker, test Capablanca, teoria carico cognitivo Sweller, chunking, Flow Csikszentmihalyi, deliberate practice Ericsson, downloader semantico"
categories: blog
---

### Quali sono i progressi che si possono fare con Scacchi-it? Non è un applicativo di primo livello da abbandonare. È un ecosistema che scala fino a Maestro. E lo dimostrano le neuroscienze.

di Rosario Turco

#### Il problema delle altre applicazioni

Chi ha provato a giocare a scacchi da non vedente con le applicazioni generaliste conosce bene la sensazione. Si apre una di queste applicazioni con lo screen reader NVDA o JAWS, si cerca di capire dove sono i pezzi e si viene immediatamente travolti. Una FEN di settanta caratteri letta tutta d'un fiato in cuffia, `rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1`, una esplorazione tattile continua sulla scacchiera fisica senza punti di riferimento, uno screen reader che legge frammenti web che cambiano ad ogni aggiornamento del sito. Il risultato è quasi sempre lo stesso: una curva di apprendimento ripidissima, frustrazione, ansia da prestazione e poi l'abbandono.

Le neuroscienze hanno un nome preciso per questo fenomeno. John Sweller, con la sua Cognitive Load Theory, lo chiama sovraccarico del carico cognitivo estraneo. Alan Baddeley, con il modello della Working Memory, ci ha spiegato che la memoria di lavoro ha una capacità limitatissima. Se tu occupi tutta la memoria per decodificare dove si trova un pezzo, non ti rimane più spazio per pensare alla strategia vera. Stai usando la testa per fare il lavoro che dovrebbe fare l'interfaccia. Ecco perché tante app per non vedenti vengono abbandonate dopo due mesi: non è colpa dell'utente, è colpa di un design che genera carico estraneo altissimo.

#### La soluzione Scacchi-it + ROTN

Scacchi-it nasce per invertire questo paradigma, e lo fa partendo da una intuizione semplice che viene dalla ricerca sui campioni. Il cervello dell'esperto non calcola più mosse una ad una, riconosce configurazioni. È il chunking di cui parlava George Miller nel suo famoso articolo sul numero magico sette, e poi William Chase e Herbert Simon studiando i maestri di scacchi. Un maestro non vede 32 pezzi sparsi, vede cinque o sei strutture note, le chunk. Per permettere questo chunking a chi non vede, serve però un formato che parli al cervello verbale e logico, non a un motore.

Qui entra la ROTN, Readable Ordered Text Notation. La ROTN è un formato open data che rappresenta un'immagine scacchistica in modalità testuale, con una specifica formale completa EBNF ISO 14977. Mentre la FEN è nata per i motori, la ROTN è nata per l'uomo. La differenza in cuffia è abissale. Altri in cuffia: `rnbqkbnr/pppppppp...` uguale rumore, uguale carico cognitivo estraneo. Scacchi-it in cuffia: `Mossa numero 1 Tocca al Bianco casa e2, Pedone bianco`. È solo il necessario in cuffia. Questo è il principio di Dual Coding di Allan Paivio: se offri la stessa informazione su più canali, tattile più audio più Braille, la traccia mnestica diventa più forte, più ridondata e più resistente all'oblio.

I vantaggi della ROTN, che spesso vengono elencati in modo solo tecnico, sono in realtà vantaggi cognitivi e didattici. La ROTN permette di importare ed esportare i dati in modo semplice, di sospendere e riprendere una partita in qualsiasi momento, di esportare posizioni dove si è avuto un bug, di interscambiare dati tra formati diversi, da FEN a ROTN, di svolgere i test più rapidamente anche automatizzandoli, di mettere su database didattici più rapidamente, di aggiungere alle immagini dei diagrammi nei libri ed ebook un formato per non vedenti semplice, dove col copia e incolla un non vedente può provare la posizione su una piattaforma come Scacchi-it senza doverla ricostruire pezzo per pezzo.

#### Test con benda e doppio canale

Durante lo sviluppo di Scacchi-it abbiamo usato un metodo che in neuroscienze si chiama external scaffolding cognitivo, di Clark e Chalmers. Lo sviluppatore bendato sente la necessità reale di gioco non vedente. Il collaboratore non informatico con la grafica SFML vede se il pezzo va davvero dalla casa di partenza alla casa di arrivo. C'è una separazione netta di ciò che si DICE da ciò che si MOSTRA. Se c'è un bug, lo intercetti subito, non lo scarichi sul giocatore. È un controllo di qualità che riduce il carico estraneo.

#### Posizionamento intelligente e2/e7

Un altro dettaglio che sembra piccolo ma cambia tutto è il posizionamento intelligente. Al cambio turno Scacchi-it non ti lascia perso sulla scacchiera a cercare con i cursori. Ti dice: ora sei in casa e7 se sei il Nero, e2 se sei il Bianco. Scacchi-it non fa correre coi cursori nel proprio schieramento. È un principio puro di Universal Design: porta l'utente dove deve agire, senza dargli il suggerimento su cosa fare, e lo posiziona sempre sulla stessa casa che diventa un'ancora stabile per la costruzione della sua mappa mentale.
La mappa mentale del giocatore non vedente deve essere la scacchiera, una matrice 8x8 e non una stringa lineare.
Per questo in Scacchi-it il non vedente naviga sulla scacchiera come farebbe con una scacchiera tattile. Questo aiuta alla creazione della mappa mentale e al riconoscimento dei pattern.

> Questa necessità di una matrice 8x8 e non di una stringa lineare è la stessa che emerge nelle partite alla cieca: nessun giocatore alla cieca memorizza una FEN come elenco, ma ricostruisce mentalmente la scacchiera con ancore stabili. La differenza è che nel gioco alla cieca la scacchiera è immaginata, in Scacchi-it è esplorata con i cursori, ma in entrambi i casi la mappa mentale resta spaziale, non verbale.

#### Perché Scacchi-it non lo abbandoni dopo 2 mesi? Perché scala

Qui sta la differenza tra un'app di primo livello e un ecosistema. Scacchi-it segue la teoria del Flow di Mihaly Csikszentmihalyi. Il Flow, lo stato di massima prestazione e soddisfazione, nasce quando la sfida è pari alla competenza. Se la sfida è troppo alta ti frustri, se è troppo bassa ti annoi. Scacchi-it ti offre 21 livelli Stockfish offline. Puoi iniziare dal livello 0 e salire di un livello alla volta, in modo graduale. Poi hai il CoachGame che analizza una partita intera che hai registrato e ti insegna dove hai sbagliato, e il CoachFEN che analizza una singola posizione. Hai l'Openings Trainer per costruire il tuo repertorio PGN, parcellizzando la complessità, che è alla base della neuroplasticità mirata di cui parla Ericsson con la deliberate practice. Ti scarichi i file PGN da un sito ufficiale di archivi PGN, quelli delle aperture desiderate, e cresci dentro Scacchi-it senza mai dover cambiare software.
Il blog fornisce un downloader semantico per non vedenti pensato proprio per questo:

[downloader semantico](https://redyouman.github.io/blog/2026/08/12/downloader4blinds.html)

#### La ROTN interviene solo quando serve

È importante capirlo: la ROTN non serve in live per spostarsi sulla scacchiera. In live usi i cursori e l'audio. La ROTN interviene in live in cuffia in molte situazioni chiave: quando devi caricare una posizione scacchistica presa da un libro, quando sospendi una partita e la riprendi il giorno dopo, quando metti su incolla-ROTN i dati presi da una estensione browser o da un ebook e li leggi prima di salvarli, se vuoi verificare il lavoro del simpleEditor con un Notepad. In genere una ROTN incollata oppure scritta dal simpleEditor è validata da Scacchi-it, quindi sai che è corretta. Si nota la differenza anche con incolla-FEN. Quando copi una FEN e la metti sullo strumento incolla-FEN non sempre sei sicuro di aver preso tutto, ma leggere in cuffia 70 caratteri per controllare è faticoso. La soluzione è stata la validazione per non mandare a Scacchi-it FEN non valide, sia perché non l'abbiamo prelevata correttamente dalla sorgente, sia perché ci potrebbe essere un errore alla sorgente stessa. Qualunque sia il tipo di caricamento, con incolla-FEN o incolla-ROTN, Scacchi-it ti dà l'informazione della casa di ogni Re sulla scacchiera, per orientarti meglio. Sono riferite in sintesi vocale sia il numero di mossa, il tratto, l'esistenza della possibilità di catturare un pedone con en passant, sia durante la partita che alla ripresa.

#### Metodo Woodpecker tiflologico con ROTN - per chi studia da solo.

In questo metodo la ROTN non viene letta in cuffia tutta d'un fiato per fissare il pattern. Il pattern non si fissa ascoltando un elenco.
Vengono letti in cuffia solo le posizioni dei re, per dare orientamento ai non vedenti.

Riprendiamo il metodo Woodpecker di Axel Smith e Hans Tikkanen e lo adattiamo in versione tiflologica con ROTN e orologio tiflologico. Gli istruttori ti danno 5 posizioni ROTN con stesso tema tattico e un file di testo di soluzioni per ogni file ROTN.
Gli istruttori o tu stesso metti i file in cartella problemi. Tu carichi con CTRL+P il primo problema ma non senti la ROTN in cuffia, e questo è voluto. Devi navigare sulla scacchiera per capire come sono posti i pezzi. Ti viene detta solo la posizione dei due Re, ed è l'informazione chiave per orientarti subito, senza carico estraneo.

Perché così? Perché il pattern scacchistico nel non vedente si fissa con esplorazione attiva, non con ascolto passivo. Se ti leggessi tutta la ROTN prima di rispondere, torneresti al rumore delle altre applicazioni. Il cervello esperto fa chunking spaziale, non memorizza un elenco verbale. La ROTN qui è il contenitore che permette all'istruttore di costruirti il database omogeneo, non è la lettura del test.

Come fare, passo passo, da solo:

Primo passo: creati un file di testo con la lista dei file ROTN, accanto ai quali potrai mettere dopo le risposte, una per ogni ciclo.

Secondo passo: apri Scacchi-it e imposta l'orologio tiflologico a 3 minuti x numero delle posizioni ROTN poste in cartella problemi. Hai messo 5 posizioni, imposti 15 minuti. È il tuo primo ciclo lento.
L'orologio lo usi considerando solo la parte del tempo del bianco. Questo significa che quando hai finito un test lo fai passare al nero, mentre se ricarichi un altro test lo rimandi poi sul bianco. Quindi ti basi solo sul tempo trascorso del bianco.
L'obiettivo è di farcela a rispondere a tutti i test nel tempo assegnato. Se non ce la facciamo, si ricomincia il ciclo daccapo.

Terzo passo: fai partire il test. Dall'elenco di file che ti sei creato, segui un ordine casuale e carica con CTRL+P un file su Scacchi-it. Senti solo dove sono i due Re. Ora esplora la scacchiera e ricostruisci il pattern con i cursori. Dai una risposta su ogni posizione con una sola mossa , quella risolutiva ed esci. Ti segni la mossa fatta sul file di testo .

Quarto passo: termini i 5 test. Solo alla fine confronti a mano ogni mossa risposta e registrata sul file di testo, con un elenco soluzioni che ti era stato dato dagli istruttori.

Quinto passo: resetti l'orologio e imposti un tempo ridotto pari a 3 minuti in meno. Quindi 12minuti per 5 posizioni. Rifai le stesse 5. Poi riduci a 9, 6, fino a che il tempo settato è di 3 minuti totali per tutti e 5. Stai trasformando System 2 lento in System 1 automatico di Kahneman. È deliberate practice di Ericsson. Quando arrivi a 3 minuti totali, quel tema non lo calcoli più, lo vedi.

#### Test Capablanca - proposta per la valutazione posizionale

Se il Woodpecker allena la tattica, il test Capablanca allena la strategia.

Prendi 10 ROTN di finali didattici di Capablanca dal tuo archivio. Mettili nella tua cartella problemi. Caricane una alla volta con CTRL+P . Senza muovere nessun pezzo, solo esplorando in cuffia, valuta: chi sta meglio e perché? Re più attivo, struttura pedonale, case deboli. Registra la tua valutazione. Poi fai il confronto con CoachFEN o fai giocare stockfish a livello 12.
Infine il metodo Scacchi-it: registra la partita con stockfish, poi valutala con il coachGame.

#### Conclusione neuro-psicologica

Scacchi-it segue la filosofia della scacchiera tattile: esplori la scacchiera e ti alleni per i tornei, ma trasforma una curva molto ripida e solo tattile in una curva graduale e multimodale, audio più tattile più Braille più digitale. Il carico estraneo passa da alto a minimo grazie a feedback immediato e ROTN ordinata. L'abbandono passa da alto a basso grazie a 21 livelli scalabili, multiplayer Hamachi alla pari vedenti e non vedenti, inclusione sociale. Permette addestramento autonomo a casa, didattica inclusiva a corsi FSI, didattica duale con facilità di passaggio da Scacchi-it a scacchiera tattile. Avvicina non vedenti agli scacchi e permette lettura facile di dispense con diagrammi e ROTN.

Non è un'app di primo livello. È un ecosistema tiflologico italiano, gratis, portable, con ROTN, che ti porta da principiante a Maestro senza farti cambiare software.

**Riferimenti bibliografici**

Sweller, J. (1988, 2011). Cognitive Load Theory.

Baddeley, A. (2000). Working Memory.

Miller, G. A. (1956). The Magical Number Seven. E Chase, W. G., & Simon, H. A. (1973). Perception in Chess.

Paivio, A. (1986). Dual Coding Theory.

Ericsson, K. A., Krampe, R. Th., Tesch-Römer, C. (1993). The Role of Deliberate Practice.

Csikszentmihalyi, M. (1990). Flow.

Clark, A., & Chalmers, D. (1998). The Extended Mind.

Kahneman, D. (2011). Thinking, Fast and Slow.

Smith, A., & Tikkanen, H. (2018). The Woodpecker Method.
