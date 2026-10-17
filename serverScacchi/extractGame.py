#!/usr/bin/env python3
"""Estrae una partita dagli eventi di una room registrati nel log del server."""

import argparse
from datetime import date
import re
import sys
from pathlib import Path


MOVE_RE = re.compile(r"^([PRNTDACBNQK])([a-h][1-8])([PRNTDACBNQK])([a-h][1-8])(?:=([PRNTDACBNQK]))?$", re.I)
SENT_RE = re.compile(
	r"Mossa inviata a (?P<player>.+?) in (?P<room>[^:]+):\s*(?P<move>\S+)", re.I
)
RESULT_RE = re.compile(r"\b(Mat[to]+|Patta|Abbandona)\b", re.I)

PIECE_NAMES = {
	"P": "",  # pedone
	"A": "A",  # alfiere
	"C": "C",  # cavallo
	"T": "T",  # torre
	"D": "D",  # donna
	"R": "R",  # re
	# Accetta anche i codici internazionali eventualmente usati dal server.
	"B": "A",
	"N": "C",
	"Q": "D",
	"K": "R",
}


def format_move(raw_move):
	"""Converte, ad esempio, Af1Ac4 in Ac4 e Pe2Pe4 in e4."""
	if raw_move.lower() == "none":
		return None
	match = MOVE_RE.fullmatch(raw_move)
	if not match:
		return None
	start_piece, _start_square, end_piece, end_square, promotion = match.groups()
	piece = PIECE_NAMES.get(end_piece.upper(), PIECE_NAMES.get(start_piece.upper(), ""))
	if promotion:
		piece += "=" + PIECE_NAMES.get(promotion.upper(), promotion.upper())
	return piece + end_square


def initial_board():
	"""Crea la posizione iniziale per riconoscere le catture dalla destinazione."""
	board = {}
	back_rank = "TCADRACT"
	for file_index, piece in enumerate(back_rank):
		file_name = chr(ord("a") + file_index)
		board[file_name + "1"] = (piece, "white")
		board[file_name + "2"] = ("P", "white")
		board[file_name + "7"] = ("P", "black")
		board[file_name + "8"] = (piece, "black")
	return board


def is_capture(raw_move, board, turn, previous_move=None):
	"""Rileva le catture normali e en passant, aggiornando la scacchiera."""
	match = MOVE_RE.fullmatch(raw_move)
	if not match:
		return False
	start_piece, start_square, end_piece, end_square, promotion = match.groups()
	moving_piece = PIECE_NAMES.get(start_piece.upper(), start_piece.upper())
	moving = board.pop(start_square, (moving_piece, turn))
	captured = end_square in board and board[end_square][1] != moving[1]
	if (
		not captured
		and moving[0] == "P"
		and start_square[0] != end_square[0]
		and previous_move is not None
	):
		last_start, last_end, last_piece, last_color = previous_move
		ep_square = last_end[0] + str(int(start_square[1]) + (1 if moving[1] == "white" else -1))
		if (
			last_piece == "P"
			and last_color != moving[1]
			and last_end == start_square
			and last_start[0] == last_end[0]
			and abs(int(last_end[1]) - int(last_start[1])) == 2
			and end_square == ep_square
			and last_end in board
			and board[last_end] == ("P", last_color)
		):
			board.pop(last_end)
			captured = True
	placed_piece = (
		PIECE_NAMES.get(promotion.upper(), promotion.upper())
		if promotion
		else PIECE_NAMES.get(end_piece.upper(), end_piece.upper())
	)
	board[end_square] = (placed_piece, moving[1])
	return captured


def extract_game(room, log_name):
	moves = []
	players = []
	result = ""
	room = room.casefold()
	board = initial_board()
	previous_move = None

	with Path(log_name).open("r", encoding="utf-8", errors="replace") as log_file:
		for line in log_file:
			# Gli eventi di risultato includono room=... e player=... nel log.
			if RESULT_RE.search(line) and re.search(
				rf"\broom={re.escape(room)}\b", line, re.I
			):
				event = RESULT_RE.search(line).group(1).casefold()
				player_match = re.search(r"\bplayer=([^\s]+)", line, re.I)
				player = player_match.group(1).casefold() if player_match else ""
				if event.startswith("matt"):
					result = "#"
				elif event == "patta":
					result = "1/2-1/2"
				elif event == "abbandona":
					result = "1-0" if player in {"nero", "black"} else "0-1"
				continue

			sent = SENT_RE.search(line)
			if not sent or sent.group("room").casefold() != room:
				continue
			player = sent.group("player").strip()
			if player and player.casefold() not in {name.casefold() for name in players}:
				players.append(player)
			raw_move = sent.group("move")
			move = format_move(raw_move)
			if move is not None:
				match = MOVE_RE.fullmatch(raw_move)
				turn = "white" if len(moves) % 2 == 0 else "black"
				if is_capture(raw_move, board, turn, previous_move):
					move = move[:-2] + "X" + move[-2:]
				moves.append(move)
				previous_move = (
					match.group(2),
					match.group(4),
					PIECE_NAMES.get(match.group(1).upper(), match.group(1).upper()),
					turn,
				)

	# La notazione italiana assegna il numero alla mossa del Bianco.
	notation = []
	for index, move in enumerate(moves):
		if index % 2 == 0:
			notation.append(f"{index // 2 + 1}.{move}")
		else:
			notation[-1] += " " + move

	game = " ".join(notation)
	if result == "#":
		if game:
			game += "#"
	elif result:
		game = f"{game} {result}".strip()

	# Salva la partita accanto a questo programma, usando i nomi dei giocatori.
	filename_players = (players + ["player1", "player2"])[:2]
	filename_players = [
		re.sub(r'[<>:"/\\|?*]', "_", name).strip().rstrip(".") or f"player{index + 1}"
		for index, name in enumerate(filename_players)
	]
	output_path = Path(__file__).resolve().parent / (
		f"{filename_players[0]}-{filename_players[1]}-{date.today().isoformat()}.txt"
	)
	output_path.write_text(game + "\n", encoding="utf-8")
	print(f"file creato {output_path.name}")
	return game


def main():
	parser = argparse.ArgumentParser(
		description="Estrae la notazione algebrica italiana di una room dal log."
		, prog=Path(sys.argv[0]).stem
	)
	parser.add_argument("room", help="nome/codice della room")
	parser.add_argument("log", help="percorso del file di log")
	args = parser.parse_args()
	try:
		print(extract_game(args.room, args.log))
	except OSError as error:
		print(f"Errore apertura log: {error}", file=sys.stderr)
		return 1
	return 0


if __name__ == "__main__":
	raise SystemExit(main())
