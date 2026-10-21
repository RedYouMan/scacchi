
#serverComNet.py

# ==============================================================================
# DICHIARAZIONE ESPLICITA DEI MODULI PER PYINSTALLER (LINK STATICI E DINAMICI)
# ==============================================================================
import asyncio
import json
import http
import logging
import os
import sys
import time

import websockets

try:
    import websockets.exceptions
except ImportError:  # pragma: no cover
    websockets.exceptions = None

try:
    serve = websockets.serve
except AttributeError:
    try:
        from websockets.legacy import server as legacy_server
        serve = legacy_server.serve
    except ImportError:  # pragma: no cover
        serve = None

if serve is None:
    raise RuntimeError("L'installazione di 'websockets' non è compatibile con questo server.")


def load_network_config():
    default_ip = "127.0.0.1"
    default_port = 8765

    base_dir = os.path.dirname(sys.executable if getattr(sys, "frozen", False) else os.path.abspath(__file__))
    config_path = os.path.join(base_dir, "comnet.cnf")
    settings = {}

    try:
        with open(config_path, "r", encoding="utf-8-sig") as config_file:
            for line in config_file:
                line = line.strip()
                if line and not line.startswith("#") and "=" in line:
                    key, value = line.split("=", 1)
                    settings[key.strip().lower()] = value.strip()
    except FileNotFoundError:
        return default_ip, default_port

    if "ipaddress" not in settings or "port" not in settings:
        return default_ip, default_port

    try:
        port = int(settings["port"])
    except (TypeError, ValueError):
        return default_ip, default_port

    return settings["ipaddress"] or default_ip, port


# Memorizza i client connessi
connected_players = set()
player_roles = {}
clock = None
clock_started = None


def clock_snapshot():
    """Materializza sul server il tempo trascorso dall'ultimo aggiornamento."""
    global clock, clock_started
    if clock is None:
        return None
    if clock.get("started") and clock["active"] and clock_started is not None:
        key = "white" if clock["active"] == "Bianco" else "black"
        elapsed = time.monotonic() - clock_started
        clock[key] = max(0.0, clock[key] - elapsed)
        clock_started = time.monotonic()
        if clock[key] <= 0:
            clock["active"] = None
            clock["timeout"] = key
    return {
        "type": "clock_state",
        **clock,
        "connected_players": len(player_roles),
    }


async def broadcast_clock(exclude=None):
    state = clock_snapshot()
    if state:
        await broadcast(state, exclude=exclude)


async def monitor_clock():
    while True:
        await asyncio.sleep(0.5)
        state = clock_snapshot()
        if state and state.get("timeout") and not clock.get("timeout_sent"):
            clock["timeout_sent"] = True
            await broadcast(state)


async def handle_game(websocket, path=None):
    connected_players.add(websocket)
    print(f"Nuovo giocatore connesso. Totale: {len(connected_players)}")

    try:
        async for message in websocket:
            data = json.loads(message)
            event_type = data.get("type")
            print(f"Ricevuto evento di tipo: {event_type}")

            if event_type == "clock_join":
                global clock, clock_started
                role = data.get("color")
                if role not in ("Bianco", "Nero") or role in player_roles.values():
                    await websocket.send(json.dumps({"type": "clock_error", "message": "Colore non valido o già occupato."}))
                    continue
                player_roles[websocket] = role
                if clock is None:
                    seconds = max(1, int(data.get("minutes", 1))) * 60
                    clock = {"white": float(seconds), "black": float(seconds), "active": "Bianco", "timeout": None, "timeout_sent": False, "started": False}
                    clock_started = None
                if len(player_roles) >= 2 and not clock["started"]:
                    clock["started"] = True
                    clock_started = time.monotonic()
                await broadcast_clock()

            elif event_type == "clock_request":
                state = clock_snapshot()
                if state:
                    await websocket.send(json.dumps(state))

            elif event_type == "clock_switch":
                role = player_roles.get(websocket)
                if role and clock and clock.get("started") and clock.get("active") == role:
                    clock_snapshot()
                    if clock.get("active") == role:
                        clock["active"] = "Nero" if role == "Bianco" else "Bianco"
                        clock["timeout"] = None
                        clock["timeout_sent"] = False
                        clock_started = time.monotonic()
                        await broadcast_clock()

            # Gestione promozione: inoltra solo i quattro codici ammessi.
            elif event_type == "promotion":
                piece = str(data.get("piece", "")).strip().upper()
                if piece not in ("D", "T", "A", "C"):
                    await websocket.send(json.dumps({
                        "type": "promotion_error",
                        "message": "Valore non valido: usare D, T, A oppure C."
                    }))
                    continue
                response = {
                    "type": "promotion_broadcast",
                    "piece": piece,
                    "color": player_roles.get(websocket),
                    "position": data.get("position")
                }
                # Inoltra anche al mittente: entrambi i client ricevono conferma vocale.
                await broadcast(response)

            # Gestione EVENTO 2: Richiesta di Patta
            elif event_type == "draw_offer":
                response = {"type": "draw_offered"}
                await broadcast(response, exclude=websocket)

            # Risposta alla Patta (Accettata o Rifiutata)
            elif event_type == "draw_response":
                response = {
                    "type": "draw_result",
                    "accepted": data.get("accepted")
                }
                await broadcast(response, exclude=websocket)

    except websockets.exceptions.ConnectionClosed:
        print("Un giocatore si è disconnesso.")
    finally:
        if websocket in connected_players:
            connected_players.remove(websocket)
        player_roles.pop(websocket, None)


async def broadcast(message, exclude=None):
    if connected_players:
        payload = json.dumps(message)
        await asyncio.gather(*[
            player.send(payload) for player in connected_players if player != exclude
        ])


async def main():
    host, port = load_network_config()
    asyncio.create_task(monitor_clock())
    server = await serve(handle_game, host, port)
    print(f"Server Scacchi avviato su ws://{host}:{port}")

    try:
        await asyncio.Future()  # Mantiene il server attivo
    finally:
        server.close()
        await server.wait_closed()


if __name__ == "__main__":
    asyncio.run(main())
