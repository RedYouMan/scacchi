
#clientComNet.py

# ==============================================================================
# DICHIARAZIONE ESPLICITA DEI MODULI PER PYINSTALLER (LINK STATICI E DINAMICI)
# ==============================================================================
import asyncio
import json
import sys
import threading
import websockets
import websockets.client
import websockets.exceptions
import websockets.legacy.client
import winsound  # Suoni nativi Windows senza dipendenze esterne
import keyboard  # Gestione scorciatoie da tastiera globali (senza mouse)
import os
import time
import subprocess
import msvcrt


def sintesi_vocale(testo):
    """Legge il testo tramite il motore vocale integrato in Windows."""
    script = (
        "$testo = [Console]::In.ReadToEnd(); "
        "$voce = New-Object System.Speech.Synthesis.SpeechSynthesizer; "
        "$voce.Speak($testo)"
    )
    try:
        subprocess.run(
            ["powershell.exe", "-NoProfile", "-NonInteractive", "-Command", script],
            input=testo,
            text=True,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
            check=False,
        )
    except OSError as errore:
        print(f"Sintesi vocale non disponibile: {errore}")

def load_network_config():
    base_dir = os.path.dirname(sys.executable if getattr(sys, "frozen", False) else os.path.abspath(__file__))
    config_path = os.path.join(base_dir, "comnet.cnf")
    settings = {}
    with open(config_path, "r", encoding="utf-8-sig") as config_file:
        for line in config_file:
            line = line.strip()
            if line and not line.startswith("#") and "=" in line:
                key, value = line.split("=", 1)
                settings[key.strip().lower()] = value.strip()
    return settings["ipaddress"], int(settings["port"])

# Mappatura dei segnali acustici richiesti
def riproduci_suono(id_suono):
    if id_suono == 1:
        # Suono 1: Promozione pedone (Tono medio-alto prolungato)
        winsound.Beep(800, 400)
    elif id_suono == 2:
        # Suono 2: Proposta di patta (Due toni alternati ammonitori)
        winsound.Beep(440, 250)
        winsound.Beep(440, 250)
    elif id_suono == 4:
        # Suono 4: Allegro - Patta Accettata (Scala ascendente felice)
        winsound.Beep(523, 150) # Do
        winsound.Beep(659, 150) # Mi
        winsound.Beep(784, 300) # Sol
    elif id_suono == 5:
        # Suono 5: Poco allegro - Patta Rifiutata (Tono grave discendente)
        winsound.Beep(330, 200)
        winsound.Beep(220, 400)
    elif id_suono == 6:
        # Sei segnali lunghi e profondi: tempo esaurito.
        for _ in range(6):
            winsound.Beep(150, 1000)

