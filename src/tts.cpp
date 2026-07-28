// tts
#include <windows.h>
#include <iostream>
#include <sphelper.h>
#include <string>
#include <sapi.h>    // Header SAPI
#include <atlbase.h> // Per CComPtr e CComHeapPtr

// Definizione della funzione per pronunciare la stringa
HRESULT SpeakString(const WCHAR *pszText)
{
    HRESULT hr = S_OK;
    CComPtr<ISpVoice> pVoice; // Puntatore all'interfaccia del sintetizzatore

    // Inizializza l'ambiente COM
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr))
    {
        std::cerr << "Errore nell'inizializzazione di COM: " << hr << std::endl;
        return hr;
    }

    // Crea l'istanza del sintetizzatore
    hr = pVoice.CoCreateInstance(CLSID_SpVoice);
    if (FAILED(hr))
    {
        std::cerr << "Errore nella creazione di SpVoice: " << hr << std::endl;
        CoUninitialize();
        return hr;
    }

    // --- MODIFICA: Selezione Voce Italiana o Default ---
    CComPtr<ISpObjectToken> pTokenItaliano;
    // Cerca una voce con lingua italiana (LANGID 0x0410)
    hr = SpFindBestToken(SPCAT_VOICES, L"Language=410", NULL, &pTokenItaliano);

    if (SUCCEEDED(hr))
    {
        // Imposta la voce italiana trovata
        pVoice->SetVoice(pTokenItaliano);
    }
    else
    {
        // Se non trova l'italiano, SpFindBestToken o SetVoice falliscono,
        // ma pVoice userà automaticamente la voce predefinita di sistema.
        // std::cout << "Voce italiana non trovata, uso quella di sistema." << std::endl;
    }
    // ----------------------------------------------------

    // Pronuncia la stringa
    hr = pVoice->Speak(pszText, 0, NULL);
    if (FAILED(hr))
    {
        // std::cerr << "Errore durante la pronuncia: " << hr << std::endl;
    }

    // Disattiva l'ambiente COM
    CoUninitialize();
    return hr;
}

int callTextToSpeech(std::string stringaInput)
{
    std::wstring textToSpeak(stringaInput.begin(), stringaInput.end());

    // Chiama la funzione per pronunciare la stringa
    HRESULT hr = SpeakString(textToSpeak.c_str());

    if (FAILED(hr))
    {
        return 1; // Errore nella sintesi
    }

    return 0; // Successo
}
