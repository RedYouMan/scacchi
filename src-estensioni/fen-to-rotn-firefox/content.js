
let fenList = [];

function findFENs() {
  const fenRegex = /([rnbqkpRNBQKP1-8]+\/[rnbqkpRNBQKP1-8]+\/[rnbqkpRNBQKP1-8]+\/[rnbqkpRNBQKP1-8]+\/[rnbqkpRNBQKP1-8]+\/[rnbqkpRNBQKP1-8]+\/[rnbqkpRNBQKP1-8]+\/[rnbqkpRNBQKP1-8]+)\s([wb])\s([KQkq-]+)\s([a-h1-8-]+)\s(\d+)\s(\d+)/g;
  fenList = [...document.body.innerText.matchAll(fenRegex)].map(m => m[0]);
}

function speak(text, lang='it-IT') {
  return new Promise(resolve => {
    speechSynthesis.cancel();
    const utterance = new SpeechSynthesisUtterance(text);
    utterance.lang = lang;
    utterance.rate = 0.95;
    utterance.onend = () => resolve();
    speechSynthesis.speak(utterance);
  });
}

function buildEnPassantTrace(enpassant, board, turnF) {
  if(enpassant === '-') return '-';
  const files = 'abcdefgh';
  const epFile = enpassant[0];
  const epRank = parseInt(enpassant[1]);
  const pawnRank = turnF === 'w'? epRank + 1 : epRank - 1;
  const pawnSq = epFile + pawnRank;
  const checkRank = pawnRank;
  const fileIndex = files.indexOf(epFile);
  const leftFile = fileIndex > 0? files[fileIndex - 1] : null;
  const rightFile = fileIndex < 7? files[fileIndex + 1] : null;
  let attackers = [];
  const enemyPawn = turnF === 'w'? 'p' : 'P';
  const boardRows = board.split('/');
  const rowIndex = 8 - checkRank;
  if(rowIndex < 0 || rowIndex > 7) return '-';
  const rowStr = boardRows[rowIndex];
  let col = 0;
  for(let c of rowStr){
    if(!isNaN(c)){ col += parseInt(c); continue; }
    if(leftFile && col === files.indexOf(leftFile) && c === enemyPawn) attackers.push(leftFile + checkRank);
    if(rightFile && col === files.indexOf(rightFile) && c === enemyPawn) attackers.push(rightFile + checkRank);
    col++;
  }
  if(attackers.length === 0) return '-';
  return attackers.join('') + pawnSq;
}

function parseFEN(fen) {
  const [board, turnF, castling, enpassant, halfmoves, moveNum] = fen.split(' ');
  const V = turnF === 'w'? 'B' : 'N';
  const M = V;
  const mossaFen = parseInt(moveNum);
  const P = turnF === 'w'? mossaFen : mossaFen;
  const lang = 'it-IT';
  const files = 'abcdefgh';
  const nomi = {K:'R', Q:'D', R:'T', B:'A', N:'C', P:'P'};
  let B_re = '-', N_re = '-';
  let B_tutti = [], N_tutti = [];
  let row = 8, fileIndex = 0;
  for(let c of board){
    if(c==='/'){row--;fileIndex=0;continue;}
    if(!isNaN(c)){fileIndex+=parseInt(c);continue;}
    const sq = files[fileIndex] + row;
    const pezzo = nomi[c.toUpperCase()];
    const pezzoCoord = pezzo + sq + ';';
    if(c === 'K') B_re = sq;
    if(c === 'k') N_re = sq;
    if(c === c.toUpperCase()) B_tutti.push(pezzoCoord);
    else N_tutti.push(pezzoCoord);
    fileIndex++;
  }
  let B_ar = '', N_ar = '';
  if(!castling.includes('K')) B_ar += 'NS;';
  if(!castling.includes('Q')) B_ar += 'NL;';
  if(!castling.includes('k')) N_ar += 'NS;';
  if(!castling.includes('q')) N_ar += 'NL;';
  return {
    V, lang,
    B: `R${B_re};${B_ar}`,
    N: `R${N_re};${N_ar}`,
    B_tutti: B_tutti.join(''),
    N_tutti: N_tutti.join(''),
    M, P, U: buildEnPassantTrace(enpassant, board, turnF), Z: halfmoves, board, moveNum: mossaFen
  };
}