class ChessClient:
    def __init__(self):
        self.ws = None
        self.loop = None
        self.in_draw_decision = False
        self.color = None
        self.remaining = {"white": 0.0, "black": 0.0}
        self.active_color = None
        self.clock_updated_at = time.monotonic()
        self.timeout_announced = False
        self.clock_started = False

    async def connect_server(self):
        host, port = load_network_config()
        uri = f"ws://{host}:{port}"
        try:
            async with websockets.connect(uri) as websocket:
                self.ws = websocket
                await websocket.send(json.dumps({"type": "clock_join", "color": self.color, "minutes": self.minutes}))
                print("Connesso al server di scacchi con successo.")
                print("Comandi globali attivi (funzionano anche fuori dalla finestra):")
                print(" - Alt+P: Richiedi Patta all'avversario")
                print(" - Alt+U: Annuncia la promozione (D=Donna, T=Torre, A=Alfiere, C=Cavallo)")
                print(" - Alt+1: Accetta la patta (quando proposta)")
                print(" - Alt+2: Rifiuta la patta (quando proposta)")
                
                async for message in websocket:
                    data = json.loads(message)
                    await self.handle_server_message(data)
        except Exception as e:
            print(f"Errore di connessione: {e}")

    async def handle_server_message(self, data):
        event_type = data.get("type")

        if event_type == "clock_error":
            print(f"Errore orologio: {data.get('message')}")
        elif event_type == "clock_state":
            self.remaining["white"] = float(data.get("white", 0))
            self.remaining["black"] = float(data.get("black", 0))
            # Il timer locale avanza solo dopo che il server segnala l'avvio
            # della partita (normalmente quando si è collegato anche il secondo client).
            connected = data.get("connected_players", data.get("players_connected"))
            both_connected = connected is None or int(connected) >= 2
            self.clock_started = bool(data.get("started", both_connected)) and both_connected
            self.active_color = data.get("active") if self.clock_started else None
            self.clock_updated_at = time.monotonic()
            if data.get("timeout") and not self.timeout_announced:
                self.timeout_announced = True
                print(f"{self._timeout_loser(data['timeout'])} ha perso per esaurimento del tempo.")
                riproduci_suono(6)
            if data.get("active"):
                print("Orologio aggiornato. " + self.format_clock())
        
        if event_type == "promotion_broadcast":
            piece_names = {"D": "Donna", "T": "Torre", "A": "Alfiere", "C": "Cavallo"}
            piece = piece_names.get(str(data.get("piece", "")).upper(), "pezzo non riconosciuto")
            sender_color = str(data.get("color") or "").strip().casefold()
            sender_color = {"white": "Bianco", "bianco": "Bianco", "black": "Nero", "nero": "Nero"}.get(sender_color, "avversario")
            position = data.get("position")
            where = f" in posizione {position}" if position else ""
            if sender_color == self.color:
                testo = f"Hai annunciato la promozione del pedone a {piece}{where}."
            else:
                testo = f"Messaggio ricevuto dal {sender_color}: promozione del pedone a {piece}{where}."
            print(f"\n{testo}")
            threading.Thread(target=sintesi_vocale, args=(testo,), daemon=True).start()
            riproduci_suono(1) # Suono 1: Promozione pedone

        elif event_type == "promotion_error":
            print(f"\nErrore promozione: {data.get('message')}")
            
        elif event_type == "draw_offered":
            print("\n[PROPOSTA] L'avversario ti offre la patta!")
            print("Premi ALT+1 per ACCETTARE o ALT+2 per RIFIUTARE.")
            self.in_draw_decision = True
            riproduci_suono(2) # Suono 2: Proposta di patta
            
        elif event_type == "draw_result":
            accepted = data.get("accepted")
            if accepted:
                print("\n[PARTITA TERMINATA] La patta è stata ACCETTATA.")
                riproduci_suono(4) # Suono 4: Allegro di patta accettata
            else:
                print("\n[PARTITA CONTINUA] La patta è stata RIFIUTATA.")
                riproduci_suono(5) # Suono 5: Poco allegro di patta rifiutata
            self.in_draw_decision = False

    def invia_richiesta_patta(self):
        """Invocato globalmente con Alt+P"""
        if self.ws and self.loop:
            asyncio.run_coroutine_threadsafe(
                self.ws.send(json.dumps({"type": "draw_offer"})), self.loop
            )
            print("\nHai inviato una proposta di patta.")

    def annuncia_promozione(self):
        """Alt+U: invia all'avversario il pezzo scelto per la promozione."""
        piece_names = {"D": "Donna", "T": "Torre", "A": "Alfiere", "C": "Cavallo"}
        if not self.ws or not self.loop:
            print("\nNon sei connesso al server.")
            return
        print("\nPromozione: premi D=Donna, T=Torre, A=Alfiere, C=Cavallo: ", end="", flush=True)
        while True:
            choice = msvcrt.getwch().upper()
            if choice in piece_names:
                piece = choice
                print(piece)
                break
            print("\nTasto non valido. Premi D, T, A oppure C: ", end="", flush=True)
        asyncio.run_coroutine_threadsafe(
            self.ws.send(json.dumps({"type": "promotion", "piece": piece})), self.loop
        )
        print(f"\nAnnuncio inviato: promozione a {piece_names[piece]}.")

    def switch_clock(self):
        """Alt+T: ferma il proprio orologio e avvia quello avversario."""
        if self.ws and self.loop:
            asyncio.run_coroutine_threadsafe(
                self.ws.send(json.dumps({"type": "clock_switch"})), self.loop
            )

    def request_clock(self):
        """Alt+O: chiede al server i tempi correnti di entrambi i giocatori."""
        if self.ws and self.loop:
            asyncio.run_coroutine_threadsafe(
                self.ws.send(json.dumps({"type": "clock_request"})), self.loop
            )

    def format_clock(self):
        values = dict(self.remaining)
        if self.active_color:
            key = "white" if self.active_color == "Bianco" else "black"
            values[key] = max(0.0, values[key] - (time.monotonic() - self.clock_updated_at))
        def fmt(seconds):
            total = max(0, int(seconds))
            return f"{total // 60:03d}:{total % 60:02d}"
        return f"Bianco {fmt(values['white'])} — Nero {fmt(values['black'])} (attivo: {self.active_color or 'nessuno'})"

    @staticmethod
    def _timeout_loser(timeout):
        if isinstance(timeout, dict):
            timeout = timeout.get("color") or timeout.get("loser")
        color = str(timeout or "").strip().lower()
        if color in ("white", "bianco"):
            return "Il Bianco"
        if color in ("black", "nero"):
            return "Il Nero"
        return f"Il giocatore {timeout or 'segnalato dal server'}"

    def announce_clock(self):
        print("\n" + self.format_clock())

    def gestisci_risposta_patta(self, accetta):
        """Invocato globalmente con Alt+1 (True) o Alt+2 (False)"""
        if not self.in_draw_decision:
            return
        if self.ws and self.loop:
            asyncio.run_coroutine_threadsafe(
                self.ws.send(json.dumps({"type": "draw_response", "accepted": accetta})), self.loop
            )
            self.in_draw_decision = False
            print(f"\nHai risposto alla patta: {'Accettata' if accetta else 'Rifiutata'}")

    def setup_shortcuts(self):
        # Assegnazione dei tasti rapidi senza bisogno del mouse o del focus obbligatorio
        keyboard.add_hotkey('alt+p', self.invia_richiesta_patta)
        keyboard.add_hotkey('alt+u', self.annuncia_promozione)
        keyboard.add_hotkey('alt+1', lambda: self.gestisci_risposta_patta(True))
        keyboard.add_hotkey('alt+2', lambda: self.gestisci_risposta_patta(False))
        keyboard.add_hotkey('alt+t', self.switch_clock)
        keyboard.add_hotkey('alt+o', self.request_clock)

    def start(self):
        choice = ""
        while choice not in ("b", "bianco", "n", "nero"):
            choice = input("Colore (B=Bianco, N=Nero): ").strip().lower()
        self.color = "Bianco" if choice in ("b", "bianco") else "Nero"
        print(f"Colore scelto: {self.color}")
        while True:
            try:
                self.minutes = int(input("Minuti iniziali (es. 120): ").strip())
                if self.minutes > 0:
                    break
            except ValueError:
                pass
            print("Inserire un numero intero positivo.")
        print(f"Minuti iniziali scelti: {self.minutes}")
        self.setup_shortcuts()
        self.loop = asyncio.new_event_loop()
        asyncio.set_event_loop(self.loop)
        self.loop.run_until_complete(self.connect_server())

if __name__ == "__main__":
    client = ChessClient()
    client.start()
