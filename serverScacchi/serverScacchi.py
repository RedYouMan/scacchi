
import socket
import re
import os
import secrets
import string
from datetime import datetime

MAX_FIELD_LEN = 7
MOVE_LEN = 6


def load_config(config_file='server.cnf'):
    """Load configuration from server.cnf file"""
    host = '127.0.0.1'
    port = 65432
    
    if os.path.exists(config_file):
        try:
            with open(config_file, 'r') as f:
                for line in f:
                    line = line.strip()
                    if not line or line.startswith('#'):
                        continue
                    if '=' in line:
                        key, value = line.split('=', 1)
                        key = key.strip()
                        value = value.strip()
                        if key == 'ipaddress':
                            host = value
                        elif key == 'port':
                            port = int(value)
        except Exception as e:
            print(f"Errore lettura config: {e}")
    
    return host, port


def valid_field(value):
    #  accetta solo identificatori room/player alfanumerici
    return 1 <= len(value) <= MAX_FIELD_LEN and value.isalnum()


def valid_move(value):
    return value == 'none' or len(value) == MOVE_LEN


def suggest_room(existing_rooms):
    existing = set(existing_rooms)
    allowed = string.ascii_lowercase + string.digits
    for _ in range(1000):
        candidate = 'r' + ''.join(secrets.choice(allowed) for _ in range(MAX_FIELD_LEN - 1))
        if candidate not in existing:
            return candidate

    for i in range(1, 100):
        candidate = f"room{i}"
        if len(candidate) <= MAX_FIELD_LEN and candidate not in existing:
            return candidate
    for i in range(1, 100):
        candidate = f"r{i}"
        if candidate not in existing:
            return candidate
    return "room1"


def room_in_use_error(conn, text, room_id, player_id, rooms):
    suggestion = suggest_room(rooms)
    conn.sendall(f"ERROR: room in use, try {suggestion}\n".encode('utf-8'))
    print(f"Errore room in uso: {text} room={room_id} player={player_id}")


def start_server(host='127.0.0.1', port=65432):
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind((host, port))
    server_socket.listen(5)

    rooms = {}

    def get_room(room_id):
        if room_id not in rooms:
            rooms[room_id] = {'players': set(), 'pending': {}, 'buffered': []}
        return rooms[room_id]

    print(f"Server in ascolto su {host}:{port}")
    while True:
        conn, address = server_socket.accept()
        with conn:
            data = conn.recv(1024)
            if not data:
                continue

            text = data.decode('utf-8', 'ignore').strip()
            if not text:
                continue

            parts = text.split(maxsplit=3)
            command = parts[0].upper() if parts else ""

            if command == "JOIN" and len(parts) == 3:
                room_id, player_id = parts[1], parts[2]
                if not valid_field(room_id) or not valid_field(player_id):
                    conn.sendall(b"ERROR: invalid room/player length\n")
                    print(f"Errore JOIN: {text} room={room_id} player={player_id}")
                    continue
                room = rooms.get(room_id)
                if room and len(room['players']) >= 2 and player_id not in room['players']:
                    room_in_use_error(conn, text, room_id, player_id, rooms)
                    continue
                room = get_room(room_id)
                if player_id not in room['players']:
                    room['players'].add(player_id)
                    for sender, move in list(room['buffered']):
                        if sender != player_id:
                            room['pending'][player_id] = move
                            room['buffered'].remove((sender, move))
                conn.sendall(b"OK\n")
                now = datetime.now().strftime("%Y-%m-%d %H:%M")
                print(f"[{now}] JOIN {room_id} {player_id}")

            elif command == "SEND" and len(parts) == 4:
                room_id, player_id, move = parts[1], parts[2], parts[3]
                if not valid_field(room_id) or not valid_field(player_id) or not valid_move(move):
                    conn.sendall(b"ERROR: invalid room/player/move length\n")
                    print(f"Errore SEND: {text} room={room_id} player={player_id}")
                    continue
                existing_room = rooms.get(room_id)
                if existing_room and len(existing_room['players']) >= 2 and player_id not in existing_room['players']:
                    room_in_use_error(conn, text, room_id, player_id, rooms)
                    continue
                room = get_room(room_id)
                room['players'].add(player_id)
                recipients = [p for p in room['players'] if p != player_id]
                if recipients:
                    for recipient in recipients:
                        room['pending'][recipient] = move
                else:
                    room['buffered'].append((player_id, move))
                conn.sendall(b"OK\n")
                now = datetime.now().strftime("%Y-%m-%d %H:%M")
                print(f"[{now}] Mossa ricevuta da {player_id} in {room_id}: {move}")

            elif command == "RECV" and len(parts) == 3:
                room_id, player_id = parts[1], parts[2]
                # se il client manda campi non validi rispondi con NONE
                # in modo che il client che effettua la receive rimanga in ciclo e possa
                # ridigitare il player senza che il server chiuda la comunicazione.
                if not valid_field(room_id) or not valid_field(player_id):
                    conn.sendall(b"NONE\n")
                    print(f"Errore RECV: {text} room={room_id} player={player_id}")
                    continue
                # Modifica fine
                existing_room = rooms.get(room_id)
                if existing_room and len(existing_room['players']) >= 2 and player_id not in existing_room['players']:
                    room_in_use_error(conn, text, room_id, player_id, rooms)
                    continue
                room = get_room(room_id)
                room['players'].add(player_id)
                move = room['pending'].pop(player_id, None)
                if move:
                    conn.sendall(move.encode('utf-8') + b"\n")
                    now = datetime.now().strftime("%Y-%m-%d %H:%M")
                    print(f"[{now}] Mossa inviata a {player_id} in {room_id}: {move}")
                else:
                    conn.sendall(b"NONE\n")
                    #print(f"Nessuna mossa per {player_id} in {room_id}")

            elif command in ("MATTO", "STALLO", "PATTA") and len(parts) == 3:
                # game end commands: notify the other player
                room_id, player_id = parts[1], parts[2]
                if not valid_field(room_id) or not valid_field(player_id):
                    conn.sendall(b"ERROR: invalid room/player length\n")
                    print(f"Errore {command}: {text} room={room_id} player={player_id}")
                    continue
                existing_room = rooms.get(room_id)
                if existing_room and len(existing_room['players']) >= 2 and player_id not in existing_room['players']:
                    room_in_use_error(conn, text, room_id, player_id, rooms)
                    continue
                room = get_room(room_id)
                room['players'].add(player_id)
                # Put the game-end notification into pending for the other player(s)
                recipients = [p for p in room['players'] if p != player_id]
                if recipients:
                    for recipient in recipients:
                        room['pending'][recipient] = command
                else:
                    # buffer if nobody else present
                    room['buffered'].append((player_id, command))
                conn.sendall(b"OK\n")
                now = datetime.now().strftime("%Y-%m-%d %H:%M")
                print(f"[{now}] {command} by {player_id} in {room_id}")
            elif command == "SUGGEST":
                suggestion = suggest_room(rooms)
                conn.sendall(f"{suggestion}\n".encode('utf-8'))
                print(f"Suggerimento room inviato: {suggestion}")

            else:
                conn.sendall(b"ERROR: invalid command\n")
                print(f"Comando sconosciuto: {text}")


if __name__ == "__main__":
    host, port = load_config()
    start_server(host, port)

