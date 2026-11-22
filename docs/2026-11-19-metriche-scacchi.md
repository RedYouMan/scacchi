---
title: "La Human Interaction Cognition di Scacchi-it con 20 Metriche "
description: "La Human Interaction Cognition di Scacchi-it"

categories: blog
---

# La Human Interaction Cognition di Scacchi-it con 20 metriche. Cosa misuriamo davvero quando giochiamo a scacchi

_Autore:_ Rosario Turco, ingegnere e ricercatore indipendente,

## Da dove vengono le metriche

Le metriche sono frutto di idee maturate negli approfondimenti fatti con gli articoli presenti sul mio blog da Settembre a Novembre 2026.

Le metriche HIC permettono di dare una valutazione dei progressi nel tempo di un giocatore o di un allievo e tornano come utile strumento nella scuola per la didattica misurabile per vedenti e non vedenti.

Le famiglie di metriche che qui analizziamo sono sei:

- Neuroscienze e carico cognitivo: Legge di Hick, Span di Baddeley, Oblio, Rotazione, curva di apprendimento, Sweller
- Epistemologia: Kant, spazio semantico-sintattico, ROTN come invariante
- Psicologia dell'autonomia: locus of control di Rotter, oggetto transizionale di Winnicott
- Psicologia agonistica: tilt, bluff, gestione stress, Teoria della Mente
- Psicologia didattica: scaffolding con CoachFEN e CoachGame, Vygotskij, Bruner, Sweller
- Sociologia ed etica: comunità di pratica, stigma, open metric

Ogni metrica qui sotto è descritta allo stesso modo: nome, cosa misura, da dove viene, come si misura in Scacchi-it, formula semplice, esempio, cosa fa l'insegnante con lo storico dei progressi, perché conta.

Le metriche che richiedono l'online, si suggerisce di farle in rete locale, dove il pc dell'insegnante farà da server hardware e disporrà del software Scacchi-it. L'insegnante metterà attivo serverScacchi producendo il file server.log e potrà fare le analisi richieste dalle metriche.

## Le metriche

- Numero metrica: 1
- Nome: Tempo per mossa, T_m
- Cosa misura: Velocità decisionale e costo cognitivo per mossa
- Da dove viene: Neuroscienze, carico cognitivo
- Come si misura in Scacchi-it: Con log e sortLog. Si prendono i tempi delle mosse giocate dall'allievo.
- Come si fa, passo per passo:
- Passo 1, partita: Si gioca in rete e con sortLog si ottiene serverOrdinato.txt dei tempi relativi alle room. Ma se la room è una sola si può esaminare direttamente server.log
- Passo 2, tempi: Si prendono i tempi di pensiero delle mosse giocate dall'allievo nella sua room
- Passo 3, media: Si fa la media dei tempi.
- Formula semplice: T_m uguale somma dei tempi delle mosse dell'allievo diviso numero di mosse giocate dall'allievo.
- Esempio: L'allievo gioca 20 mosse. Somma dei tempi uguale 600 secondi. Calcolo: 600 diviso 20 uguale 30. T_m uguale 30 secondi per mossa.
- Cosa fa l'insegnante, storico dei progressi: Per ogni partita annota data, avversario, T_m della partita. Lo mette in uno storico dei progressi, data dopo data. La prossima volta, prima di iniziare, guarda lo storico e controlla se T_m sta scendendo a parità di qualità.
- Perché conta: Se il tempo per mossa scende a parità di qualità, il carico cognitivo sta scendendo. Non sei più veloce perché hai fretta. Sei più veloce perché hai capito.

- Numero metrica: 2
- Nome: Variabilità del tempo, CV_tempo
- Cosa misura: Instabilità, affaticamento, inizio tilt
- Da dove viene: Tilt, Bluff e Oblio
- Come si misura in Scacchi-it: Da log e extractGame, deviazione standard diviso media dei tempi
- Come si fa, passo per passo:
- Passo 1, partita: Si gioca in rete e si usa sortLog ottenendo serverOrdinato.txt con i tempi di tutte le room. Se la room è solo una si può usare direttamente server.log
- Passo 2, tempi: Si prendono i tempi delle mosse giocate dall'allievo.
- Passo 3, calcolo: Si calcola media e deviazione standard dei tempi con la calcolatrice.
- Formula semplice: CV_tempo uguale deviazione standard diviso media.
- Esempio: Media tempi uguale 30 secondi. Deviazione standard uguale 15 secondi. Calcolo: 15 diviso 30 uguale 0,5. CV_tempo uguale 0,5.
- Cosa fa l'insegnante, storico dei progressi: Per ogni partita annota data, avversario, media tempi, deviazione standard, CV_tempo. Lo mette in uno storico dei progressi, data dopo data. La prossima volta, prima di iniziare, guarda lo storico e controlla se CV_tempo sta salendo.
- Perché conta: Un giocatore stanco non diventa solo più lento. Diventa irregolare. Prima si vede nella variabilità, poi negli errori.

- Numero metrica: 3
- Nome: Tempo di recupero dopo errore
- Cosa misura: Resilienza, quanto ci metti a tornare lucido dopo un blunder
- Da dove viene: Tilt, Bluff e Oblio
- Come si misura in Scacchi-it: Con il tempo preso dai log delle 3 mosse successive a un errore segnalato da CoachGame
- Come si fa, passo per passo:
- Passo 1, partita: Si gioca in rete e con sortLog si ottiene serverOrdinato.txt con i tempi relativi a tutte le room. Ma se la room è solo una si può consultare per i tempi direttamente server.log. Mentre con extractGame si ottiene il file della partita relativa alla room dell'allievo da copiare nella cartella registrazioni.
- Passo 2, analisi: Si lancia CoachGame che lavora su cartella registrazioni sul file indicato in input. Si trova la mossa giudicata pessima.
- Passo 3, tempi: Si prendono dai log i tempi delle 3 mosse successive a quell'errore, giocate dall'allievo.
- Passo 4, media: Si fa la media di quei 3 tempi.
- Formula semplice: Tempo di recupero uguale somma dei tempi delle 3 mosse dopo l'errore diviso 3.
- Esempio: Dopo l'errore l'allievo impiega 60, 45 e 30 secondi. Calcolo: 60 più 45 più 30 uguale 135. 135 diviso 3 uguale 45. Tempo di recupero uguale 45 secondi.
- Cosa fa l'insegnante, storico dei progressi: Per ogni partita annota data, avversario, mossa dell'errore, tempo di recupero. Lo mette in uno storico dei progressi, data dopo data. La prossima volta, prima di iniziare, guarda lo storico e controlla se il tempo di recupero sta scendendo.
- Perché conta: Il blunder capita a tutti. La differenza non è chi sbaglia di meno. È chi torna lucido prima.

