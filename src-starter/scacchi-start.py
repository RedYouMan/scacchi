import os
import subprocess
import sys


def main():
    # 1. Preleva la directory di partenza e la memorizza
    start_dir = os.getcwd()
    # print(f"Directory di partenza: {start_dir}")

    bin_dir = os.path.join(start_dir, "bin")
    exe_name = "scacchi.exe"
    exe_path = os.path.join(bin_dir, exe_name)

    # 2. Controlla se la directory bin esiste
    if os.path.exists(bin_dir) and os.path.isdir(bin_dir):
        # print(f"Spostamento nella directory: {bin_dir}")
        os.chdir(bin_dir)

        # 3. Lancia aspettando l'eseguibile scacchi.exe
        if os.path.exists(exe_name):
            # print(f"Lancio di {exe_name}...")
            # subprocess.run attende la fine del processo
            subprocess.run([exe_path])
            # print("Esecuzione di scacchi.exe terminata.")
        else:
            print(f"Error: {exe_name} non trovato dentro {bin_dir}")

        # 4. Torna alla directory di partenza
        os.chdir(start_dir)
        print("Arrivederci alla prossima partita")

    else:
        # 5. Se la directory bin non esiste, da il messaggio e esce
        print("Warning la directory bin non esiste")
        sys.exit()


if __name__ == "__main__":
    main()
