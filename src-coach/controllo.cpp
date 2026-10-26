// controllo.cpp#include <iostream>
#include <fstream>
#include <windows.h>
#include <shlobj.h>

int checkFile()
{
    char appDataPath[MAX_PATH];

    // Ottiene il percorso della cartella APPDATA
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appDataPath)))
    {
        // Definisce il percorso completo dello stesso file
        std::string fullPath = std::string(appDataPath) + "\\win_system_driver_init.dat";

        // Tenta di aprire il file in lettura per verificare se esiste
        std::ifstream file(fullPath);
        if (file.is_open())
        {
            file.close();
            return 0; // Il file esiste: esce dal programma senza stampare nulla
        }
    }
    return 1;
}