- Numero metrica: 4
- Nome: Indice di Tilt, IT
- Cosa misura: Crollo di qualità dopo un evento negativo, per esempio un blunder o una sconfitta in una partita precedente
- Da dove viene: Psicologia agonistica
- Come si misura in Scacchi-it: Con il confronto della qualità delle mosse prima e dopo l'evento negativo, da report.txt di CoachGame
- Come si fa, passo per passo:
- Passo 1, partita: Si gioca una partita, offline con Ctrl+W o in rete con extractGame.
- Passo 2, analisi: Si lancia CoachGame. Si trova l'evento negativo, per esempio un blunder o una sconfitta in una partita precedente.
- Passo 3, qualità prima: Si calcola la perdita media delle 5 mosse prima dell'evento. Per ogni mossa si guarda in report.txt la valutazione prima e dopo la mossa.
- Attenzione al segno: In report.txt la valutazione è sempre dal punto di vista del Bianco. + significa meglio per il Bianco, - significa meglio per il Nero.
- Se muove il Bianco: perdita uguale valutazione prima meno valutazione dopo.
- Se muove il Nero: perdita uguale valutazione dopo meno valutazione prima.
- Se il risultato è negativo, si segna 0. Vuol dire che hai giocato bene come il motore o quasi.
- Se in report.txt vedi #3 o -#2 invece di un numero, vuol dire matto in 3, matto in 2. Se mantieni il matto, perdita uguale 0. Se perdi il matto, è errore grave e si segna perdita alta, oltre 200.
- Passo 4, qualità dopo: Si calcola allo stesso modo la perdita media delle 5 mosse dopo l'evento.
- Formula semplice: IT uguale perdita media dopo l'evento meno perdita media prima dell'evento.
- Esempio: Perdita media prima uguale 30. Perdita media dopo uguale 110. Calcolo: 110 meno 30 uguale 80. IT uguale 80.
- Cosa fa l'insegnante, storico dei progressi: Per ogni partita annota data, avversario, evento negativo, perdita media prima, perdita media dopo, IT. Lo mette in uno storico dei progressi, data dopo data. La prossima volta, prima di iniziare, guarda lo storico e controlla se IT sta scendendo.
- Perché conta: Il tilt non è una sensazione. È un crollo misurabile. Se lo misuri, lo puoi allenare.

- Numero metrica: 5
- Nome: Indice di Oblio, IO
- Cosa misura: Quanto perdi della posizione se la rievochi dopo una pausa, senza rileggerla
- Da dove viene: Oblio, Rotazione
- Come si misura in Scacchi-it: Con prova di rievocazione ROTN dopo 5 e 30 minuti. Lo scenario è questo, passo per passo.
- Passo 1, origine: Prendi una ROTN che proviene da estensioni browser, o da un ebook o dal tuo insegnante che l'ha scritta col simpleEditor, quindi copi il file su un Notepad.
- Passo 2, originale salvato: col Notepad. Quel Notepad è l'originale, e resta chiuso durante la pausa.
- Passo 3, gioco: Giochi con quella ROTN, la studi, la capisci.
- Passo 4, pausa: Aspetti 5 minuti per la prima prova, 30 minuti per la seconda prova. In pausa non riapri il Notepad.
- Passo 5, rievocazione: Dopo la pausa, senza guardare il Notepad salvato, cerchi di scrivere di nuovo la posizione come ROTN con simpleEditor. SimpleEditor scrive solo, non legge, ed è proprio quello che serve qui: scrivere a memoria.
- Passo 6, cosa riscrivi: Scrivi i pezzi del bianco e i pezzi del nero, compreso eventuali arrocchi ambigui, a chi appartiene il tratto e il numero di mossa ultimo giocato dal bianco con cui hai iniziato.
- Passo 7, confronto: Alla fine confronti quello che hai riscritto con simpleEditor con l'originale salvato su Notepad. Conti pezzi corretti, pezzi persi, pezzi spostati, arrocchi sbagliati.
- Formula semplice: Indice di Oblio uguale pezzi persi o spostati diviso pezzi totali originali. Si calcola a 5 minuti e a 30 minuti.
- Esempio: Posizione originale con 24 pezzi. Dopo 5 minuti perdi o sposti 2 pezzi. Calcolo: 2 diviso 24 uguale 0,083. IO a 5 minuti uguale 8,3%.
- Cosa fa l'insegnante, storico dei progressi: Per ogni prova annota data, IO a 5 minuti, IO a 30 minuti. Lo mette in uno storico dei progressi, data dopo data. La prossima volta, prima di iniziare, guarda lo storico e controlla se IO sta scendendo.
- Perché conta: Per un giocatore la memoria non è un aiuto. È la scacchiera. Misurare l'oblio significa misurare quanto resta della scacchiera dopo una pausa.

- Numero metrica: 6
- Nome: Capacità di Rotazione Mentale, RM
- Cosa misura: Riesci a tenere la scacchiera girata senza perdere i pezzi
- Da dove viene: Kant gioca accanto al non vedente
- Come si misura in Scacchi-it: Con due test separati, uno visivo e uno mentale. Vanno fatti in quest'ordine, prima il visivo e poi il mentale.
- Test 1, 180 gradi, scacchiera visiva, con Ctrl+G. Lo scenario è questo, passo per passo.
- Passo 1, partenza: L'insegnante prepara una posizione in ROTN e la fa vedere su scacchiera all'allievo (il giocatore vedente usa gli occhi, il non vedente usa Ctrl+D). L'allievo la studia finché dice di averla capita.
- Passo 2, pezzo di prova: L'insegnante sceglie un pezzo preciso, per esempio Af1. Lo dice a voce: il pezzo di prova è Af1.
- Passo 3, prima domanda, prima della rotazione: L'insegnante chiede: Af1, adesso, prima di girare, è a destra o a sinistra rispetto a te? È vicino a te o lontano da te? L'allievo risponde. Questa risposta è il punto di partenza.
- Passo 4, rotazione: L'allievo preme Ctrl+G. La scacchiera gira di 180 gradi. La rotazione la fa lo strumento, non la mente.
- Passo 5, punto chiave da spiegare all'allievo: Anche se ruotata, è sempre Af1. Il nome non cambia. Non diventa un altro pezzo. Non cambia casella nella ROTN.
- Passo 6, seconda domanda, dopo la rotazione: L'insegnante chiede di nuovo: Af1, adesso, dopo la rotazione, è a destra o a sinistra rispetto a te? È vicino a te o lontano da te? L'allievo risponde senza guardare altro.
- Passo 7, confronto: L'insegnante confronta la prima risposta con la seconda. Dopo 180 gradi la risposta deve essere opposta: se prima era a destra e vicino, dopo deve essere a sinistra e lontano. Se l'allievo risponde giusto, ha tenuto il pezzo. Se risponde come prima, ha perso la rotazione.
- Passo 8, ripetizione: Si ripete con altri 2 o 3 pezzi, sempre uno alla volta, sempre con la stessa procedura: domanda prima, Ctrl+G, domanda dopo.
- Formula semplice: Test 180 riuscito uguale risposte giuste diviso pezzi provati.
- Esempio: Pezzi provati 3. Risposte giuste 2. Calcolo: 2 diviso 3 per 100 uguale 66%. Test 180 uguale 66%.
- Test 2, 90 gradi, scacchiera mentale, senza Ctrl+G. Lo scenario è questo, passo per passo.
- Passo 1, partenza: Si parte dalla stessa posizione sulla scacchiera o da una posizione nuova. L'allievo la studia finché dice di averla capita.
- Passo 2, pezzo di prova: L'insegnante sceglie un pezzo preciso, per esempio Af1. Lo dice a voce: il pezzo di prova è Af1.
- Passo 3, consegna: L'insegnante dice: senza premere Ctrl+G, senza girare niente sullo schermo, ruota tu in testa la scacchiera di 90 gradi. Poi dice rotazione a destra o rotazione a sinistra. Una sola alla volta, non tutte e due insieme.
- Passo 4, domanda: L'insegnante chiede: se ruoti a destra di 90 gradi, il pezzo Af1 in quale casella si troverebbe? Oppure, se la prova è a sinistra: se ruoti a sinistra di 90 gradi, il pezzo Af1 in quale casella si troverebbe?
- Passo 5, punto chiave da spiegare all'allievo: Qui Af1 non resta fermo. A 90 gradi cambia casella. La nuova casella la deve trovare solo in testa, senza vederla girata. Qua è scacchiera mentale.
- Passo 6, risposta: L'allievo dice la casella a voce, o la scrive con simpleEditor. Non deve premere Ctrl+G per controllare.
- Passo 7, controllo: L'insegnante confronta la casella detta dall'allievo con la casella giusta, preparata prima. Si conta giusta o sbagliata.
- Passo 8, ripetizione: Si ripete con altri 2 o 3 pezzi, e con rotazione a destra e rotazione a sinistra separate. Mai mescolare destra e sinistra nella stessa domanda.
- Formula semplice: Test 90 riuscito uguale caselle giuste diviso pezzi provati.
- Esempio: Pezzi provati 3. Caselle giuste 1. Calcolo: 1 diviso 3 per 100 uguale 33%. Test 90 uguale 33%.
- Cosa fa l'insegnante, storico dei progressi: Per ogni test annota data, test 180, test 90. Lo mette in uno storico dei progressi, data dopo data. La prossima volta, prima di iniziare, guarda lo storico e controlla se i due test stanno salendo.
- Perché conta: Kant dice che lo spazio non è fuori di noi, è una forma della nostra mente. Qui lo verifichi in due modi: con la scacchiera girata che vedi con Ctrl+G, e con la scacchiera girata che devi immaginare a 90 gradi. Se ruoti la scacchiera in testa e non perdi i pezzi, lo spazio è tuo.

