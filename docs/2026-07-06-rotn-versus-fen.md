---
title: "Scacchi: Cosa è la rotn"
description: "blog di Rosario Turco con downloads di giochi gratuiti per ciechi"
keywords:
  [
    "Articoli per scacchi ",
    "giochi",
    "gratuiti",
    "gratis",
    "non vedenti",
    "ciechi",
    "ipovedenti",
  ]
categories: Blog
---

# **Cosa è la ROTN**

di Rosario Turco, ingegnere

In questo articolo è indagata una specifica di accessibilità digitale per chi sviluppa software scacchistico compatibile con screen reader (come NVDA e simili), ma che consente di utilizzare il software anche ai vedenti, in linea con un concetto a me caro di integrazione dei due mondi: non vedenti e vedenti (NV&V).
Il gioco diventa un momento sociale di integrazione e di possibilità alla pari tra vedente e non vedente.

Non solo vengono indagate caratteristiche di accessibilità digitale, come inclusione e autonomia dei non vedenti, ma anche tutte quelle caratteristiche utili allo sviluppo di unapplicativo di scacchi o una piattaforma di scacchi.

Lo studio ha portato alla redazione, quindi, di una specifica ROTN e allo sviluppo della piattaforma Scacchi-it.

La specifica dichiara un metodo semplice e intuitivo per superare facilmente vecchie problematiche, che gli scacchi da sempre si portano dietro: una immagine non è fruibile da un non vedente su Internet o su un ebook e la fen è poco intuitiva per un umano, adatta più a un computer e risalente al secolo scorso.

L'obiettivo della ROTN è di avere una posizione parlante con sintesi vocale e contenente descrizioni come il tema della posizione e consigli anche tecnici.

Oggi uno screen reader leggendo la fen vi spara un insieme di caratteri velocemente, senza darvi il tempo di "vedere" attraverso il canale uditivo.

La rotn, in realtà , è una opportunità aggiuntiva alla fen, per sfruttare altri canali tecnologici e dare voce alla FEN.

Non fare l'errore di credere che la ROTN sostituisca qualche standard: non sostituisce nè lo standard pgn, nè la FEN. Piuttosto si aggiunge come utile strumento ulteriore a vantaggio dei non vedenti e degli screen reader.
Scacchi-it stesso dispone del comando incolla-FEN.

La ROTN è costituita da un file testuale fatto di descrittori seguiti da valori. Tutti i campi descrittori sono obbligatori e con posizione e ordine fisso.

_Esempio ROTN_
T:Matto in una mossa
V:N;
B:Rh8;
N:Rf7;Dg1;
M:N
P:34;

- T: descrive la posizione (tema) che verrà letta anche con sintesi vocale, con possibili consigli tecnici
- V: descrive da che punto di vista o lato della scacchiera si vuole che il programma carichi la posizione (Bianco o Nero usando B; o N;)
- B: descrive l'elenco separato da punti e virgole dei pezzi con notazione algebrica italiana e la situazione degli arrocchi se necessario (es: Ag5;Re2;)
- N: descrive l'insieme dei pezzi neri come sopra
- M: descrive a chi tocca la mossa (Bianco o Nero usando B o N)
- P: descrive il numero di mossa della posizione
  La regola "ogni descrittore una riga" non è per un fatto informatico ma per la semplice psicologia che un facile copia e incolla aumenta l'autostima del non vedente.

Descrittore opzionale U: se presente indica la lista di case, non separate da ';', dei pedoni che possono fare una cattura enpassant e la casa del pedone sotto attacco es. U:c4b4; altro esempio U:d4f4e4;
Il descrittore serve negli automatismi di sospensione di una partita e la sua ripresa. In posizioni scritte a mano, è responsabilità di chi scrive osservare l'ordine esatto e la sintassi del comando.
Esso rappresenta un reale enpassant, quindi U è presente solo se accanto al pedone sotto attacco ci sono affiancati uno o due pedoni avversari.
Consultare il Manuale.

Descrittore opzionale Z: indica il numero di semimosse presenti nella fen (campo 5)

Descrittori riservati ai produttori software:F, K, L, C, E, tutti utili nelle fasi di sospensione e ripresa di una partita. In particolare per un portale, la sospensione e ripresa che comportano file presenti sul server, si può usare una strategia di permanenza per un breve periodo di giorni (es. 2 giorni dopo i quali non è più possibile la ripresa della partita).

La sintesi vocale nell'applicativo deve leggere: T, M, P, U, Z.
Inoltre l'applicativo ruota la scacchiera secondo V.
Dalla ROTN l'applicativo Scacchi-it apprende gli arrocchi validi (quelli non validi sono marcati con NS=No Short Castle e NL=No Long Castle), il numero di semimosse per la regola delle 50 mosse , oltre all'enpassant.

# **Vantaggi della ROTN**

La ROTN unisce i vantaggi di una immagine con quelli della notazione algebrica più sintetica rispetto alla FEN.
Infatti aggiunge il testo (T:) che valorizza il momento artistico e clou della posizione, inoltre aggiunge il punto di vista da cui guardare e giocare la partita. L'applicativo, difatti, carica la posizione dal punto di vista dichiarato nella ROTN.
Un applicativo che rispetta la ROTN può attraverso essa: sospendere una partita, riprendere una partita, ruotare la scacchiera, caricare un problema scritto con Notepad, creato da noi o copiato da internet o da un e-book.

La sinteticità della notazione algebrica rende più rapidamente intuibile la posizione.

Scacchi-it è stato il software creato da me e centrato sulla ROTN, per un costo di 8 mesi/uomo per ottenere circa 35 funzionalità tra offline e online.
Per comprendere meglio i limiti di una fen esaminiamo come è fatta.

