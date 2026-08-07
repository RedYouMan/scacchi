import os
import subprocess
import sys


def print_usage():
    print('Uso: coachGame-start "file da analizzare con estensione" "Colore da analizzare (es. b o n)"')


def main():
    if len(sys.argv) != 3:
        print('Errore: parametri non sufficienti .')
        print_usage()
        sys.exit(1)

    LogPartita = os.path.join("..", "registrazioni", sys.argv[1])
    Colore = sys.argv[2]
    LogDaPassare = sys.argv[1]

    # 1. Preleva la directory di partenza e la memorizza
    start_dir = os.getcwd()
    # print(f"Directory di partenza: {start_dir}")

    bin_dir = os.path.join(start_dir, "bin")
    exe_name = "coachGame.exe"
    exe_path = os.path.join(bin_dir, exe_name)

    # 2. Controlla se la directory bin esiste
    if os.path.exists(bin_dir) and os.path.isdir(bin_dir):
        # print(f"Spostamento nella directory: {bin_dir}")
        os.chdir(bin_dir)

        if not os.path.exists(LogPartita):
            print(f"Errore: file di log '{LogPartita}' non trovato.")
            sys.exit(1)

        if Colore not in ("b", "n"):
            print("Errore: colore non valido. Deve essere 'b' o 'n'.")
            sys.exit(1)

        # 3. Lancia aspettando l'eseguibile coachGame.exe con i parametri 
        if os.path.exists(exe_name):
            # print(f"Lancio di {exe_name} con parametri...")
            subprocess.run([exe_path, LogDaPassare, Colore])
            # print("Esecuzione di coachGame.exe terminata.")
        else:
            print(f"Errore: {exe_name} non trovato dentro {bin_dir}")

        # 4. Torna alla directory di partenza
        os.chdir(start_dir)

    else:
        # 5. Se la directory bin non esiste, da il messaggio e esce
        print("Warning: la directory bin non esiste")
        sys.exit(1)


if __name__ == "__main__":
    main()