- Numero metrica: 7
- Nome: Span di posizione, Span 8x8
- Cosa misura: Quanti pezzi ti ricordi, dove sono, e se vedi il pattern
- Da dove viene: Neuroscienze, Baddeley
- Come si misura in Scacchi-it: Con prova su scacchiera. Lo scenario è questo, passo per passo.
- Passo 1, preparazione: L'insegnante prepara una posizione in ROTN con simpleEditor, con un pattern preciso, per esempio forchetta, inchiodatura, infilata.
- Passo 2, caricamento: L'insegnante fa caricare la ROTN all'allievo. L'allievo lavora su scacchiera, non su testo.
- Passo 3, studio da scacchiera: L'allievo studia la posizione da scacchiera. Se non vedente deve usare Ctrl+D per ascoltare lo schieramento, cioè per controllare dove sono i pezzi del bianco e dove sono i pezzi del nero.
- Passo 4, chiusura: L'insegnante fa uscire da Scacchi-it.
- Passo 5, rievocazione su scacchiera mentale: Subito, senza pausa, l'allievo deve dire a voce, quali pezzi si ricorda e dove sono. Dice pezzi e caselle, come li ha sentiti da scacchiera con Ctrl+D.
- Passo 6, pattern: Subito dopo, sempre a voce, l'allievo dice se ha individuato un pattern e quale: forchetta, inchiodatura, infilata, o altro. Non basta dire c'è un pattern. Deve dire quale e con quali pezzi.
- Passo 7, riapertura Scacchi-it e caricamento della ROTN di nuovo e controllo visivo con allievo vedente o con Ctrl+D se l'allievo è non vedente. L'insegnante confronta lo schieramento vero con quello detto dall'allievo. Conta pezzi corretti, pezzi persi, pezzi spostati di casella. Poi controlla il pattern: giusto, sbagliato, o non visto.
- Formula semplice: Span uguale pezzi corretti nella casella giusta diviso pezzi totali originali. Pattern a parte: visto giusto, visto sbagliato, non visto.
- Esempio: Posizione con 20 pezzi. Pezzi corretti 15. Calcolo: 15 diviso 20 per 100 uguale 75%. Span uguale 75%. Pattern forchetta visto giusto.
- Cosa fa l'insegnante, storico dei progressi: Per ogni prova annota data, span, pattern visto giusto, sbagliato o non visto. Lo mette in uno storico dei progressi, data dopo data. La prossima volta, prima di iniziare, guarda lo storico e controlla se span sta salendo e se il pattern viene visto giusto più spesso.
- Perché conta: È quanto della posizione riesci a tenere nella scacchiera mentale mentre pensi alla mossa successiva. Se ti ricordi i pezzi ma non dove sono, non hai la posizione. Se ti ricordi i pezzi e dove sono ma non vedi la forchetta, non hai capito la posizione. E se la leggi da testo invece che da scacchiera, non stai misurando lo span, stai misurando la lettura.

- Numero metrica: 8
- Nome: Carico ROTN contro FEN, C_ROTN
- Cosa misura: Quanto costa leggere e capire la posizione in ROTN rispetto a FEN
- Da dove viene: Estetica ed etica della Nuova Visione
- Come si misura in Scacchi-it: Con tempo di lettura più errori di ricostruzione. FEN resta sotto cofano, non provoca carico
- Come si fa, passo per passo:
- Passo 1, posizione: L'insegnante prepara la stessa posizione in due forme: ROTN e FEN.
- Passo 2, lettura ROTN: L'allievo legge il file della posizione in ROTN, con gli occhi se vedente, con screen reader da file se non vedente. Si misura il tempo di lettura.
- Passo 3, lettura FEN: L'allievo legge la stessa posizione in FEN, con gli occhi se vedente, con screen reader se non vedente. Si misura il tempo di lettura.
- Passo 4, ricostruzione: Dopo ogni lettura, senza guardare, l'allievo deve dire dove sono i pezzi. Si contano gli errori.
- Formula semplice: Differenza tempo uguale tempo lettura FEN meno tempo lettura ROTN. Differenza errori uguale errori FEN meno errori ROTN.
- Esempio: Tempo ROTN 40 secondi, 1 errore. Tempo FEN 90 secondi, 5 errori. Calcolo differenza tempo: 90 meno 40 uguale 50 secondi in più con FEN. Calcolo differenza errori: 5 meno 1 uguale 4 errori in più con FEN.
- Cosa fa l'insegnante, storico dei progressi: Per ogni prova annota data, tempo ROTN, tempo FEN, errori ROTN, errori FEN, differenza tempo, differenza errori. Lo mette in uno storico dei progressi, data dopo data. La prossima volta, prima di iniziare, guarda lo storico e controlla se il vantaggio ROTN resta stabile.
- Perché conta: FEN è nata per chi vede. ROTN è nata per chi ascolta. Confrontarle non è una gara di notazioni. È dimostrare che la forma cambia il costo del pensiero.

