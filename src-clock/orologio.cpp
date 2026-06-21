#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <array>
#include "tts.h"

#pragma warning(disable : 4244)

using namespace std;

/*

 Orologio scacchistico
 C++ E SFML 2.6.1
 Ad ogni pressione della barra spaziatrice, viene segnato il cambio del turno sull'orologio e parte il tempo di chi è di turno.
Il cambio del turno viene indicato mettendo una sola scritta sotto i tempi, Es. Turno del Bianco  (oppure Nero).Non occorre mettere Bianco o Nero accanto i tempi.

Il tempo del giocatore di turno  viene diminuito per la durata del tempo che ha pensato la mossa e poi premuto la barra spaziatrice. Il tempo del giocatore di turno viene scalato fino a che non schiaccia la barra spaziatrice, dopo di che diventa di turno l'altro giocatore e si ripete il procedimento.
 .
 Durante la fase che il giocatori pensa il suo orologio si aggiorna ogni 5 secondi.
 Occorre segnalare vocalmente ai non vedenti
 la variazione del tempo dei due giocatori.
 Occorre un avviso vocale per chi ha terminato il tempo
Un messaggio vocale con callTextToSpeech per dire "Help: switch orologi barra spaziatrice , Control-Q per uscire"
*/

// Helper per formattare il tempo (MM:SS)
std::string formatTime(sf::Time time)
{
    int totalSeconds = static_cast<int>(time.asSeconds());
    if (totalSeconds < 0)
        totalSeconds = 0;
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << minutes << ":"
       << std::setw(2) << std::setfill('0') << seconds;
    return ss.str();
}

static std::string formatUnit(int value, const std::string &singular, const std::string &plural)
{
    std::stringstream ss;
    ss << value << ' ' << (value == 1 ? singular : plural);
    return ss.str();
}

// Formato parlato per TTS, ad esempio "5 minuti e 12 secondi"
std::string formatTimeForSpeech(sf::Time time)
{
    int totalSeconds = static_cast<int>(time.asSeconds());
    if (totalSeconds < 0)
        totalSeconds = 0;

    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    if (minutes == 0 && seconds == 0)
    {
        return "zero secondi";
    }

    std::stringstream ss;
    if (minutes > 0)
    {
        ss << formatUnit(minutes, "minuto", "minuti");
        if (seconds > 0)
        {
            ss << " e ";
        }
    }

    if (seconds > 0)
    {
        ss << formatUnit(seconds, "secondo", "secondi");
    }

    return ss.str();
}

