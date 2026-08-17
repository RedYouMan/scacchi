
let fenList = [];

function findFENs() {
  const fenRegex = /([rnbqkpRNBQKP1-8]+\/[rnbqkpRNBQKP1-8]+\/[rnbqkpRNBQKP1-8]+\/[rnbqkpRNBQKP1-8]+\/[rnbqkpRNBQKP1-8]+\/[rnbqkpRNBQKP1-8]+\/[rnbqkpRNBQKP1-8]+\/[rnbqkpRNBQKP1-8]+)\s([wb])\s([KQkq-]+)\s([a-h1-8-]+)\s(\d+)\s(\d+)/g;
  fenList = [...document.body.innerText.matchAll(fenRegex)].map(m => m[0]);
}

function speak(text) {
  window.speechSynthesis.cancel();
  const utterance = new SpeechSynthesisUtterance(text);
  utterance.lang = 'it-IT';
  utterance.rate = 0.9;
  window.speechSynthesis.speak(utterance);
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
  const P = turnF === 'w'? parseInt(moveNum) : parseInt(moveNum);
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
  return { V, B: `R${B_re};${B_ar}`, N: `R${N_re};${N_ar}`, B_tutti: B_tutti.join(''), N_tutti: N_tutti.join(''), M, P, U: buildEnPassantTrace(enpassant, board, turnF), Z: halfmoves, board, moveNum: parseInt(moveNum) };
}

function fenToROTN_COMPLETA(data) {
  let righe = [`T:Testo non disponibile;`, `V:${data.V};`, `B:${data.B_tutti}`, `N:${data.N_tutti}`, `M:${data.M};`, `P:${data.P};`];
  if(data.U!== '-') righe.push(`U:${data.U};`);
  if(parseInt(data.Z) > 0) righe.push(`Z:${data.Z};`);
  return righe.join('\n');
}

function getFullSpeechIT(data) {
  const nomi = {R:'Re', D:'Donna', T:'Torre', A:'Alfiere', C:'Cavallo', P:'Pedone'};
  let castlingText = '';
  if(data.B.includes('NS')) castlingText += 'Arrocco corto bianco non consentito. ';
  if(data.B.includes('NL')) castlingText += 'Arrocco lungo bianco non consentito. ';
  if(data.N.includes('NS')) castlingText += 'Arrocco corto nero non consentito. ';
  if(data.N.includes('NL')) castlingText += 'Arrocco lungo nero non consentito. ';
  if(castlingText === '') castlingText = 'Tutti gli arrocchi consentiti. ';
  let extraInfo = '';
  if(data.U!== '-') extraInfo += `Enpassant in ${data.U}. `;
  if(parseInt(data.Z) > 0) extraInfo += `Semimosse: ${data.Z}. `;
  let pezziB = "Pezzi bianchi: ";
  data.B_tutti.split(';').filter(x=>x).forEach(p => { pezziB += `${nomi[p[0]]} ${p.substring(1)}. `; });
  let pezziN = "Pezzi neri: ";
  data.N_tutti.split(';').filter(x=>x).forEach(p => { pezziN += `${nomi[p[0]]} ${p.substring(1)}. `; });
  return `Autore Rosario Turco. FEN trovata. Lato ${data.V==='B'?'Bianco':'Nero'}. Re Bianco in ${data.B.split(';')[0].substring(1)}. Re Nero in ${data.N.split(';')[0].substring(1)}. ${castlingText} Mossa numero ${data.moveNum}. Muove ${data.M==='B'?'Bianco':'Nero'}. ${extraInfo} ${pezziB} ${pezziN} ROTN copiata negli appunti.`;
}

function runConversion() {
  findFENs();
  if(!fenList.length){ speak("Autore Rosario Turco. Nessuna FEN trovata nella pagina"); return; }
  let lastData = parseFEN(fenList[0]);
  const rotnFile = fenToROTN_COMPLETA(lastData);
  navigator.clipboard.writeText(rotnFile);
  speak(getFullSpeechIT(lastData));
}

chrome.runtime.onMessage.addListener((req) => {
  if(req.action==="convert") { runConversion(); }
});

document.addEventListener('keydown', (e) => {
  if(e.key === 'Escape'){ speechSynthesis.cancel(); }
}, true);