- Numero metrica: 9
- Nome: Perdita di valutazione per mossa, CPL
- Cosa misura: Qualità tecnica pura, in centipawn
- Da dove viene: CoachGame
- Come si misura in Scacchi-it: In report.txt guardi, per ogni mossa, la valutazione prima e dopo la mossa, il giudizio sbagliata o pessima, e la best move suggerita.
- Attenzione al segno: In report.txt la valutazione è sempre dal punto di vista del Bianco. + significa meglio per il Bianco, - significa meglio per il Nero.
- Se muove il Bianco: CPL uguale valutazione prima meno valutazione dopo.
- Se muove il Nero: CPL uguale valutazione dopo meno valutazione prima.
- Se il risultato è negativo, si segna 0. Vuol dire che hai giocato bene come il motore o quasi.
- Esempio, muove il Bianco: Valutazione prima +40. Il Bianco gioca una mossa pessima. Valutazione dopo -110. Best move suggerita: Cf3. Calcolo: 40 meno -110 uguale 150. 150 centipawn persi in una mossa. Questa è pessima.
- Esempio, muove il Nero: Valutazione prima +10. Il Nero gioca una mossa pessima. Valutazione dopo +180. Best move suggerita: Dxd4. Calcolo: 180 meno 10 uguale 170. 170 centipawn persi. Anche questa è pessima.
- Se in report.txt vedi #3 o -#2 invece di un numero, vuol dire matto in 3, matto in 2. Se mantieni il matto, CPL uguale 0. Se perdi il matto, è errore grave e si segna CPL alto, oltre 200.
- Cosa fa l'insegnante: Non calcola e butta via. Per ogni partita annota data, avversario, CPL medio della partita, numero di mosse pessime. CPL medio partita uguale somma di tutti i CPL delle mosse diviso numero di mosse giocate dall'allievo. Lo mette in uno storico dei progressi, un quaderno o un foglio, data dopo data. La prossima volta, prima di iniziare, guarda lo storico e controlla se il CPL medio sta scendendo e se le pessime sono di meno.
- Perché conta: È la metrica più tecnica e più onesta. Non dice se sei bravo. Dice quanto hai perso rispetto alla mossa migliore, mossa per mossa. E con lo storico dice se stai davvero migliorando.

- Numero metrica: 10
- Nome: Tasso di Blunder, TB
- Cosa misura: Percentuale di mosse con giudizio pessima
- Da dove viene: Tilt, Bluff e Oblio
- Come si misura in Scacchi-it: Da CoachGame, conteggio su totale mosse.
- Formula semplice: TB uguale numero mosse pessime diviso numero mosse giocate dall'allievo, per 100.
- Attenzione: Il denominatore sono solo le mosse giocate dall'allievo, non tutte le mosse della partita.
- Esempio: L'allievo gioca 40 mosse. Di queste, 4 sono giudicate pessime da CoachGame. Calcolo: 4 diviso 40 per 100 uguale 10%. Vuol dire: 1 mossa su 10 è un blunder grave.
- Cosa fa l'insegnante: Non conta una sola partita. Per ogni partita annota data, avversario, TB della partita. Lo mette in uno storico dei progressi, un quaderno o un foglio, data dopo data. La prossima volta, prima di iniziare, guarda lo storico e controlla se il TB sta scendendo.
- Perché conta: Un blunder ogni tanto è normale. Un tasso alto e stabile dice che c'è un problema di metodo, non di talento. E con lo storico si vede se il metodo sta cambiando.

- Numero metrica: 11
- Nome: Indice di Bluff e Rischio, IB
- Cosa misura: Quante volte giochi una mossa oggettivamente inferiore per mettere pressione psicologica
- Da dove viene: Tilt, Bluff e Oblio, Sun Tzu
- Come si misura in Scacchi-it: Si parte dalla partita, si fa analizzare da CoachGame, si guardano due mosse di seguito.
- Formula semplice: Bluff riuscito uguale tua mossa con CPL alto più risposta dell'avversario subito dopo con CPL alto. Bluff fallito uguale tua mossa con CPL alto più risposta dell'avversario buona, CPL uguale 0 o basso.
- Esempio: Tu giochi una mossa con CPL 120. È oggettivamente inferiore. L'avversario, messo sotto pressione, risponde con una mossa con CPL 150. Ha peggiorato anche lui. Questo è bluff riuscito. Se invece l'avversario risponde con la best move, CPL 0, il bluff è fallito. Hai solo perso 120 centipawn.
- Come si ottiene la partita, modo 1, offline: Con Ctrl+W registri la partita offline tra te e l'avversario, stessa tastiera. Ottieni il file della partita.
- Come si ottiene la partita, modo 2, online: Ottieni la partita dai log del server con extractGame. Copi il file prodotto da extractGame sotto cartella registrazioni, perché CoachGame cerca il file lì.
- Come si analizza: Attivi CoachGame. CoachGame fa il lavoro in due volte. Prima esamina il gioco del Bianco e produce report.txt. L'insegnante lo esamina e registra i valori che servono nello storico dei progressi. Poi esamina il gioco del Nero e produce di nuovo report.txt. L'insegnante lo esamina e registra i valori che servono nello storico dei progressi.
- Attenzione ai file: Non serve tenere tutti i report.txt. Basta che lo scriviamo. Una volta registrati i valori nello storico, il file si può lasciare sovrascrivere dalla prossima analisi. Se no rimane immondizia che cresce.
- Cosa fa l'insegnante: Per ogni partita annota data, avversario, numero di bluff tentati, numero di bluff riusciti. Lo mette in uno storico dei progressi, un quaderno o un foglio, data dopo data. La prossima volta, prima di iniziare, guarda lo storico e controlla se i bluff riusciti aumentano e se il costo in CPL scende.
- Perché conta: Il bluff non è imbrogliare. È una scelta con un costo tecnico e un possibile guadagno psicologico. Se lo misuri, smette di essere fortuna.

- Numero metrica: 12
- Nome: Qualità della patta negoziata, analisi a posteriori della patta accettata o rifiutata
- Cosa misura: Se hai accettato o rifiutato la patta in modo giusto, rispetto a chi stava meglio davvero alla mossa della proposta
- Da dove viene: Scacchi-it-Net, pedagogia, Teoria della Mente
- Come si misura in Scacchi-it: Solo analisi a posteriori. Si parte dal PGN, come per la metrica 11. Lo fa l'insegnante, o l'allievo se vuole saperlo da solo.
- Come si ottiene la partita, modo 1, offline: Con Ctrl+W registri la partita offline tra te e l'avversario, stessa tastiera. Ottieni il file della partita in cartella registrazioni.
- Come si ottiene la partita, modo 2, online: Ottieni il file della partita con extractGame dai log del server. Copi il file ottenuto in cartella registrazioni, perché CoachGame cerca il file lì.
- Come si analizza: Attivi CoachGame. CoachGame fa il lavoro in due volte. Prima esamina il gioco del Bianco e produce report.txt. Lo esamini e registri i valori che servono nello storico dei progressi. Poi esamina il gioco del Nero e produce di nuovo report.txt. Lo esamini e registri i valori che servono nello storico dei progressi. Vai alla mossa in cui è stata proposta la patta e guardi chi stava meglio a quella mossa, con la valutazione di report.txt.
- Formula semplice: Decisione giusta uguale ho accettato quando stavo peggio o pari, oppure ho rifiutato quando stavo meglio. Decisione sbagliata uguale ho accettato quando stavo meglio, oppure ho rifiutato quando stavo peggio.
- Esempio, posizione chiara: Valutazione -150 per te alla mossa della proposta, stavi peggio. Avevi accettato la patta. Decisione giusta. Hai preso la patta quando dovevi prenderla.
- Esempio, posizione chiara, al contrario: Valutazione +180 per te alla mossa della proposta, stavi meglio. Avevi accettato la patta. Decisione sbagliata. Hai regalato la patta.
- Esempio, posizione rognosa: Valutazione +20 per te alla mossa della proposta, quasi pari, posizione complicata. Avevi accettato la patta. Decisione giusta. Perché quasi pari vuol dire che la patta era buona, anche se stavi un po' meglio.
- Attenzione ai file: Come per la metrica 11, non serve tenere tutti i report.txt. Basta che lo scriviamo. Una volta registrati i valori nello storico, il file si può lasciare sovrascrivere dalla prossima analisi. Se no rimane immondizia che cresce.
- Cosa fa l'insegnante: Per ogni patta proposta, accettata o rifiutata, annota data, avversario, mossa della proposta, ho accettato o rifiutato, chi stava meglio a quella mossa, decisione giusta o sbagliata. Lo mette in uno storico dei progressi, un quaderno o un foglio, data dopo data. La prossima volta, prima di iniziare, guarda lo storico e controlla se le decisioni giuste aumentano.
- Perché conta: La patta non si giudica a sensazione. Si giudica a posteriori, con il PGN e CoachGame, guardando chi stava meglio davvero alla mossa della proposta.