static bool loadUiFont(sf::Font &font)
{
    const std::array<std::string, 4> fontPaths = {
        "../fonts/arial.ttf",
        "../fonts/segoeui.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/segoeui.ttf"};

    for (const std::string &fontPath : fontPaths)
    {
        if (font.loadFromFile(fontPath))
        {
            return true;
        }
    }

    return false;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {

        printf("Usage: clock [time_in_minutes]\n");
        return 1;
    }

    int setting_time = atoi(argv[1]);
    if (setting_time <= 0)
    {
        printf("Setting errato, uso 5 minuti.\n");
        setting_time = 5;
    }
    float timeInSeconds = static_cast<float>(setting_time * 60);
    sf::Time whiteTime = sf::seconds(timeInSeconds);
    sf::Time blackTime = sf::seconds(timeInSeconds);

    sf::RenderWindow window(sf::VideoMode(400, 200), "Chess Clock");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!loadUiFont(font))
    {
        std::cerr << "Errore caricamento font. Verifica la presenza di Arial o Segoe UI." << std::endl;
        callTextToSpeech("Attenzione: non riesco a caricare il font dell'interfaccia.");
    }

    sf::Text whiteText, blackText, turnText;
    whiteText.setFont(font);
    blackText.setFont(font);
    turnText.setFont(font);

    whiteText.setCharacterSize(30);
    blackText.setCharacterSize(30);
    turnText.setCharacterSize(20);

    whiteText.setPosition(50, 50);
    blackText.setPosition(250, 50);
    turnText.setPosition(130, 120);

    whiteText.setFillColor(sf::Color::Red);
    blackText.setFillColor(sf::Color::Red);
    turnText.setFillColor(sf::Color::Yellow);

    sf::Clock gameClock;
    sf::Time lastTick = gameClock.getElapsedTime();
    bool isWhiteTurn = true;
    bool clockStarted = false; // Il tempo parte solo dopo il primo spazio
    bool whiteTimeOverAnnounced = false;
    bool blackTimeOverAnnounced = false;

    auto updateClockTexts = [&]()
    {
        whiteText.setString("B: " + formatTime(whiteTime));
        blackText.setString("N:   " + formatTime(blackTime));
    };

    auto announceTimes = [&]()
    {
        std::string msg = "Bianco " + formatTimeForSpeech(whiteTime) +
                          ". Nero " + formatTimeForSpeech(blackTime);
        callTextToSpeech(msg);
    };

    auto syncTick = [&]()
    {
        lastTick = gameClock.getElapsedTime();
    };

    auto stopClockWithMessage = [&](const std::string &visualMessage, const std::string &voiceMessage)
    {
        clockStarted = false;
        turnText.setString(visualMessage);
        syncTick();
        callTextToSpeech(voiceMessage);
        syncTick();
    };

    auto updateRunningTime = [&]()
    {
        sf::Time now = gameClock.getElapsedTime();
        sf::Time elapsed = now - lastTick;
        lastTick = now;

        if (!clockStarted || elapsed <= sf::Time::Zero)
        {
            return;
        }

        sf::Time &activeTime = isWhiteTurn ? whiteTime : blackTime;
        bool &timeOverAnnounced = isWhiteTurn ? whiteTimeOverAnnounced : blackTimeOverAnnounced;

        activeTime -= elapsed;
        if (activeTime <= sf::Time::Zero)
        {
            activeTime = sf::Time::Zero;
            if (!timeOverAnnounced)
            {
                timeOverAnnounced = true;
                stopClockWithMessage(
                    isWhiteTurn ? "Tempo Bianco finito" : "Tempo Nero finito",
                    isWhiteTurn ? "Tempo del bianco terminato" : "Tempo del nero terminato");
            }
        }
    };

    // Aggiornamento iniziale testi prima del loop
    updateClockTexts();
    turnText.setString("Premi SPAZIO per iniziare");
    announceTimes();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {

                bool ctrlPressed = event.key.control;
                if (ctrlPressed)
                {
                    if (event.key.code == sf::Keyboard::Q)
                    {
                        window.close(); // Chiude la finestra
                    }
                }
            }

            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space)
            {
                // Se è la prima volta che premo spazio, faccio partire il clock
                if (!clockStarted)
                {
                    clockStarted = true;
                    turnText.setString("Turno: Bianco");
                    syncTick();
                    callTextToSpeech("Partita iniziata. Turno bianco.");
                    syncTick();
                    announceTimes();
                    syncTick();
                }
                else
                {
                    updateRunningTime();

                    // Switch turno
                    isWhiteTurn = !isWhiteTurn;
                    printf("Switch orologio\n");
                    turnText.setString(isWhiteTurn ? "Turno: Bianco" : "Turno: Nero");
                    syncTick();

                    if (isWhiteTurn)
                    {
                        callTextToSpeech("Turno bianco");
                    }
                    else
                    {
                        callTextToSpeech("Turno nero");
                    }

                    syncTick();
                    announceTimes();
                    syncTick();
                }
            }
        }

        updateRunningTime();

        updateClockTexts();

        // Rendering
        window.clear(sf::Color(30, 30, 30));
        window.draw(whiteText);
        window.draw(blackText);
        window.draw(turnText);
        window.display();
    }

    return 0;
}