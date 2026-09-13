---
title: "scacchi accessibili: Specifica ROTN - Readable Ordered Text Notation"
description: "Una specifica per notazione ROTN negli scacchi"
categories: Blog
---

# ROTN v1.9 - Readable Ordered Text NotationSpecification

# AUTHOR: Rosario Turco

# DATE: 2026-07-02

# LICENSE: CC BY-SA 4.0 + GPL v3

## [IT] Specifica Italiana

ROTN = Readable Ordered Text Notation.
Formato testuale ASCII per descrivere posizioni scacchistiche accessibili per screen reader.

Regole Generali:

1. Estensione: .txt
2. Codifica: UTF-8
3. Ordine Fisso: T;V;B;N;M;P;
4. 1 riga = 1 Descrittore
5. Separatore: ; tra i pezzi e non obbligatorio alla fine di ogni riga
6. Grammatica: conforme a EBNF ISO/IEC 14977
7. Rotazione posizione con V, sintesi vocale per T, M, P, U e Z.
8. Con ordine fisso se dopo V incontro B è versione italiana, se incontro W è versione inglese
9. Utilità 1: molti test rapidi di posizioni complesse di un applicativo scacchistico
10. Utilità 2: interscambio dati tra formati diversi tra piattaforme
11. Utilità 3: aggiunta di testo e rotazione scacchiera

Descrittori Obbligatori - Ordine fisso:
T;V;B;N;M;P;

Descrittore opzionale U alla fine - elenco case senza separatore ';', dei pedoni che possono eseguire la cattura enpassant e per ultima la casa del pedone che può essere catturato (es. U:d4f4e4;). U diventa utile negli automatismi di sospensione e ripresa partita. Esiste solo se ci sono i pedoni affiancati.

Descrittore opzionale Z alla fine - numero di semimosse (campo 5 fen)

Descrittore riservato F - per ripresa scrittura log della partita
Descrittore riservato E - per startare un engine alla ripresa della partita
Descrittore riservato K - livello di skill dell'engine alla ripresa
Descrittore riservato L - livello di profondità analisi dell'engine alla ripresa
Descrittore riservato C - per salvataggio mossa se utile per la ripresa

| Descrittore | Significato                               | Valori / Esempio                          |
| ----------- | ----------------------------------------- | ----------------------------------------- |
| T           | Testo della posizione                     | T:Matto in 2 mosse                        |
| V           | Punto di vista / Lato                     | V:B; = Guardi dal Bianco, V:N; = dal Nero |
| B           | Posizione Pezzi Bianchi                   | Lista Pezzoxx separati da ;               |
| N           | Posizione Pezzi Neri                      | Lista Pezzoxx separati da ;               |
| M           | Chi muove                                 | M:B; oppure M:N;                          |
| P           | ultimo Numero di mossa giocato dal bianco | P:1;                                      |

Notazione Pezzi IT: R=Re, D=Donna, T=Torre, A=Alfiere, C=Cavallo, P=Pedone.
Notazione Pezzi EN: K=King, Q=Queen, R=Rook, B=Bishop, N=Knight, P=Pawn.
Coordinate: notazione algebrica a1..h8

### Regola di sintesi Vocale

1. Lettura di T
2. Lettura numero mossa P
3. Dichiarazione di chi ha la mossa in base a M
4. Dichiarazione della presenza dell'enpassant
5. Dichiarazione di Z per il valore di semimosse, se esso è maggiore di zero.

### Regole di Validazione

1. T non può essere vuota
2. V e M devono essere B o N
3. B e N devono contenere max 16 pezzi
4. Non possono esser 2 pezzi sulla stessa casa
5. Ogni pezzo deve essere nel formato Lettera+a1..h8
6. Ci devono essere un re per colore
7. Deve esserci ordine fisso: T,V,B,N,M,P
8. La lista dei pezzi può contenere uno o entrambi i valori NS o NL, per casi ambigui di arrocco corto o lungo non valido

### [IT] Esempio 1: Posizione Iniziale

T:Posizione iniziale
V:B;
B:Ta1;Cb1;Ac1;Dd1;Re1;Af1;Cg1;Th1;Pa2;Pb2;Pc2;Pd2;Pe2;Pf2;Pg2;Ph2;
N:Ta8;Cb8;Ac8;Dd8;Re8;Af8;Cg8;Th8;Pa7;Pb7;Pc7;Pd7;Pe7;Pf7;Pg7;Ph7;
M:B;
P:0;

### [IT] Esempio 2: Stallo.

T:Stallo
V:B;
B:Re6;Pe7;
N:Re8;
M:N;
P:35;

## [EN] International Specification

ROTN is an ASCII text format for accessible chess positions.

General Rules:

1. Extension: .txt
2. Encoding: UTF-8
3. Fixed Order: T;V;W;B;M;P;
4. 1 line = 1 Descriptor
5. Separator: ; between pieces and not mandatory at end of each line
6. Grammar: compliant with EBNF ISO/IEC 14977
7. Position rotation with V, speech synthesis for T, M, P, U and Z.

Mandatory Descriptors - Fixed Order:
T;V;W;B;M;P;

Optional Descriptor U - list of square, without of ';',of pawns that can perform enpassant and the square of pawn under enpassant attack (eg. U:d4f4e4;). U becomes useful in the automation of match suspension and resumption. It exists if there are connected pawns.

Optional Descriptor Z - number of halfmoves (field 5 fen), if it's greater than zero.

Restricted Descriptor F - resume writing game log
Restricted Descriptor E - restart engine at game resumption
Restricted Descriptor K - engine skill to be restarted at resume
Restricted Descriptor L - engine depth
Restricted Descriptor C - for data engine if useful

| Descriptor | Meaning                          | Values / Example                          |
| ---------- | -------------------------------- | ----------------------------------------- |
| T          | Position Text                    | T:Mate in 2                               |
| V          | Viewpoint / Side                 | V:W; = View from White, V:B; = from Black |
| W          | White Pieces Position            | List Piecexx ; separated                  |
| B          | Black Pieces Position            | List Piecexx ; separated                  |
| M          | Move Side                        | M:W; or M:B;                              |
| P          | last Move Number played by white | P:1;                                      |

### [EN] Example 1: Starting Position

T:Starting Position
V:W;
W:Ra1;Nb1;Bc1;Qd1;Ke1;Bf1;Ng1;Rh1;Pa2;Pb2;Pc2;Pd2;Pe2;Pf2;Pg2;Ph2;
B:Ra8;Nb8;Bc8;Qd8;Ke8;Bf8;Ng8;Rh8;Pa7;Pb7;Pc7;Pd7;Pe7;Pf7;Pg7;Ph7;
M:W;
P:0;

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
V and M must be W or B
W and B must contain at most 16 pieces
The piece list can contain one or both of the NS or NL values, for ambiguous cases of invalid kingside or queenside castling.
very useful in complex positioning tests with heavy time savings.
possible data exchange between different formats to and from a chess application.
check order: T,V,W,B,M,P
split W and B with ;
check duplicate coordinates
