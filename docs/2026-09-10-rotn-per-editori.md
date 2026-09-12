---
title: "ROTN v1.9: Proposta di Come rendere accessibili i diagrammi scacchistici"
description: "Documento tecnico di proposta di come ROTN v1.9 sia utilizzabile nella Editoria come standard aperto per l'alternativa testuale ai diagrammi di scacchi. Come implementare alt + ROTN in EPUB 3"
keywords:
  [
    "ROTN",
    "ROTN v1.9",
    "diagrammi scacchistici accessibili",
    "EAA 82/2022",
    "D.Lgs 82/2022",
    "WCAG 1.1.1",
    "EN 301 549",
    "EPUB accessibile",
    "scacchi non vedenti",
    "alt text scacchi",
    "FEN accessibile",
    "Rosario Turco",
    "Scacchi-it",
  ]
categories: ["Blog", "Accessibilità", "ForDevelopers"]
author: "Rosario Turco"
date: 2026-07-02
license: "CC BY-SA 4.0 + GPL v3"
---

DOCUMENTO TECNICO - proposta per diagrammi scacchistici

Titolo: ROTN v1.9 proposta come alternativa testuale
Autore: Rosario Turco - Progetto Scacchi-it (CC BY-SA 4.0 + GPL v3)
Data: 2026-07-02 - Spec v1.9

1. Problema
   Il FEN come alt-text (r1bqkbnr/pppp...) non e' leggibile da NVDA/JAWS. Viola il principio di equivalente comprensibile.

2. Soluzione proposta: ROTN
   Ordine fisso T;V;B;N;M;P; in italiano, T;V;W;B;M;P; in inglese
   T = testo posizione
   V = vista
   B/N = lista pezzi formato Lettera+casella
   M = tratto
   P = ultimo numero di mossa giocato dal Bianco
   U = en-passant opzionale, Z = semimosse opzionale

Esempio:
T:Matto in 2 mosse
V:B;
B:Re1;Dd1;
N:Re8;
M:B;
P:12;

3. Implementazione EPUB 3 / HTML5 a norma
   Usare figure con figcaption, img con alt breve, e pre con ROTN esteso. Questo passa Ace by DAISY.

4. Confronto
   FEN illeggibile da NVDA. ROTN lettura naturale tramite T+M+P. Validazione con max 16 pezzi e re obbligatorio.

5. Per editore a norma 82/2022
   Fornire alt sintetico + blocco ROTN + validazione Ace.

   Si consiglia, prima di utilizzare questa proposta di soluzione, di consultare l'autorità competente (Agid).