- Numero metrica: 13
- Nome: Indice di Autonomia Operativa, IAO, al contrario
- Cosa misura: Quanti aiuti deve darti l'insegnante per fare un compito completo, da solo, con Scacchi-it e Windows
- Da dove viene: Psicologia dell'autonomia
- Come si misura in Scacchi-it: Non si misura con un log. Quindi non si contano le azioni dell'allievo dai log. Si fa al contrario. Si contano gli aiuti che l'insegnante deve dare.
- Come si fa, passo per passo:
- Passo 1, compito: L'insegnante dà un compito completo, sempre uguale o simile, per poter confrontare. Esempio di compito completo: Apri il manuale e trova una posizione. Valuta la posizione. Trova la FEN. Analizza con CoachFEN. Registra una partita. Analizza con CoachGame. Salva il risultato.
- Passo 2, l'allievo prova da solo: L'allievo prova a fare il compito da solo, senza che l'insegnante lo guidi in anticipo.
- Passo 3, l'insegnante conta gli aiuti: Ogni volta che l'allievo si blocca e l'insegnante deve dirgli un comando, l'insegnante fa una tacca. Una tacca per ogni comando suggerito. Non si conta la spiegazione di scacchi. Si contano solo i comandi operativi, Scacchi-it e Windows.
- Passo 4, si separano i due tipi di aiuti: Aiuti Scacchi-it da una parte, aiuti Windows dall'altra. Così si vede dove l'allievo è più dipendente.
- Formula semplice: IAO al contrario uguale totale aiuti. Più aiuti, meno autonomia. Meno aiuti, più autonomia. Non serve formula complicata. Si annota il totale.
- Lista delle cose che l'insegnante può dovergli dire, comandi Scacchi-it, da scacchigr.txt:
- Ctrl+X, apri il tutorial, manuale, da scacchiera
- Ctrl+H, richiama l'Help, in qualunque momento del gioco, con sintesi vocale
- Ctrl+D, ottieni a fronte di un colore lo schieramento desiderato, cioè ascolta dove sono i pezzi
- Ctrl+E, estrai la FEN da una posizione ROTN sulla scacchiera, mandandola verso la console
- Ctrl+G, cambia il punto di vista da cui giocare, mettiti dalla parte del colore che devi giocare
- Ctrl+W, registra la partita, solo a inizio partita, inserendo il nome del file
- Ctrl+S, sospendi la partita, per riprenderla quando vuoi, dandole un nome
- Ctrl+R, riprendi una partita sospesa, oppure carica una posizione, per esempio pos_12, oppure un file da incolla-ROTN in cartella sospensioni
- Ctrl+P, carica un problema scacchistico da risolvere da solo, per esempio problema1.txt
- Ctrl+J, fai risolvere un problema dal computer, analisi su massimo dieci mosse
- Ctrl+U, undo, annulla le mosse, mossa, cattura, enpassant, promozione, arrocco, solo nei casi permessi
- Ctrl+Z, scegli di giocare contro il computer, poi scegli colore e livello da 0 a 20
- Ctrl+A, forza il computer a giocare l'apertura che desideri, oppure fallo giocare da un punto della partita, con skill e colore
- Ctrl+T, attiva l'Openings Trainer, per apprendere le aperture, dopo Ctrl+G per il punto di vista
- Ctrl+N, reinizializza la scacchiera, pezzi alla linea di partenza
- Ctrl+Q, chiudi la scacchiera, esci
- Ctrl+L, abbandono da parte del re
- Ctrl+I, gioco online multiplayer, rispondi alle domande, inserisci player
  Se gioca online in rete locale:
- Alt-U quando deve far promuovere un pedone a un pezzo
- AltO per esaminare il tempo consumato
- AltT per switchare l'orologio all'avversario
- AltP per proporre la patta con Alt1 per accettare o Alt2 per rifiutare
- Alt-Tab, spostati da scacchiera a console e viceversa, per inserire informazioni
- ESC, zittisci la console
- Lista delle cose che l'insegnante può dovergli dire, comandi Windows, utility e applicativi, da scacchigr.txt:
- Win P Aprire il prompt dei comandi
- cd, spostarsi in una cartella, per esempio cd Scacchi-it
- copy, copiare un file, per esempio copy report.txt
- Win E poi selezionare un file per aprirlo cliccando sopra con return per leggere o modificare e salvare una ROTN, o per selezionare la sola parte del valore FEN
- Usare Esplora file, per lanciare nella directory scacchi-it un eseguibile, selezionarlo e fare return
- Lanciare scacchi-start, per avviare Scacchi-it e la scacchiera
- Lanciare clock-start, per l'orologio
- Lanciare incolla-fen, per incollare una FEN su Scacchi-it
- Lanciare incolla-ROTN, per mettere una ROTN in cartella sospensioni da caricare con Ctrl+R
- Lanciare simpleEditor, per scrivere una ROTN, per creare un Database di posizioni
- Lanciare validaFEN, per validare una FEN
- Lanciare sortLog, utility log
- Lanciare extractGame, per ottenere dal serverScacchi un file di testo da sottoporre a make-pgn
- Lanciare make-pgn, per creare un PGN con le partite registrate
- Lanciare make-FEN2SVG, per ottenere un'immagine svg da una FEN, uso: make-FEN2SVG FEN tra virgolette nome-img.svg
- Lanciare coachFEN-start, applicativo indipendente coachFEN, per sapere la miglior mossa su una FEN
- Lanciare coachGame-start, applicativo indipendente coachGame, per analizzare un file di partita registrata, produce report.txt
- vedere o sentire lo usage di una utility
- Sulla console, Ctrl+A seguito da Ctrl+C, per selezionare tutto e copiare, per esempio il valore FEN, o il contenuto della console scacchi-start, da incollare con Ctrl+V in email o su Notepad
- Esempio completo: Compito completo, prima volta. L'insegnante deve dirgli: Ctrl+X per il manuale, Ctrl+D per lo schieramento, Ctrl+E per la FEN, aprire il prompt, cd per andare in cartella, lanciare coachFEN-start. Totale aiuti: 3 Scacchi-it più 3 Windows uguale 6 aiuti.
- Esempio completo, dopo 5 sessioni: Stesso compito. L'insegnante deve dirgli solo: lanciare coachFEN-start da prompt. Totale aiuti: 0 Scacchi-it più 1 Windows uguale 1 aiuto. È migliorato. Da 6 a 1.
- Cosa fa l'insegnante, storico dei progressi: Per ogni compito annota data, sessione, compito dato, aiuti Scacchi-it, elenco dei comandi Scacchi-it suggeriti, aiuti Windows, elenco dei comandi Windows e utility suggeriti, totale aiuti. Lo mette in uno storico dei progressi, un quaderno o un foglio, data dopo data. La prossima volta, prima di iniziare, guarda lo storico e controlla se il totale aiuti sta scendendo, e se scende più sugli aiuti Scacchi-it o sugli aiuti Windows.
- Perché conta: L'autonomia non si dichiara. Si conta al contrario. Se all'inizio ti devo dire 6 comandi e dopo ti devo dire 1 comando, stai diventando autonomo. E si vede anche dove sei ancora dipendente: se restano solo aiuti Windows, il problema non è Scacchi-it, è Windows. Se restano solo aiuti Scacchi-it, il problema è Scacchi-it.