fu ideata da Steven J. Edwards estendendo il Forsyth's system ideato per l'uso del calcolatore. La sua forma,inizialmente, era dovuta al tentativo di risparmiare la quantità di memoria nel registrare delle informazioni.
Difatti i componenti di una stringa FEN (6 parti) sono:

1. Posizione dei pezzi: Fila per fila (dall'8a alla 1a), lettere minuscole per il nero, maiuscole per il bianco, numeri per le case vuote.
2. Turno: w (bianco) o b (nero).
3. Arrocco: Kqkq (disponibilità arrocco lato re/donna) oppure valorizzati ognuno con -.
4. Cattura en passant: Casa in cui un pedone avversario potrebbe andare dopo la cattura , o -.
   C'è da aggiungere che, per standard, all'atto della fotografia della posizione, essa contiene il valore della casa di cattura dovuta allo spostamento di due passi di un pedone, anche se non esiste nessun pedone avversario che possa usufruire dell'enpassant.
   Ad esempio se si muove un pedone da b2 a b4 la casa indicata è b3, ovvero la casa di cattura in cui si metterebbe un pedone avversario in caso di cattura enpassant.
5. Semi-mosse: Numero per la regola delle 50 mosse.
6. Mosse totali: Numero di mosse della partita.

   Per la semimossa nella fen (quinto campo della stringa), come recita Wikipedia, è un contatore del numero di mezzi turni (mosse del Bianco
   o del Nero) effettuati dall'ultima cattura o spinta di pedone. Viene utilizzato per determinare la regola delle 50 mosse: se arriva a 100 (50 mosse complete), la partita può essere dichiarata patta.

Esempi FEN:
• Posizione iniziale: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1.
• Dopo 1. e4 c5 2.Ch8f3:
rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2.

Ad esempio la fen seguente mostra i suoi limiti:
7K/5k2/8/8/8/8/8/6q1 B ---- - 0 34

Tutto questo per dire che il nero ha due pezzi: re e regina e il bianco solo il re, con la mossa al nero; Ma non dice cosa occorre fare e da che punto di vista mettersi per valutare meglio la posizione.
Nella ROTN troveremo semplicemente e in maniera chiara senza doversi fare i conti:

T:Matto in una mossa
V:N;
B:Rh8;
N:Rf7;Dg1;
M:N
P:34;

Come si nota la fen ha anche altre due barriere invisibili all'accessibilità:

- la lingua: K, k, Q, q, B, b etc.
- la non immediata comprensione: occorre farsi dei calcoli per capire la posizione dei pezzi

Scacchi-it , ideato per vedenti e non vedenti, nelle sospensioni si segna l'enpassant per comunicare la possibilità di esso alla ripresa, ma solo se esiste tale possibilità, diversamente dallo standard fen.

In un e-book per non vedenti se ci sono immagini, esse non sono fruibili dal non vedente; per cui, generalmente, viene descritta la posizione con la notazione algebrica, che ha il vantaggio di far comprendere la posizione dei pezzi rapidamente.
La ROTN fa un passo ulteriore, consentendo l'ascolto e l'immediato utilizzo su scacchiera e non solo.

Se in un e-book viene mostrata la ROTN, col copia e incolla si può rapidamente trasferirla in un file e caricarla in Scacchi-it. In tal modo si può ascoltare, con sintesi vocale, il testo e navigare sulla scacchiera esplorando la posizione, apprezzando meglio quanto descritto nel e-book.

La ROTN, inoltre, rende open data l'applicativo Scacchi-it in modo bidirezionale: consente di sospendere una partita e di riprenderla più tardi, come pure scrivere o copiare problemi e caricarli per la loro soluzione.
Per cui la giusta definizione della rot notation è che essa estende la notazione algebrica e rende open data un applicativo di scacchi, consentendo di interagire con esso in due direzioni e aggiungendo la sintesi vocale e i vantaggi di una immagine.

Infine la ROTN è, nel caso Scacchi-it, la lingua interna, di interscambio con tutti i formati esterni come laFEN o altri formati futuri.

un ultima osservazione: la vera forza di ROTN non è evidente. Permette di piazzare qualsiasi posizione in 3 secondi e testare: matto, stallo, arrocco, en passant e qualsiasi altra posizione complicata, senza dover giocare 40 mosse ogni volta. Un grosso guadagno di tempo per consolidare Scacchi-it.

Questo risulta un enorme vantaggio anche per gli istruttori di scacchi, che con basso sforzo cognitivo possono creare rapidamente posizioni o database di posizioni didattiche.

# **Specifica formalizzata per non vedenti**

Come sempre accade, chi ha delle necessità, realizza una specifica e un applicativo di scacchi (Scacchi-it) che ne dimostra i vantaggi in modo pratico.Un applicativo innovativo, per giocare offline e online, con stockfish, studiare le aperture con un Openings Trainer, poter fare lezioni con undo delle mosse e con una facile reinizializzazione della scacchiera.

Se siete interessati a scaricare Scacchi-it, il manuale e la specifica, utilizzare i link:

[scarica Scacchi-it](https://github.com/RedYouMan/redyouman.github.io/raw/main/_posts/repo/scacchiGr.7z)
[Manuale Scacchi-it](https://github.com/RedYouMan/redyouman.github.io/raw/main/_posts/tutorials/scacchiGr.txt)

La definizione della Specifica ufficiale ROTN completa, ISO 14977 è disponibile a:
[Leggi specifica](https://redyouman.github.io/blog/2026/07/02/specifica-rotn.html

[Dare voce alla fen con rotn](https://redyouman.github.io/blog/2026/07/18/estensioni-browser.html)
