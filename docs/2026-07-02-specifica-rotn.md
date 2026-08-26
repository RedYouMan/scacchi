---
title: "scacchi accessibili: Specifica ROTN - Rot Notation "
description: "Una specifica per notazione ROTN negli scacchi"
categories: Blog
---

# ROTN v1.8 - Rot Notation Specification

# AUTHOR: Rosario Turco

# DATE: 2026-07-09

## [IT] Specifica Italiana

_ROTN_ = *Ro*T *N*otation
Formato testuale ASCII per descrivere posizioni scacchistiche accessibili per screen reader.

_Regole Generali:_

1. _Estensione_: `.txt`
2. _Codifica_: UTF-8
3. _Ordine Fisso_: `T;V;B;N;M;P;`
4. _1 riga = 1 Descrittore_
5. _Separatore_: `;` tra i pezzi e non obbligatorio alla fine di ogni riga
6. _Grammatica_: conforme a EBNF ISO/IEC 14977
7. rotazione posizione con V, sintesi vocale per T, M, P, U e Z.
8. con ordine fisso se dopo V incontro B è versione italiana, se incontro W è versione inglese
9. utilità 1: molti test rapidi di posizioni complesse di un applicativo scacchistico
10. utilità 2: interscambio dati tra formati diversi tra piattaforme
11. utilità 3: aggiunta di testo e rotazione scacchiera

_Descrittori Obbligatori - Ordine fisso:_
`T;V;B;N;M;P;`

_Descrittore opzionale U alla fine_ - elenco case senza separatore ';', dei pedoni che possono eseguire la cattura enpassant e per ultima la casa del pedone che può essere catturato (es. `U:d4f4e4;`). U diventa utile negli automatismi di sospensione e ripresa partita. Esiste solo se ci sono i pedoni affiancati.

_Descrittore opzionale Z alla fine_ - numero di semimosse (campo 5 fen)

_Descrittore riservato F_ - per ripresa scrittura log della partita  
_Descrittore riservato E_ - per startare un engine alla ripresa della partita  
_Descrittore riservato K_ - livello di skill dell'engine alla ripresa  
_Descrittore riservato L_ - livello di profondità analisi dell'engine alla ripresa  
_Descrittore riservato C_ - per salvataggio mossa se utile per la ripresa
**Descrittore** | **Significato** | **Valori / Esempio**
**T** | Testo della posizione | `T:Matto in 2 mosse`
**V** | Punto di vista / Lato | `V:B;` = Guardi dal Bianco, `V:N;` = dal Nero
**B** | Posizione Pezzi Bianchi | Lista `Pezzoxx` separati da `;`
**N** | Posizione Pezzi Neri | Lista `Pezzoxx` separati da `;`
**M** | Chi muove | `M:B;` oppure `M:N;`
**P** | Numero di mossa n della posizione | `P:1;`
_Notazione Pezzi IT:_ `R`=Re, `D`=Donna, `T`=Torre, `A`=Alfiere, `C`=Cavallo, `P`=Pedone.  
_Notazione Pezzi EN:_ `K`=King, `Q`=Queen, `R`=Rook, `B`=Bishop, `N`=Knight, `P`=Pawn.  
_Coordinate:_ notazione algebrica `a1..h8`

### Regola di sintesi Vocale

1. Lettura di T
2. lettura numero mossa P
3. dichiarazione di chi ha la mossa in base a M
4. dichiarazione della presenza dell'enpassant
5. dichiarazione di Z per il valore di semimosse, se esso è maggiore di zero.

### Regole di Validazione

1. `T` non può essere vuota
2. `V` e `M` devono essere `B` o `N`
3. `B` e `N` devono contenere max 16 pezzi
4. Non possono esser 2 pezzi sulla stessa casa
5. Ogni pezzo deve essere nel formato `Lettera+a1..h8`
6. Ci devono essere un re per colore
7. Deve esserci ordine fisso: T,V,B,N,M,P
8. La lista dei pezzi può contenere uno o entrambi i valori NS o NL, per casi ambigui di arrocco corto o lungo non valido

### [IT] Esempio 1: Posizione Iniziale

T:Posizione iniziale
V:B;
B:Ta1;Cb1;Ac1;Dd1;Re1;Af1;Cg1;Th1;Pa2;Pb2;Pc2;Pd2;Pe2;Pf2;Pg2;Ph2;
N:Ta8;Cb8;Ac8;Dd8;Re8;Af8;Cg8;Th8;Pa7;Pb7;Pc7;Pd7;Pe7;Pf7;Pg7;Ph7;
M:B;
P:1;

### [IT] Esempio 2: Stallo.

T:Stallo
V:B;
B:Re6;Pe7;
N:Re8;
M:N;
P:35;

## [EN] International Specification

ROTN is an ASCII text format for accessible chess positions.

_General Rules:_

1. _Extension_: `.txt`
2. _Encoding_: UTF-8
3. _Fixed Order_: `T;V;W;B;M;P;`
4. _1 line = 1 Descriptor_
5. _Separator_: `;` between pieces and not mandatory at end of each line
6. _Grammar_: compliant with EBNF ISO/IEC 14977
7. position rotation with V, speech synthesis for T, M, P, U and Z.

_Mandatory Descriptors - Fixed Order:_
`T;V;W;B;M;P;`

_Optional Descriptor U_ - list of square, without of ';',of pawns that can perform enpassant and the square of pawn under enpassant attack (eg. `U:d4f4e4;`). U becomes useful in the automation of match suspension and resumption. It exists if there are connected pawns.

_Optional Descriptor Z_ - number of halfmoves (field 5 fen), if it's greater than zero.

_Restricted Descriptor F_ - resume writing game log  
_Restricted Descriptor E_ - restart engine at game resumption  
_Restricted Descriptor K_ - engine skill to be restarted at resume  
_Restricted Descriptor L_ - engine depth  
_Restricted Descriptor C_ - for data engine if useful
**Descriptor** | **Meaning** | **Values / Example**
**T** | Position Text | `T:Mate in 2`
**V** | Viewpoint / Side | `V:W;` = View from White, `V:B;` = from Black
**W** | White Pieces Position | List `Piecexx` ; separated
**B** | Black Pieces Position | List `Piecexx` ; separated
**M** | Move Side | `M:W;` or `M:B;`
**P** | Move Number n of the position | `P:1;`

### [EN] Example 1: Starting Position

T:Starting Position
V:W;
W:Ra1;Nb1;Bc1;Qd1;Ke1;Bf1;Ng1;Rh1;Pa2;Pb2;Pc2;Pd2;Pe2;Pf2;Pg2;Ph2;
B:Ra8;Nb8;Bc8;Qd8;Ke8;Bf8;Ng8;Rh8;Pa7;Pb7;Pc7;Pd7;Pe7;Pf7;Pg7;Ph7;
M:W;
P:1;

### [EN] Example 2: StaleMate.

T:StaleMate
V:W;
W:Ke6;Pe7;
B:Ke8;
M:B;
P:35;

## Parsing and other

read file line by line  
Descriptor = first letter before :  
With fix order, if W is after V it is english version. If B after V it is italian version.  
value = everything after :  
`V` and `M` must be `W` or `B`  
`W` and `B` must contain at most 16 pieces  
"The piece list can contain one or both of the NS or NL values, for ambiguous cases of invalid kingside or queens ide castling.  
very useful in complex positioning tests with heavy time savings.  
possible data exchange between different formats to and from a chess application.  
check order: T,V,W,B,M,P  
split W and B with ;  
check duplicate coordinates
