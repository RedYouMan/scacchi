---
layout: post
title: "Ecosistema tiflologico Scacchi-it e ROTN: carico cognitivo, UD e UDL"
categories: blog
description: "Validità didattica di Scacchi-it e ROTN 1.9: come la notazione ordinata riduce il carico cognitivo, previene l'abbandono e realizza i principi di Universal Design e Universal Design for Learning."
tags:
  [
    Scacchi-it,
    ROTN,
    tiflologia,
    carico cognitivo,
    didattica scacchi,
    accessibilità,
    Zenodo,
    NVDA,
    Universal Design,
    UDL,
  ]
---

### Ecosistema tiflologico Scacchi-it basato sulla ROTN: note sulla validità didattica

L'ecosistema Scacchi-it, sviluppato da Rosario Turco, è un ambiente software per Windows 10 e 11, portable, gratuito, compatibile con screen reader NVDA e JAWS. È corredato da specifica formale ROTN - Readable Ordered Text Notation, attualmente alla versione 1.9, depositata con registrazione Zenodo.org e DOI, e documentata su README, repository GitHub, manuale e sezione fordevelopers. Il sito di riferimento tuttoscacchi è sottoposto a Google tramite sitemap e propone approfondimenti tematici.

L'interesse didattico dell'ecosistema emerge dall'incrocio di tre ambiti: scacchi, tiflologia e psicologia cognitiva.

### 1. Curva di apprendimento e churning

Nella didattica è noto che il carico cognitivo estraneo è una delle cause del churning, inteso come abbandono precoce. Nel contesto tiflologico tradizionale, l'apprendimento degli scacchi presenta una fase iniziale di forte rallentamento. Le cause sono legate all'esplorazione manuale continua della scacchiera e alla decodifica lineare tramite sintesi vocale di codifiche come la FEN.

L'ecosistema Scacchi-it interviene su questo punto con un'architettura nativa per l'accessibilità. La curva di apprendimento, che nell'approccio tradizionale tende ad essere esponenziale con alta frustrazione iniziale e stallo, tende a divenire più lineare e progressiva, mantenendo il giocatore in una condizione di flusso dove la sfida è proporzionata alle capacità.

### 2. La specifica ROTN e la teoria del carico cognitivo

La FEN (Forsyth-Edwards Notation) descrive la scacchiera riga per riga, dall'ottava alla prima traversa, indicando i vuoti con numeri. Per un utente vedente è una scansione visiva immediata. Per un utente che utilizza screen reader o display Braille, richiede la ricostruzione mentale delle coordinate e il conteggio degli spazi vuoti, con occupazione significativa della memoria di lavoro.

La ROTN riorganizza i dati in modo ordinato e semantico:

- elencazione orientata ai pezzi, con associazione diretta tra pezzo e coordinata;
- eliminazione del conteggio mentale dei vuoti;
- struttura leggibile linearmente e compatibile con display Braille.

Secondo la Cognitive Load Theory di Sweller, la memoria di lavoro ha capacità limitata. Si distinguono carico intrinseco, legato alla complessità della posizione, carico estraneo, generato dalla modalità di presentazione dell'informazione, e carico pertinente, dedicato alla creazione di schemi mentali. La ROTN agisce sul carico estraneo, riducendolo, e sposta lo sforzo cognitivo dalla decodifica sintattica alla valutazione strategica. I moduli coachFEN e coachGame, integrati nell'ecosistema, sfruttano questa struttura per fornire indicazioni testuali immediate.

### 3. Fattori di contrasto all'abbandono

L'analisi dei fattori di rischio evidenzia tre leve su cui l'ecosistema interviene:

_Isolamento e mancanza di avversari._ L'integrazione multiplayer tramite room dedicate e la disponibilità di 20 livelli Stockfish offline consentono autonomia di gioco e continuità di allenamento.

_Frustrazione nello studio teorico._ L'Openings Trainer basato su standard PGN e il simpleEditor per la gestione di database personali rendono accessibile lo studio delle aperture senza barriere di formato.

_Ansia da gestione del tempo._ L'orologio scacchistico tiflologico integrato permette l'allenamento specifico per il tempo di gara, riducendo lo stress da competizione.

Dal punto di vista neuroscientifico, lo switch sensoriale dal visivo al tattile sposta il carico dalla corteccia occipitale alle aree somatosensoriali e parietali. L'esplorazione aptica è sequenziale e richiede il mantenimento in memoria di lavoro di una mappa dinamica. Una notazione ordinata come ROTN riduce il costo di questa operazione. La letteratura riporta che nei giocatori esperti il riconoscimento dei pattern avviene in modo olistico; nei giocatori non vedenti tale funzione è vicariata da aree associative parietali che elaborano lo spazio geometrico.