function fenToROTN_COMPLETA(data) {
  let righe = [];
  righe.push(`T:Testo non disponibile;`);
  righe.push(`V:${data.V};`);
  righe.push(`B:${data.B_tutti}`);
  righe.push(`N:${data.N_tutti}`);
  righe.push(`M:${data.M};`);
  righe.push(`P:${data.P};`);
  if(data.U!== '-') righe.push(`U:${data.U};`);
  if(parseInt(data.Z) > 0) righe.push(`Z:${data.Z};`);
  return righe.join('\n');
}

function fenToSpoken_RIASSUNTO(data) {
  let castlingText = '';
  if(data.B.includes('NS')) castlingText += 'Arrocco corto bianco non consentito. ';
  if(data.B.includes('NL')) castlingText += 'Arrocco lungo bianco non consentito. ';
  if(data.N.includes('NS')) castlingText += 'Arrocco corto nero non consentito. ';
  if(data.N.includes('NL')) castlingText += 'Arrocco lungo nero non consentito. ';
  if(castlingText === '') castlingText = 'Tutti gli arrocchi consentiti. ';
  let summary = `Testo non disponibile. `;
  summary += `Lato scacchiera ${data.V==='B'?'Bianco':'Nero'}. `;
  summary += `Bianco: Re in ${data.B.split(';')[0].substring(1)}. `;
  summary += `Nero: Re in ${data.N.split(';')[0].substring(1)}. `;
  summary += castlingText;
  summary += `Mossa numero ${data.moveNum}. Muove il ${data.M==='B'?'Bianco':'Nero'}. `;
  if(data.U!=='-') summary += `Possibile En Passant. `;
  summary += `Vuoi che ti dica tutta la lista dei pezzi bianchi e neri? S e Invio per Si. Solo Invio per No.`;
  return summary;
}

function getPieceListIT_B_N(data) {
  const nomi = {R:'Re', D:'Donna', T:'Torre', A:'Alfiere', C:'Cavallo', P:'Pedone'};
  let list = "Lista completa pezzi Bianchi: ";
  data.B_tutti.split(';').filter(x=>x).forEach(p => {
    list += `${nomi[p[0]]} in ${p.substring(1)}. `;
  });
  list += "Lista completa pezzi Neri: ";
  data.N_tutti.split(';').filter(x=>x).forEach(p => {
    list += `${nomi[p[0]]} in ${p.substring(1)}. `;
  });
  return list;
}

async function runConversion() {
  findFENs();
  if(!fenList.length){ await speak("Nessuna FEN trovata"); return; }
  const data = parseFEN(fenList[0]);
  const rotnFile = fenToROTN_COMPLETA(data);
  const spoken = fenToSpoken_RIASSUNTO(data);
  navigator.clipboard.writeText(rotnFile);
  await speak(spoken, data.lang);
  speechSynthesis.cancel();
  await new Promise(r => setTimeout(r, 300));
  const risposta = prompt("Vuoi che ti dica tutta la lista dei pezzi bianchi e neri? S + Invio per Si. Solo Invio per No");
  if(risposta && risposta.toLowerCase() === 's') {
    const lista = getPieceListIT_B_N(data);
    await speak(lista, data.lang);
  } else {
    await speak("Ok. ROTN negli appunti.", data.lang);
  }
}

browser.runtime.onMessage.addListener((req) => {
  if(req.action==="convert") {
    setTimeout(runConversion, 500);
  }
});
