TUNING

Con il programma scacchi è possibile fare un minimo di tuning tra Windows e chess engine. Questa attività riguarda solo se attivate l'engine con CntrlZ per giocarvi contro, 

in particolare si può agire, nella directory bin, sul file chessparam.cnf, nell'ordine di configurazione sui parametri:
- maxThreads
Esso è consigliato di essere al massimo pari al numero di core del pc disponibile.
- maxHashTable
Essa è la massima dimensione rappresentata con un numero intero espresso in mega byte, scelto considerando al massimo la metà della RAM libera.
Tale valore è memoria RAM che si da a disposizione ai threads per elaborare in parallelo le attività di analisi del chess engine.
Se non si settano threads non si setta nemmeno la hashTable.
- MultiPV , 
un intero maggiore di 0 che richiede all'engine di restituire le prime 'multiPV' principali varianti migliori di analisi nell'albero considerato.

Il file chessparam.cnf  ha un valore per riga. Normalmente i valori a0 sono buoni per la maggior parte dei computer, se gli utenti non particolari esigenze.
Se i valori in esso sono tutti 0 si demanda al chess engine di lavorare con i valori suoi di default.
Il programma scacchi una volta letto i valori da chessparam.cnf, se essi sono maggiori di 0, li passa con comandi uci standard al chess engine.

Ad esempiocon un computer 8 core, 32 Gbyte di RAM di cui almeno 16Gbyte  libera, possibili valori potrebbero essere 
4
12
5

Ovviamente il secondo valore è 16 Mbyte, ovvero ho dato 2 Mbyte a threads.
Una volta che da Inpostazioni di Windows venite a conoscenza di cpu (core),ram libera potreste decidere quali valori settare. 
Nota: se non siete certi dei valori da utilizzare, lasciate i valori di default, che non agiscono sul chess engine.
E' vostra responssabilità quali valori inserite nel file di configurazione e non dell'engine stockfish.

Consigliamo almeno i tre valori 0, 0, 5 comefile configurato di default. 

Buon divertimento.

  