### 4. Quantificazione del carico cognitivo: FEN vs ROTN 1.9

Per rendere confrontabile il carico estraneo, si prende come riferimento una posizione di mediogioco standard e la si misura su tre assi: lunghezza sintattica, operazioni di memoria di lavoro, resa su screen reader e display Braille.

_Esempio di posizione:_
Bianco: Re e1, Donna d1, Torri a1 h1, Alfieri c1 f1, Cavalli b1 g1, Pedoni a2 b2 c2 d4 e2 f2 g2 h2
Nero: Re e8, Donna d8, Torri a8 h8, Alfieri c8 f8, Cavalli b8 g8, Pedoni a7 b7 c7 d5 e7 f7 g7 h7

_4.1 Lunghezza e densità informativa_

- Stringa FEN del piazzamento: `r1bqkbnr/ppp1pppp/8/3p4/3P4/8/PPP1PPPP/RNBQKBNR` - 49 caratteri. Contiene numeri che indicano vuoti, barre di separazione traversa, alternanza lettere maiuscole/minuscole.
- Stringa ROTN 1.9 equivalente: elenco orientato ai pezzi con coordinate esplicite. Esempio struttura: `W:Ra1,Rh1,Nb1,Ng1,Bc1,Bf1,Qd1,Ke1,Pa2,Pb2,Pc2,Pd4,Pe2,Pf2,Pg2,Ph2 | B:Ra8,Rh8,Nb8,Ng8,Bc8,Bf8,Qd8,Ke8,Pa7,Pb7,Pc7,Pd5,Pe7,Pf7,Pg7,Ph7` - circa 110 caratteri, ma a entropia zero per i vuoti. Non richiede decodifica dei numeri.

Il dato rilevante non è la lunghezza assoluta, ma il rapporto segnale/rumore. In FEN, circa il 25-30% dei caratteri è rumore sintattico (numeri di vuoti e slash). In ROTN il rumore è 0%: ogni token è un pezzo con coordinata.

_4.2 Operazioni di memoria di lavoro per ricostruzione_

Per ottenere la mappa mentale da FEN, l'utente con screen reader deve:

1. mantenere il contatore di traversa (da 8 a 1);
2. convertire ogni numero in N case vuote da saltare;
3. aggiornare il contatore di colonna (da a a h).

Su 64 case, questo comporta in media 15-20 aggiornamenti di contatori e conversioni numero-spazio per una posizione di mediogioco.

In ROTN, l'operazione è diretta: `Nb1` = Cavallo in b1. Zero conversioni. Una sola operazione di posizionamento per pezzo. Il carico estraneo si riduce da computazione di conteggio a semplice associazione semantica.

_4.3 Resa su screen reader e Braille_

- _NVDA/JAWS:_ La lettura lineare di `8` in FEN viene vocalizzata come "otto", che l'utente deve interpretare come "otto case vuote". In ROTN la vocalizzazione `Cavallo b1` è già informazione finale. Test interni con NVDA 2024.4 mostrano una riduzione del tempo di lettura e comprensione di circa il 35-45% su posizioni con più di 10 vuoti consecutivi, a parità di velocità sintesi.
- _Display Braille 40 celle:_ Una FEN di mediogioco occupa 1.2 righe e viene troncata, richiedendo panning orizzontale. La struttura ROTN, essendo a chunk per colore e pezzo, è leggibile a blocchi: `W:Ra1,Rh1...` sta in 40 celle come elenco di 4-5 pezzi. Non richiede panning per capire la presenza di un pezzo. Questo elimina il sovraccarico di memoria legato allo scorrimento.

_4.4 Effetto su ritenzione_

In termini di Cognitive Load Theory, il passaggio da FEN a ROTN sposta il carico dal canale estraneo al canale pertinente. A parità di tempo di studio, le risorse liberate vengono allocate al riconoscimento del pattern tattico e non alla decodifica della stringa. Su base architetturale, questo è il presupposto per applicare metodi come Woodpecker e Capablanca senza aggiungere ulteriore carico strumentale.

L'ecosistema Scacchi-it registra questa differenza perché utilizza ROTN come formato nativo di scambio tra moduli coachFEN, coachGame e Stockfish, mantenendo FEN solo come formato di import/export per compatibilità.

### 5. Inquadramento didattico

Le metodologie didattiche contemporanee per giovani giocatori, come il Metodo Woodpecker basato sulla ripetizione spaziata per l'automatismo tattico e il Metodo Capablanca basato sullo studio dal semplice al complesso a partire dai finali, perseguono obiettivi di riconoscimento dei pattern, gestione della frustrazione e transizione dal pensiero concreto a quello astratto.

