import os
import subprocess
import sys


def main():
    # 1. Preleva la directory di partenza e la memorizza
    start_dir = os.getcwd()
    # print(f"Directory di partenza: {start_dir}")

    bin_dir = os.path.join(start_dir, "sospensioni")
    exe_name = "fenpos.exe"
    exe_path = os.path.join(bin_dir, exe_name)

    # Verifica parametri: devono essere 3 (escluso il nome dello script)
    if len(sys.argv) - 1 != 3:
        print("Usage: incolla-fen <testo tra doppi apici> <numero posizione> <fen tra doppi apici>")
        print("Dopo su CtrlR inserire nome file pos_<numero posizione>. Fai attenzione a mettere il carattere sottolineato tra pos e numero posizione nel nome del file")
        sys.exit(2)

    # Prende i tre parametri
    args = sys.argv[1:4]

    # Rimuove eventuali apici esterni dalla FEN prima di passarla a validaFEN
    fen = args[2]
    if (fen.startswith('"') and fen.endswith('"')) or (fen.startswith("'") and fen.endswith("'")):
        fen = fen[1:-1]
    args[2] = fen

    # Prima di spostarsi in sospensioni, esegue validaFEN.exe accanto a questo script
    valida_exe = os.path.join(start_dir, "validaFEN.exe")
    if not os.path.exists(valida_exe):
        print(f"Error: validaFEN.exe non trovato in {start_dir}")
        sys.exit(4)

    # lancia validaFEN con il terzo argomento (la FEN)
    try:
        res = subprocess.run([valida_exe, args[2]])
    except Exception as e:
        print(f"Errore avviando validaFEN.exe: {e}")
        sys.exit(5)

    # Se validaFEN torna 1 si esce con messaggio
    if res.returncode == 1:
        print("La stringa non risulta una FEN.")
        sys.exit(6)
    # se torna 0 si prosegue; altri codici li lasciamo proseguire o segnalare
    if res.returncode != 0:
        print(f"validaFEN.exe ha ritornato codice {res.returncode}")
        sys.exit(7)

    # 2. Controlla se la directory bin esiste
    if os.path.exists(bin_dir) and os.path.isdir(bin_dir):
        # print(f"Spostamento nella directory: {bin_dir}")
        os.chdir(bin_dir)

        # 3. Lancia l'eseguibile senza attendere la sua terminazione
        if os.path.exists(exe_name):
                # Verifica parametri: devono essere 3 (escluso il nome dello script)
                if len(sys.argv) - 1 != 3:
                    print("Usage: incolla-fen <testo tra doppi apici> <numero posizione> <fen tra doppi apici>")
                    print("Dopo su CtrlR inserire nome file pos_<numero posizione>. Fai attenzione a mettere il carattere sottolineato tra pos e numero posizione nel nome del file")
                    
                    sys.exit(2)

                # Prende i tre parametri e li passa all'eseguibile
                args = sys.argv[1:4]

                # Avvia il processo senza attendere la sua terminazione
                try:
                    subprocess.Popen([exe_path] + args)
                except Exception as e:
                    print(f"Errore avviando {exe_name}: {e}")
                    sys.exit(3)

                # Messaggi di completamento immediato (il processo continua in background)
                print("Eseguito avvio di fenpos.exe (in background)")
                print("Fine operazioni.")
        else:
            print(f"Error: {exe_name} non trovato dentro {bin_dir}")

        # 4. Torna alla directory di partenza
        os.chdir(start_dir)
        #print("Arrivederci alla prossima partita")

    else:
        # 5. Se la directory bin non esiste, da il messaggio e esce
        print("Warning la directory bin non esiste")
        sys.exit()


if __name__ == "__main__":
    main()
