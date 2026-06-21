// inputhandler.h
// inputhandler.h

#include <string>
#include <iostream>

class InputHandler
{
private:
    std::string inputBuffer;
    bool isInputMode;

public:
    InputHandler() : isInputMode(false), inputBuffer("") {}

    void startInputMode()
    {
        isInputMode = true;
        inputBuffer = "";
    }

    void stopInputMode()
    {
        isInputMode = false;
    }

    bool isInInputMode() const
    {
        return isInputMode;
    }

    void addCharacter(char c)
    {
        if (isInputMode && std::isdigit(c))
        {
            inputBuffer += c;
        }
    }

    void removeCharacter()
    {
        if (!inputBuffer.empty())
        {
            inputBuffer.pop_back();
        }
    }

    std::string getInput() const
    {
        return inputBuffer;
    }

    int getInputAsInt() const
    {
        if (inputBuffer.empty())
            return -1;
        try
        {
            return std::stoi(inputBuffer);
        }
        catch (...)
        {
            return -1;
        }
    }

    void clearInput()
    {
        inputBuffer = "";
    }
};
/**
 * @class InputHandler
 * @brief Gestisce l'input utente in modalità numerica per applicazioni interattive.
 *
 * @details
 * La classe InputHandler fornisce funzionalità per raccogliere e processare input numerico
 * da parte dell'utente. Offre controllo sulla modalità di input, buffering dei caratteri,
 * e conversione dell'input in formato intero.
 *
 * @usage
 * \code
 * InputHandler handler;
 *
 * // Avviare la modalità di input
 * handler.startInputMode();
 *
 * // Aggiungere caratteri numerici al buffer
 * if (handler.isInInputMode()) {
 *     handler.addCharacter('5');
 *     handler.addCharacter('3');
 * }
 *
 * // Recuperare l'input
 * std::string input = handler.getInput();      // "53"
 * int value = handler.getInputAsInt();         // 53
 *
 * // Ripulire e terminare
 * handler.clearInput();
 * handler.stopInputMode();
 * \endcode
 *
 * @note
 * - Solo i caratteri numerici (0-9) vengono accettati quando la modalità input è attiva
 * - Se il buffer è vuoto, getInputAsInt() ritorna -1
 * - removeCharacter() rimuove l'ultimo carattere dal buffer
 */