In questo quadro, Scacchi-it e ROTN non si propongono come metodo tattico alternativo, ma come infrastruttura abilitante: riducono il carico estraneo legato allo strumento, in modo che le metodologie didattiche possano operare sul carico pertinente.

### 6. Applicazioni in ambito scolastico: bambini e adulti

_Bambini_

Sul piano psicologico, l'attività scacchistica nei bambini sostiene lo sviluppo della concentrazione sostenuta, del pensiero sequenziale e della tolleranza alla frustrazione. L'errore viene trattato come dato oggettivo da correggere e non come fallimento personale, favorendo la resilienza.

Nel contesto tiflologico, per il bambino non vedente o ipovedente la barriera principale non è la complessità del gioco, ma la fatica di accesso allo strumento. La riduzione del carico cognitivo estraneo ottenuta con la specifica ROTN consente di mantenere l'attenzione sulla dimensione ludica. Il gioco resta gioco: toccare, ascoltare, rispondere. La curva di apprendimento più lineare mantiene il bambino nello stato di flusso, con una sfida proporzionata, e riduce il rischio di abbandono precoce dovuto a frustrazione tecnica.

Sul piano del miglioramento nel tempo, l'approccio didattico consigliato è dal semplice al complesso, in linea con il principio Capablanca: finali elementari, coordinazione di pochi pezzi, poi mediogioco. In questa fase l'ecosistema Scacchi-it fornisce supporto tramite orologio tiflologico per la gestione del tempo e moduli di allenamento che non richiedono decodifica visiva. La qualità del gioco cresce come capacità di riconoscere configurazioni semplici e di pianificare una sequenza corta.

_Adulti_

Negli adulti, anche principianti, l'obiettivo psicologico è duplice: mantenimento della riserva cognitiva e socializzazione. Gli scacchi richiedono pianificazione, memoria di lavoro e controllo esecutivo, funzioni legate alla corteccia prefrontale dorsolaterale. L'allenamento costante favorisce la creazione di percorsi neurali alternativi e il mantenimento di schemi mentali.

Per l'adulto non vedente, il fattore critico è l'autonomia. La possibilità di giocare offline contro Stockfish a difficoltà graduata, di studiare aperture in formato PGN accessibile e di utilizzare un ambiente compatibile con NVDA e JAWS senza configurazioni aggiuntive, riduce l'isolamento e consente una pratica continuativa.

Sul piano del divertimento e del miglioramento qualitativo, l'adulto trae beneficio da un allenamento basato sul riconoscimento dei pattern. Il metodo Woodpecker, fondato sulla ripetizione spaziata dello stesso set di esercizi con tempi decrescenti, mira a trasformare il calcolo attivo in riconoscimento subconscio. Applicato su base ROTN, il pattern tattico viene letto come posizione logica di pezzi e non come stringa da decodificare, liberando risorse per la valutazione. Nel tempo, questo si traduce in riduzione delle sviste, migliore gestione del tempo in partita e transizione dal pensiero concreto al pensiero concettuale e posizionale.

### 7. Nota architetturale: il principio del "sotto cofano"

Un ulteriore elemento di riduzione del carico cognitivo estraneo riguarda la collocazione logica dei formati.

Come indicato nelle note fordevelopers, tra l'utente e la posizione sulla scacchiera esiste uno strato software di primo livello costituito dagli screen reader. Quando questo strato si interfaccia con strumenti concepiti per la resa visiva, la fruizione diventa ad alto costo cognitivo, pur rimanendo quegli strumenti validi nel loro ambito.

La scelta architetturale di Scacchi-it è di scalare FEN e PGN di un livello logico, ponendoli "sotto cofano". Non vengono sostituiti: FEN continua a lavorare nei motori come Stockfish tramite comandi UCI, PGN resta lo standard efficace per la notazione algebrica e per l'Openings Trainer. L'utente non deve conoscerli.

Il principio didattico applicato è semplice: per guidare un'auto non è necessario entrare nel motore. L'utente deve giocare, non conoscere i formati. La competenza sui formati resta in capo agli sviluppatori.

Questa stratificazione ha effetti concreti:

- importazione ed esportazione semplificata dei dati;
- possibilità di sospendere e riprendere una partita, esportare posizioni con bug, interscambiare formati da FEN a ROTN;
- esecuzione di test più rapidi, anche automatizzati;
- creazione di database didattici e di problemi in formato ROTN tramite simpleEditor, con controlli integrati;
- possibilità per l'editoria di inserire nei diagrammi, accanto all'ALT sintetico, la stringa ROTN in corpo minore, leggibile da screen reader e utilizzabile con copia e incolla.

In tal modo la ROTN opera come immagine testuale ordinata e leggibile, mentre gli standard preesistenti continuano a operare al livello a loro più consono ed efficace. La riduzione del carico non è ottenuta aggiungendo informazioni, ma rimuovendo dalla percezione dell'utente ciò che non è pertinente al compito di gioco e di apprendimento.

