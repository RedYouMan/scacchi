## Script Python analisi_tempi.py
#tempi rapidi o tempi molto uniformi possono essere un indizio, non una prova
#dell'uso di un'intelligenza artificiale.

import argparse
import re
import statistics
import sys
from collections import defaultdict
from datetime import datetime
from pathlib import Path

FORMAT = "%Y-%m-%d %H:%M:%S"
SUGGERIMENTO = re.compile(
	r"^(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}) INFO "
	r"Suggerimento room inviato:\s*(\S+)"
)
INVIATA = re.compile(
	r"^(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}) INFO "
	r"\[.*?\] Mossa inviata a (.+?) in\s+([^:\s]+):"
)

def analizza(percorso_log, percorso_output, rapido, uniforme, variabilita, min_mosse):
	rooms = defaultdict(lambda: {"giocatori": set(), "mosse": [], "inizio": None})
	ora_lancio = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

	with open(percorso_log, encoding="utf-8", errors="replace") as file:
		righe = []
		for riga in file:
			riga = riga.rstrip("\r\n")
			# Ricompone le righe spezzate, come nel log d'esempio.
			if riga.lstrip().startswith(":") and righe:
				righe[-1] += " " + riga.strip()
			else:
				righe.append(riga)

	for riga in righe:
		match = SUGGERIMENTO.match(riga)
		if match:
			quando = datetime.strptime(match.group(1), FORMAT)
			room = rooms[match.group(2)]
			room["inizio"] = min(room["inizio"], quando) if room["inizio"] else quando
			continue

		match = INVIATA.search(riga)
		if match:
			quando = datetime.strptime(match.group(1), FORMAT)
			giocatore, nome_room = match.group(2).strip(), match.group(3)
			room = rooms[nome_room]
			room["giocatori"].add(giocatore)
			room["mosse"].append((quando, giocatore))

	risultati = []
	for nome_room, room in rooms.items():
		giocatori = sorted(room["giocatori"], key=str.casefold)
		if len(giocatori) != 2:
			continue

		tempi = defaultdict(list)
		precedente = room["inizio"]
		for quando, giocatore in sorted(room["mosse"]):
			if precedente is not None:
				secondi = (quando - precedente).total_seconds()
				if 0 <= secondi <= 24 * 60 * 60:
					tempi[giocatore].append(secondi)
			precedente = quando

		sospetti = []
		for giocatore in giocatori:
			campione = tempi[giocatore]
			if len(campione) < min_mosse:
				continue
			mediana = statistics.median(campione)
			media = statistics.mean(campione)
			cv = statistics.pstdev(campione) / media if media else 0
			if mediana <= rapido or (mediana <= uniforme and cv <= variabilita):
				sospetti.append(giocatore)

		if sospetti:
			risultati.append(f"room {nome_room} giocatori: {', '.join(giocatori)}")
			risultati.extend(f"room {nome_room} giocatore: {nome}" for nome in sospetti)

	with open(percorso_output, "w", encoding="utf-8") as file:
		file.write(f"Analisi avviata il: {ora_lancio}\n")
		if risultati:
			file.write("\n".join(risultati) + "\n")
		else:
			file.write("Non è stato trovato nulla di sospetto.\n")


def main():
	parser = argparse.ArgumentParser(
		prog=Path(sys.argv[0]).stem,
		description="Analizza i tempi delle mosse nei log di scacchi.",
	)
	parser.add_argument("log", help="percorso del log da analizzare")
	parser.add_argument("-o", "--output", default="analisiAI.txt")
	parser.add_argument("--rapido", type=float, default=3.0, help="soglia rapida in secondi")
	parser.add_argument("--uniforme", type=float, default=12.0, help="soglia mediana in secondi")
	parser.add_argument("--variabilita", type=float, default=0.20, help="coefficiente di variazione massimo")
	parser.add_argument("--min-mosse", type=int, default=4, help="minimo tempi osservati per giocatore")
	args = parser.parse_args()
	analizza(args.log, args.output, args.rapido, args.uniforme,
			 args.variabilita, args.min_mosse)


if __name__ == "__main__":
	main()