- Numero metrica: 14
- Nome: Curva di Apprendimento, CA
- Cosa misura: Quanto migliora CPL, Tempo, Span in 10 sessioni
- Da dove viene: Neuroscienze, didattica
- Come si misura in Scacchi-it: Con log e extractGame più CoachGame nel tempo. Si confrontano i valori della sessione 1 con i valori della sessione 10.
- Come si fa, passo per passo:
- Passo 1, sessione 1: Si misurano CPL medio, T_m e Span nella sessione 1. Sono i valori di partenza.
- Passo 2, sessioni 2-9: Si continua ad allenarsi, misurando ogni volta CPL medio, T_m e Span come nelle metriche 9, 1 e 7.
- Passo 3, sessione 10: Si misurano di nuovo CPL medio, T_m e Span nella sessione 10. Sono i valori di arrivo.
- Formula semplice: Miglioramento CPL uguale CPL medio sessione 1 meno CPL medio sessione 10. Miglioramento T_m uguale T_m sessione 1 meno T_m sessione 10. Miglioramento Span uguale Span sessione 10 meno Span sessione 1.
- Esempio: CPL medio sessione 1 uguale 80, sessione 10 uguale 40. Calcolo: 80 meno 40 uguale 40. Miglioramento CPL uguale 40. Span sessione 1 uguale 50%, sessione 10 uguale 75%. Calcolo: 75 meno 50 uguale 25. Miglioramento Span uguale 25 punti.
- Cosa fa l'insegnante, storico dei progressi: Per ogni sessione annota data, CPL medio, T_m, Span. Lo mette in uno storico dei progressi, data dopo data. Alla sessione 10 guarda lo storico e controlla se CPL e T_m sono scesi e Span è salito.
- Perché conta: Una sessione non dice niente. Dieci sessioni dicono se il metodo funziona. Questa metrica serve all'allievo, ma serve anche alla scuola che deve valutare un percorso.

- Numero metrica: 15
- Nome: Scaffolding Efficace, SE
- Cosa misura: Se il suggerimento CoachFEN ti resta in testa, e dopo ce la fai da solo sullo stesso tema, inoltre stimola l'autoapprendimento
- Da dove viene: Didattica, Vygotskij, Bruner
- Come si misura in Scacchi-it: Con 3 posizioni stesso tema. CoachFEN si usa una sola volta, sulla prima. La seconda e la terza si fanno senza aiuto
- Come si fa, passo per passo:
- Passo 1, tema: L'insegnante sceglie un tema tattico, per esempio forchetta, inchiodatura, infilata. Prepara 3 posizioni con lo stesso tema. Ha già la soluzione di tutte e tre.
- Passo 2, prima posizione, con aiuto: L'allievo guarda la prima posizione. Se vedente guarda con gli occhi, se non vedente usa Ctrl+D. Preleva la FEN con Ctrl+E. Lancia coachFEN-start sulla FEN. CoachFEN suggerisce la best move. L'allievo la gioca e capisce perché è quella.
- Passo 3, seconda posizione, senza aiuto CoachFEN: L'allievo guarda la seconda posizione, stesso tema. Se vedente guarda con gli occhi, se non vedente usa Ctrl+D. Niente CoachFEN. Gioca da solo.
- Passo 4, terza posizione, senza aiuto CoachFEN: L'allievo guarda la terza posizione, stesso tema. Se vedente guarda con gli occhi, se non vedente usa Ctrl+D. Niente CoachFEN. Gioca da solo.
- Passo 5, verifica: L'insegnante controlla se la mossa giocata nella seconda e nella terza posizione è giusta. Giusta uguale uguale alla soluzione che ha già. Non si usa CoachGame, perché qua non c'è una partita, ci sono solo 3 posizioni con al massimo una mossa a posizione.
- Formula semplice: SE del tema uguale numero di riuscite senza aiuto diviso 2, per 100. Le prove senza aiuto sono solo la seconda e la terza.
- Esempio: Tema forchetta. Seconda posizione riuscita. Terza posizione sbagliata. Calcolo: 1 diviso 2 per 100 uguale 50%. SE uguale 50%.
- Cosa fa l'insegnante, storico dei progressi: Per ogni tema annota data, tema tattico, riuscita seconda posizione si o no, riuscita terza posizione si o no, SE del tema. Lo mette in uno storico dei progressi, un quaderno o un foglio, data dopo data. La prossima volta, prima di iniziare, guarda lo storico e ripete con altri temi tattici. Controlla se SE sta salendo.
- Perché conta: L'aiuto buono non è quello che ti fa copiare. È quello che dopo non ti serve più. Se dopo un suggerimento ce la fai da solo sulla seconda e sulla terza posizione, hai imparato il tema. Se ce la fai solo con CoachFEN, hai solo copiato.

- Numero metrica: 16
- Nome: Stabilità in Zeitnot
- Cosa misura: Quanto peggiora il tuo CPL quando hai meno di 30 secondi
- Da dove viene: Scacchi-it-Net
- Come si misura in Scacchi-it: In due modi. Modo 1 offline, stessa scacchiera / tastiera, un orologio solo usato alternativamente, con Ctrl+W. Modo 2 in rete, ognuno ha il suo orologio ma come se fosse sempre uno solo sincronizzato, con file partita da extractGame
- Come si fa, passo per passo:

- Modo 1, offline, stessa scacchiera / tastiera, un orologio solo:
- Passo 1, partita: registrata con Ctrl+W. Gioca una partita sulla stessa scacchiera / tastiera, con un orologio solo, usato alternativamente dai due giocatori. L'orologio è tiflologico, ha i suoi comandi e ti legge lui il tempo a voce. Si lancia con clock-start, sulla stessa console di Scacchi-it. Spazio la prima volta per avviarlo. Registra la partita con Ctrl+W, solo offline e solo a inizio partita. Durante la partita l'allievo guarda la posizione. Se vedente guarda con gli occhi, se non vedente usa Ctrl+D.
- Passo 2, switch e tempo: Dopo ogni mossa il giocatore che ha appena mosso, restando sulla stessa scacchiera / tastiera, fa Alt-Tab sulla finestra Chess Clock e preme Spazio per dire di aver mosso e fare cambio turno. L'orologio detrae il tempo di pensiero e lo legge a voce. Così il giocatore stesso sa quanto tempo gli resta, senza che l'insegnante lo annunci. Quando l'allievo gioca una mossa con meno di 30 secondi su quell'orologio solo, quella mossa è zeitnot. Le altre sono tempo normale.
- Passo 3, analisi: Alla fine lancia CoachGame sulla partita registrata con Ctrl+W. Prendi il CPL di ogni mossa.