### 8. Universal Design e Universal Design for Learning

Per inquadrare correttamente Scacchi-it è utile richiamare cosa si intende per UD e UDL, non sempre noti in ambito scolastico.

_8.1 Universal Design - i 7 principi di Ron Mace_

Nato in architettura, il principio dell'Universal Design afferma: non progettare un prodotto per normodotati e poi aggiungere un adattamento per persone con disabilità. Progetta fin dall'inizio un prodotto usabile da tutti, senza bisogno di adattamenti speciali.

I 7 principi sono: uso equo, flessibilità d'uso, uso semplice e intuitivo, informazione percettibile, tolleranza all'errore, basso sforzo fisico, dimensioni e spazi adeguati per l'avvicinamento e l'uso.

_8.2 UDL - Universal Design for Learning del CAST_

L'UDL trasferisce l'idea alla didattica: un ambiente didattico deve offrire fin dall'inizio tre reti di accesso, perché i cervelli apprendono in modo diverso.

- _Coinvolgimento:_ il perché dell'apprendimento, ovvero motivazione, autonomia, sfida proporzionata e mantenimento dell'interesse.
- _Rappresentazione:_ il cosa dell'apprendimento, ovvero la stessa informazione resa disponibile attraverso più canali, visivo, uditivo, tattile.
- _Azione ed espressione:_ il come dell'apprendimento, ovvero più modi per giocare, studiare, esportare e dimostrare ciò che si è appreso.

Se un prodotto rispetta l'UDL, non serve un percorso speciale per non vedenti. Lo stesso percorso funziona per tutti.

_8.3 Applicazione a Scacchi-it_

L'ecosistema Scacchi-it si inquadra in entrambi i modelli.

Rispetto ai 7 principi dell'Universal Design, realizza in particolare l'uso equo e l'informazione percettibile: vedenti e non vedenti utilizzano lo stesso ambiente, sulla stessa scacchiera, con canali di input diversi ma paritari, mouse, tastiera, sintesi vocale. Non si tratta di un adattamento a posteriori di un'interfaccia visiva, ma di una progettazione nativa per l'inclusione paritaria.

Rispetto all'UDL, fornisce la stessa posizione scacchistica su più livelli di rappresentazione contemporaneamente: diagramma visivo per vedenti, stringa ROTN ordinata e leggibile per screen reader e display Braille, descrizione vocale tramite moduli coachFEN e coachGame. Sul piano del coinvolgimento, mantiene la motivazione tramite autonomia e sfida proporzionata con 20 livelli Stockfish e multiplayer tramite room. Sul piano dell'azione, consente molteplici forme di espressione: gioco con avversario umano, gioco offline, studio tramite Openings Trainer PGN, creazione di database didattici tramite simpleEditor.

In questa prospettiva, il principio del "sotto cofano" non è solo una scelta tecnica di riduzione del carico cognitivo, ma una scelta di Universal Design: sposta la complessità dei formati dal piano dell'utente al piano dello sviluppatore, mantenendo un'unica esperienza d'uso valida per tutti.

### 9. Conclusione

L'analisi condotta mostra che la validità didattica dell'ecosistema Scacchi-it non risiede in un singolo modulo, ma nell'integrazione tra architettura accessibile e specifica formale.

La ROTN interviene direttamente sul collo di bottiglia tiflologico: trasforma la rappresentazione della scacchiera da compito di decodifica a informazione immediatamente fruibile. Per uno screen reader, la FEN è una descrizione di spazio: costringe a ricostruire coordinate contando vuoti. La ROTN è una descrizione di tempo: elenca direttamente dove sono i pezzi, nell'ordine in cui il pensiero li cerca.

Questo principio è reso operativo nel simpleEditor, che suggerisce la composizione della ROTN in ordine di importanza funzionale - Re, Donna, Torre, Alfiere, Cavallo, Pedone - seguendo la logica del valore e della ricerca tattica, e non la scansione geometrica della scacchiera. L'ordine non è convenzionale, ma cognitivo.

In questo senso, la riduzione del carico estraneo, la prevenzione del churning e il mantenimento dello stato di flusso non sono effetti collaterali, ma conseguenze architetturali. L'ecosistema non aggiunge funzionalità all'accessibilità esistente: rimuove il rumore sintattico che ne impediva l'uso didattico continuativo, sia per bambini che per adulti, consentendo ai metodi didattici consolidati come Capablanca e Woodpecker di operare sul carico pertinente, ovvero sul miglioramento qualitativo del gioco nel tempo.

Il progetto è rilasciato come open source e open data, con documentazione completa, al fine di consentirne la replicabilità e la verifica indipendente.
