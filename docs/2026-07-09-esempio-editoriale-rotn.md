---
title: "esempio editoriale per come inserire la rotn in un articolo"
description: "blog di Rosario Turco con downloads di giochi gratuiti per non vedenti"
keywords:
  [
    "Articoli",
    "scacchi",
    "gratuiti",
    "gratis",
    "non vedenti",
    "ciechi",
    "ipovedenti",
  ]
categories: Blog
---

# Come inserire ROTN nei tuoi contenuti

di Rosario Turco

Nel seguito si elencano varie soluzioni per pubblicare un articolo di scacchi per non vedenti.

ROTN è la Rot Notation, un formato testuale ASCII per descrivere posizioni scacchistiche accessibili agli screen reader come NVDA.

Per non interrompere la lettura, la posizione ROTN non va messa nel testo. Va messa in un file separato e linkata.

un template HTML da compilare una sola volta per ogni posizione.

_Passo 1: Scarica il template_  
[Scarica template-html](https://github.com/RedYouMan/redyouman.github.io/raw/main/_posts/templates/template-html.7z)  
È un file base. Fai unzip e ottieni il file html.

_Passo 2: Compila e Rinomina_  
Apri il file con Blocco Note. Incolla la tua posizione nella textarea.  
Salvalo subito con un nome descrittivo. Esempi:
`carlsen-nakamura-2025-34.html` oppure `italia-2026-stallo.html`  
Importante: non lasciare mai il nome `template.html`

Modifica i dati del file.
Nota: Il link dell'esempio sotto su questa pagina non verrà aperto, ma serviva a farvi comprendere come sfruttarlo: provate sul vostro sito.

_Passo 3: Carica il file nella cartella del sito e Linka nel Markdown_  
Carica il file nella cartella del tuo sito.  
Nel tuo articolo scrivi così:
Il Bianco muove e vince in 2.
[Vedi e Copia posizione ROTN: Carlsen vs Nakamura m.34](/rotn/carlsen-nakamura-2025-34.html)
Cliccando si apre una pagina semplice con 1 textarea e 1 bottone "Copia". L'utente copia e torna all'articolo. NVDA legge tutto correttamente.

## 2. Flusso di lavoro per eBook ePub e PDF accessibile

Negli eBook il javascript può non funzionare. Usiamo gli allegati .txt

_Passo 1: Scarica e Compila il template_  
[Scarica template-ebook](https://github.com/RedYouMan/redyouman.github.io/raw/main/_posts/templates/template-ebook.7z)  
Fai unzip, compila i dati e salvalo con un nome diverso dal template e che sia descrittivo.  
Nome esempio: `cap3-pos2.rotn.txt`

_Passo 2: Inseriscilo nell'eBook_  
Allega il file tra le risorse dell'ePub.

_Passo 3: Linkalo nel testo_
Analizziamo la posizione.
<a href="cap3-pos2.rotn.txt" download>Scarica file ROTN </a>
Il lettore scarica e apre il .txt con NVDA.

## 3. Flusso di lavoro perprendere una posizione da Lichess/Chess.com e trasformarla in file.txt ROTN

Obiettivo: prendere una posizione da Lichess/Chess.com e trasformarla in file.txt ROTN per il tuo ebook.

#### _Cosa ti serve_

1.  _fenpos.exe_: scaricalo da qui: `[scarica fenpos](https://github.com/RedYouMan/redyouman.github.io/raw/main/_posts/repolc/fenpos.exe)
2.  _Una cartella_ sul Desktop, chiamala `ROTN_Ebook`

#### _Flusso di lavoro per l'editore_

_Passo 1: Prendi la posizione dal portale_

1.  Vai su Lichess.org → Analysis Board o Chess.com → Analysis
2.  Monta la posizione del tuo diagramma
3.  Cerca il tasto `Copia FEN`. Cliccalo.  
    _Esempio FEN copiato:_ `r1bqkb1r/pppp1ppp/2n2n2/4p3/4P3/2N2N2/PP1PPP/R1BQKB1R w KQkq - 0 1`

_Passo 2: Prepara i dati_
Ti servono 3 cose. Scrivetele su un blocco note per comodità:

1.  _Titolo_: `Apertura Italiana p.23`
2.  \_Num posizione o problema `21`
3.  _FEN_: incolla quello copiato al passo 1

_Passo 3: Lancia fenpos.exe_

1.  Metti `fenpos.exe` dentro la cartella `ROTN_Ebook`
2.  Nella cartella tieni premuto `Shift` + Tasto destro → `Apri finestra PowerShell qui`
3.  Incolla questo comando, cambiando i dati tra virgolette e il numero:
    .\fenpos.exe "Apertura Italiana p.23" 21 "r1bqkb1r/pppp1ppp/2n2n2/4p3/4P3/2N2N2/PP1PPP/R1BQKB1R w KQkq - 0 1"
4.  Premi `Invio`

_Fatto._  
Nella cartella `ROTN_Ebook` ti trovi il file: `pos_21.txt`  
Aprilo e incolla il contenuto nel tuo ebook.

#### _Note importanti per l'editore_

1.  _Le virgolette sono obbligatorie_ intorno a Titolo, Chi_muove e FEN
2.  _Num Problema_: se hai 10 diagrammi usa 001, 002, 003... così sono ordinati
3.  _Errori_: se esce "comando non trovato" significa che non sei nella cartella giusta di `fenpos.exe`

## Alternativa molto semplice

Negli ebook si possono continuare a mettere i diagrammi per i normodotati, con ALT sintetico ma fuori diagramma si può mettere la ROTN con la tecnica salva-spazio di caratteri di dimensioni minori che comunque sono leggibili da uno screen reader e possono subire, facilmente, il copia e incolla per Scacchi-it e applicativi similari.

Non mettere i diagrammi è vero che si soddisfa il non vedente ma è una eresia per i normodotati.

Buon Lavoro!

