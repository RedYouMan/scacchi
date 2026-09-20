from collections import defaultdict
import re

# File di input e output
LOG_INPUT = "server.log"
LOG_OUTPUT = "serverordinato.txt"

# Espressione regolare per cercare la stanza (stringa alfanumerica dopo "in " o "inviato: ")
# Cattura codici come 'r41nkld'
ROOM_PATTERN = re.compile(r"\b(?:in|inviato:)\s+([a-zA-Z0-9]+)\b")

# Dizionario per raggruppare le righe: { 'nome_room': [lista_di_righe] }
rooms_data = defaultdict(list)
general_logs = []

# 1. Lettura e raggruppamento
with open(LOG_INPUT, "r", encoding="utf-8") as infile:
    for line in infile:
        line_clean = line.strip()
        if not line_clean:
            continue

        # Cerca se la riga contiene il nome di una stanza
        match = ROOM_PATTERN.search(line_clean)

        if match:
            room_name = match.group(1)
            rooms_data[room_name].append(line_clean)
        else:
            # Righe generiche (es. l'avvio del server)
            general_logs.append(line_clean)

# 2. Scrittura nel nuovo file ordinato
with open(LOG_OUTPUT, "w", encoding="utf-8") as outfile:
    # Scrive prima i log generali (opzionale, utile per mantenere i log di avvio)
    if general_logs:
        outfile.write("=== LOG GENERALI DI SISTEMA ===\n")
        for log in general_logs:
            outfile.write(f"{log}\n")
        outfile.write("\n")

    # Scrive i log raggruppati per ogni stanza (in ordine alfabetico di stanza)
    for room, lines in sorted(rooms_data.items()):
        outfile.write(f"=== ROOM: {room} ===\n")
        for line in lines:
            outfile.write(f"{line}\n")
        outfile.write("\n")  # Riga vuota di separazione tra stanze

print(f"Elaborazione completata! File salvato in: {LOG_OUTPUT}")
