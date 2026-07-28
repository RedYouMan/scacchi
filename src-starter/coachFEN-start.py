import os
import subprocess
import sys


def print_usage():
    print('Uso: coachFEN-start "FEN"')
    print('coachFEN accetta un parametro di input che deve essere la FEN tra doppi apici.')


def main():
    if len(sys.argv) != 2:
        print('Errore: parametro FEN mancante.')
        print_usage()
        sys.exit(1)

    fen = sys.argv[1]

    # 1. Preleva la directory di partenza e la memorizza
    start_dir = os.getcwd()
    # print(f"Directory di partenza: {start_dir}")

    bin_dir = os.path.join(start_dir, "bin")
    exe_name = "coachFEN.exe"
    exe_path = os.path.join(bin_dir, exe_name)

    # 2. Controlla se la directory bin esiste
    if os.path.exists(bin_dir) and os.path.isdir(bin_dir):
        # print(f"Spostamento nella directory: {bin_dir}")
        os.chdir(bin_dir)

        # 3. Lancia aspettando l'eseguibile coachFEN.exe con la FEN
        if os.path.exists(exe_name):
            # print(f"Lancio di {exe_name} con FEN...")
            subprocess.run([exe_path, fen])
            # print("Esecuzione di coachFEN.exe terminata.")
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