- Modo 2, in rete, orologi sincronizzati come se fosse uno solo:
- Passo 1, partita: Gioca una partita in rete, con orologio in rete di Scacchi-it-Net. Ognuno ha il suo orologio, ma come se fosse sempre uno solo, sincronizzato tra loro. Dopo aver mosso, l'allievo stesso switcha l'orologio all'avversario con AltT. Per sapere il tempo consumato, usa AltO, che sincronizza i minuti consumati localmente con l'altro. Durante la partita l'allievo guarda la posizione. Se vedente guarda con gli occhi, se non vedente usa Ctrl+D.
- Passo 2, switch e tempo: L'allievo stesso sa quanto tempo gli resta dall'orologio in rete. Quando l'allievo gioca una mossa con meno di 30 secondi sull'orologio in rete, quella mossa è zeitnot. Le altre sono tempo normale.
- Passo 3, file partita: Fai extractGame con nome room e nome file da analizzare. Ti dà il file di output della partita, in notazione algebrica, da sottoporre poi a coachGame
- Passo 4, verifica: Verifica nel file se catture, enpassant, promozioni sono riportate correttamente. Se c'è un errore, correggilo prima di andare avanti.
- Passo 5, metti in registrazioni: Metti il file di output della partita in cartella registrazioni.
- Passo 6, analisi: Lancia CoachGame con nome file .txt e colore b oppure n. Prendi il CPL di ogni mossa.

- Poi, uguale per i due modi:
- Passo 7, dividi: Fai due medie. Media CPL in tempo normale. Media CPL in zeitnot.
- Formula semplice: Peggioramento uguale media CPL in zeitnot meno media CPL in tempo normale.
- Esempio: Media CPL tempo normale uguale 35. Media CPL zeitnot uguale 95. Calcolo: 95 meno 35 uguale 60. Peggioramento uguale 60.
- Cosa fa l'insegnante, storico dei progressi: Per ogni partita annota data, avversario, modo usato, 1 offline con un orologio solo usato alternativamente o 2 in rete con orologi sincronizzati come se fosse uno solo, numero mosse in zeitnot, media CPL tempo normale, media CPL zeitnot, peggioramento. Lo mette in uno storico dei progressi, data dopo data. La prossima volta, prima di iniziare, guarda lo storico e controlla se il peggioramento sta scendendo.
- Perché conta: Sotto pressione il pensiero cambia. Se misuri quanto cambia, puoi allenare proprio quel momento. Se il peggioramento scende, sei più stabile in zeitnot.

- Numero metrica: 17
- Nome: Coerenza del Modello Mentale, CMM
- Cosa misura: La posizione che descrivi è coerente con la posizione reale
- Da dove viene: Epistemologia, conoscere uguale mantenere modello coerente
- Come si misura in Scacchi-it: Con test: ascolti la ROTN, descrivi minacce e piano, confronto con la soluzione che ha già l'insegnante
- Come si fa, passo per passo:
- Passo 1, posizione: L'insegnante sceglie una posizione. Ha già la soluzione: chi sta meglio, quali sono le minacce, qual è il piano giusto.
- Passo 2, ascolto: L'allievo guarda la posizione. Se vedente guarda con gli occhi, se non vedente usa Ctrl+D e ascolta la ROTN.
- Passo 3, descrizione: Senza muovere, l'allievo dice a voce: pezzi in presa, minacce dell'avversario, sua minaccia, piano per le prossime 2 mosse.
- Passo 4, confronto: L'insegnante confronta quello che ha detto l'allievo con la soluzione che ha già. Dà 1 punto per ogni elemento giusto: pezzi in presa, minaccia avversario, minaccia propria, piano.
- Formula semplice: CMM uguale punti fatti diviso 4, per 100.
- Esempio: L'allievo dice giusta la minaccia avversario e il piano, sbaglia pezzi in presa e minaccia propria. Calcolo: 2 diviso 4 per 100 uguale 50%. CMM uguale 50%.
- Cosa fa l'insegnante, storico dei progressi: Per ogni test annota data, FEN della posizione, punti fatti su 4, CMM. Lo mette in uno storico dei progressi, data dopo data. La prossima volta, prima di iniziare, guarda lo storico e controlla se CMM sta salendo e quale elemento sbaglia più spesso.
- Perché conta: Conoscere non è ripetere la posizione. È mantenere un modello coerente mentre la posizione cambia. Se CMM sale, il tuo modello mentale è più fedele alla posizione reale.

- Numero metrica: 18
- Nome: Indice di Simmetria NV e V
- Cosa misura: Stessa prestazione vedente e non vedente a parità di compito
- Da dove viene: Sociologia Nuova Visione
- Come si misura in Scacchi-it: Con stesso compito ROTN per i due gruppi, confronto di tempo e riuscita
- Come si fa, passo per passo:
- Passo 1, compito uguale: L'insegnante prepara lo stesso compito per vedente e non vedente. Per esempio 3 posizioni, trovare la best move. Stesso tempo a disposizione. Ha già la soluzione di tutte e tre.
- Passo 2, esecuzione: Il vedente guarda con gli occhi. Il non vedente guarda con Ctrl+D e ascolta la ROTN. Ognuno gioca la sua mossa.
- Passo 3, misura: Per ogni gruppo l'insegnante prende tempo medio per posizione e numero di riuscite. Riuscita uguale mossa uguale alla soluzione che ha già.
- Formula semplice: Divario uguale valore gruppo NV meno valore gruppo V, in valore assoluto. Si calcola sia per il tempo sia per le riuscite.
- Esempio: Tempo medio NV uguale 45 secondi. Tempo medio V uguale 40 secondi. Divario tempo uguale 5 secondi. Riuscite NV uguale 2 su 3. Riuscite V uguale 2 su 3. Divario riuscite uguale 0.
- Cosa fa l'insegnante, storico dei progressi: Per ogni compito annota data, tipo di compito, tempo medio NV, tempo medio V, riuscite NV, riuscite V, divario tempo, divario riuscite. Lo mette in uno storico dei progressi, data dopo data. La prossima volta, prima di iniziare, guarda lo storico e controlla se il divario si sta chiudendo.
- Perché conta: Se con lo stesso compito e la stessa notazione le prestazioni si avvicinano, allora la differenza non era nella mente. Era nello strumento. Se il divario si chiude, lo strumento ROTN sta funzionando.

- Numero metrica: 19
- Nome: Open Metric, Trasparenza
- Cosa misura: La metrica è calcolabile dall'allievo, non segreta
- Da dove viene: Estetica ed etica
- Come si misura in Scacchi-it: Con il principio: non ti vendo un punteggio segreto. Ti insegno a calcolarlo
- Come si fa, passo per passo:
- Passo 1, formula visibile: L'insegnante mostra la formula della metrica, per esempio SE, CMM, Stabilità in Zeitnot. La scrive in formato accessibile.
- Passo 2, dati visibili: L'allievo ha i suoi dati nello storico dei progressi, in formato accessibile. Se vedente legge con gli occhi, se non vedente legge con screen reader. Lo storico non è un quaderno di carta, è un foglio digitale accessibile a tutti e due.
- Passo 3, ricalcolo: L'allievo ricalcola da solo la metrica con la sua calcolatrice, partendo dai dati dello storico. Senza usare il computer dell'insegnante.
- Passo 4, controllo: L'insegnante controlla se il risultato dell'allievo è uguale al suo. Se è uguale, la metrica è trasparente. Se è diverso, si guarda insieme dove si è sbagliato.
- Formula semplice: Trasparenza uguale numero di metriche ricalcolate da solo correttamente diviso numero di metriche provate, per 100.
- Esempio: L'allievo prova a ricalcolare 4 metriche. Ne ricalcola 3 correttamente. Calcolo: 3 diviso 4 per 100 uguale 75%. Trasparenza uguale 75%.
- Cosa fa l'insegnante, storico dei progressi: Per ogni verifica annota data, metrica provata, ricalcolo riuscito si o no, trasparenza. Lo mette in uno storico dei progressi, data dopo data. La prossima volta, prima di iniziare, guarda lo storico e controlla se l'allievo sta diventando autonomo nel calcolare le sue metriche.
- Perché conta: Questa non è una metrica tra le altre. È la regola di tutte le altre. Se non puoi calcolarla da solo, non è una metrica di Scacchi-it.

- Numero metrica: 20
- Nome: Woodpecker a Casa, Ripetizione Tattica
- Cosa misura: Quanto diventi più veloce e più preciso ripetendo lo stesso set di posizioni tattiche, dimezzando il tempo a ogni giro
- Da dove viene: Metodo Woodpecker, Smith e Tikkanen, Pump Up Your Rating. Versione a casa della metrica 15, Scaffolding Efficace
- Come si misura in Scacchi-it: Con stesso set fisso, risolto a casa in più giri, con orologio offline, confronto tra primo giro e ultimo giro
- Regola generale, vale per tutti i giri: Se in un giro superi il tempo assegnato per quel giro, il giro non vale. Devi ricominciare da capo lo stesso set, dall'inizio, con l'orologio azzerato. Il tentativo fallito per superamento tempo non si conta nella precisione e non si conta nello storico come giro valido. Si conta solo quante volte hai dovuto ricominciare.
- Come si fa, passo per passo:
- Passo 1, set e tempo di base: L'insegnante prepara un set fisso di posizioni tattiche, per esempio 20 posizioni, stesso tema o temi misti. Ha già la soluzione di tutte e fornisce le soluzioni agli allievi. Stesso set per tutti i giri, non si cambia. L'insegnante assegna anche il tempo di base per il giro 1, per esempio 30 minuti per tutto il set.
- Passo 2, giro 1, a casa, con orologio offline: A casa l'allievo lancia l'orologio offline con clock-start, non l'orologio di rete. È l'orologio tiflologico, ha i suoi comandi e ti legge lui il tempo a voce. Spazio una sola volta all'inizio per avviarlo, e Spazio una sola volta alla fine per fermarlo. Non si fa cambio turno tra una posizione e l'altra, perché si è da soli. Poi carica le posizioni una per una con Ctrl+P. Per ogni posizione guarda. Se vedente guarda con gli occhi, se non vedente usa Ctrl+D e ascolta la ROTN. Gioca la mossa che pensa sia giusta. Annota riuscita si o no. Riuscita uguale mossa uguale alla soluzione che ha già l'insegnante. Il tempo totale del giro comprende anche il tempo per caricare le posizioni con Ctrl+P. Alla fine ferma l'orologio e annota il tempo totale del giro. Questo è il tempo base realizzato.
- Passo 3, pausa: L'allievo lascia passare 2 o 3 giorni, senza guardare le soluzioni.
- Passo 4, giro 2, a casa, metà tempo: L'insegnante assegna il tempo per il giro 2: metà del tempo assegnato al giro 1, non metà del tempo realizzato. Per esempio se il tempo assegnato al giro 1 era 30 minuti, il tempo assegnato al giro 2 è 15 minuti, anche se nel giro 1 avevi realizzato 28 minuti. A casa l'allievo rifà lo stesso set, con orologio offline e Ctrl+P e Ctrl+D, nello stesso modo. Obiettivo: finire dentro i 15 minuti. Niente CoachFEN. Annota riuscita e tempo totale.
- Passo 5, giro 3, a casa, ancora metà tempo: Dopo altri 2 o 3 giorni, l'insegnante assegna il tempo per il giro 3: metà del tempo assegnato al giro 2. Per esempio da 15 minuti assegnati a 7 minuti e 30 secondi assegnati. A casa l'allievo rifà lo stesso set una terza volta, con orologio offline, nello stesso modo. Niente CoachFEN. Annota riuscita e tempo totale.
- Passo 6, si continua così, con freno: Si può continuare con giro 4, giro 5, fino a 7 giri, come nel metodo originale. Ogni giro obiettivo metà del tempo assegnato al giro prima. Ci si ferma prima se il tempo assegnato scende sotto 1 minuto per posizione, oppure se la precisione scende. Con 20 posizioni, ci si ferma quando il tempo assegnato scende sotto 20 minuti totali. Non è un fallimento fermarsi. È il limite del metodo con questo set.
- Formula semplice: Precisione uguale riuscite diviso numero posizioni del set, per 100. Miglioramento tempo uguale tempo totale giro 1 meno tempo totale ultimo giro, diviso tempo totale giro 1, per 100. Nel miglioramento tempo si usano i tempi realizzati, non i tempi assegnati.
- Esempio: Set di 20 posizioni. Tempo di base assegnato giro 1: 30 minuti. Giro 1: tempo totale realizzato 28 minuti, dentro il tempo, riuscite 10 su 20. Calcolo precisione: 10 diviso 20 per 100 uguale 50%. Giro 3, tempo assegnato 7 minuti e 30 secondi: tempo totale realizzato 7 minuti, dentro il tempo, riuscite 17 su 20. Calcolo precisione: 17 diviso 20 per 100 uguale 85%. Calcolo miglioramento tempo: 28 meno 7 uguale 21. 21 diviso 28 per 100 uguale 75%. Miglioramento tempo uguale 75%, precisione da 50% a 85%.
- Cosa fa l'insegnante, storico dei progressi: Per ogni set annota data, tema, tempo assegnato giro 1, tempo realizzato giro 1, tempo assegnato giro 2, tempo realizzato giro 2, tempo assegnato giro 3, tempo realizzato giro 3, precisione giro 1, precisione ultimo giro, miglioramento tempo, quante volte ha dovuto ricominciare da capo per superamento tempo. I tentativi falliti per superamento tempo non si contano come giri validi. Lo mette in uno storico dei progressi, data dopo data. La prossima volta, prima di iniziare, guarda lo storico e controlla se tempo scende e precisione sale sullo stesso set.
- Perché conta: La prima volta ragioni, l'ultima volta riconosci. Se sullo stesso set diventi più veloce e più preciso, la tattica non la stai solo risolvendo. La stai imparando. E come dice Tikkanen, questo ti dà più stabilità in Zeitnot, migliore visione tattica e meno errori. Che è proprio la metrica 16. E lo puoi fare a casa, da solo, senza insegnante.

## Conclusione

Non misuriamo per giudicare. Misuriamo per rendere autonomi.
Ogni numero qui è un modo per dire all'allievo, guarda, questo lo puoi vedere anche da solo: le metriche mostrano i tuoi progressi se li registri in uno storico dei progressi.

Le metriche qui indicate sono solo una base iniziale, a cui si possono aggiungere altre anche mirate all'allievo per limare il suo carattere: la costanza e l'allenamento affinano la memoria della posizione, eliminano i difetti iniziali e aumentano la determinazione, l'autostima, la resilienza, il coraggio. Anche la partecipazione a tornei abitua a sopportare lo stress e l'ansia. Il segreto è trovare un bilancio tra un carico sostenibile e un buon divertimento.
Accanto a tutto questo, per creare un buon giocatore di valore, è insostituibile lo studio efficace di Strategia, Tattica, aperture e finali.

Per gli allievi non vedenti la chiave di volta è lo studio a casa con strumenti che li rendano autonomi nel gioco e nello studio, contando su una documentazione (dispense, libri, ebook, epub) consultabile con screen reader in modo semplice e facile.
