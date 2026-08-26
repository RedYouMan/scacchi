// openingstrainer.cpp
#pragma warning(disable : 4244 4267)
/*
 Il programma consente di selezionare un file pgn presenti nella cartella pgn_data e eseguire le prime dieci mosse della partita scelta sulla scacchiera,
 per apprendere l'apertura.
*/
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include "proto.h"
#include "inputhandler.h"
#include "pgnreader.h"
#include "combobox.h"

using namespace std;

string openingsTrainer()
{

    std::string pgnPath = "../pgn_data";
    std::string picturesPath = "../pictures";

    sf::RenderWindow window2(sf::VideoMode(1200, 800), "OpeningsTrainer Scacchi-it");

    // Crea una texture a scacchiera
    sf::Image checkerboard;
    checkerboard.create(200, 200, sf::Color::White);
    for (int y = 0; y < 200; y += 50)
    {
        for (int x = 0; x < 200; x += 50)
        {
            if ((x / 50 + y / 50) % 2 == 0)
            {
                for (int py = y; py < y + 50 && py < 200; py++)
                {
                    for (int px = x; px < x + 50 && px < 200; px++)
                    {
                        checkerboard.setPixel(px, py, sf::Color(210, 180, 140)); // Colore legno chiaro
                    }
                }
            }
            else
            {
                for (int py = y; py < y + 50 && py < 200; py++)
                {
                    for (int px = x; px < x + 50 && px < 200; px++)
                    {
                        checkerboard.setPixel(px, py, sf::Color(160, 110, 70)); // Colore legno scuro
                    }
                }
            }
        }
    }

    sf::Texture bgTexture;
    bgTexture.loadFromImage(checkerboard);
    bgTexture.setRepeated(true);

    sf::Sprite background(bgTexture);
    const float windowWidth = static_cast<float>(window2.getSize().x);
    const float windowHeight = static_cast<float>(window2.getSize().y);
    background.setScale(windowWidth / bgTexture.getSize().x, windowHeight / bgTexture.getSize().y);
    window2.setFramerateLimit(60);

    PGNReader reader;
    ComboBox comboBox;
    // Board board;
    InputHandler inputHandler;

    sf::Font font;
    if (!font.loadFromFile("../fonts/arial.ttf"))
    {
        std::cerr << "Error loading font" << std::endl;
        return "";
    }

    auto pgnFiles = reader.getOpeningsBooks(pgnPath);
    comboBox.loadItems(pgnFiles, reader);
    const float comboWidth = 400.f;
    const float comboHeight = 50.f;
    comboBox.setSize(comboWidth, comboHeight);
    comboBox.setPosition((windowWidth - comboWidth) / 2.f, 500.f);

    sf::Texture kingTexture;
    if (!kingTexture.loadFromFile("../pictures/ReNero.png"))
    {
        std::cerr << "Error loading king image" << std::endl;
    }

    sf::Sprite kingSprite(kingTexture);
    kingSprite.setScale(1.5f, 1.5f);
    sf::FloatRect kingBounds = kingSprite.getLocalBounds();
    kingSprite.setOrigin(kingBounds.left + kingBounds.width / 2.f, kingBounds.top + kingBounds.height / 2.f);
    kingSprite.setPosition(windowWidth / 2.f, 230.f);

    sf::Text titleText("OpeningsTrainer", font, 40);
    titleText.setFillColor(sf::Color::Black);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.left + titleBounds.width / 2.f, titleBounds.top + titleBounds.height / 2.f);
    titleText.setPosition(windowWidth / 2.f, 80.f);

    sf::Text subtitleText("Scacchi-it", font, 30);
    subtitleText.setFillColor(sf::Color::Black);
    sf::FloatRect subtitleBounds = subtitleText.getLocalBounds();
    subtitleText.setOrigin(subtitleBounds.left + subtitleBounds.width / 2.f, subtitleBounds.top + subtitleBounds.height / 2.f);
    subtitleText.setPosition(windowWidth / 2.f, 360.f);

    sf::Text chooseText("Scegli l'Openings Book", font, 24);
    chooseText.setFillColor(sf::Color::Black);
    sf::FloatRect chooseBounds = chooseText.getLocalBounds();
    chooseText.setOrigin(chooseBounds.left + chooseBounds.width / 2.f, chooseBounds.top + chooseBounds.height / 2.f);
    chooseText.setPosition(windowWidth / 2.f, 450.f);

    std::cout << "Benvenuti a OpeningsTrainer di Scacchi-it. Per iniziare, premi invio. Usa le frecce su e giu per navigare nei menu, Control Q per uscire" << std::endl;

    enum State
    {
        MENU,
        OPENING_SELECTION,
        GAME_NUMBER_SELECTION,
        GAME_PLAYING
    };
    State currentState = MENU;

    int selectedGameNumber = -1;
    std::string selectedFileName;
    int gameIndex = 0;
    int totalGames = 0;

    while (window2.isOpen())
    {
        sf::Event event;
        while (window2.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
                window2.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Q && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                {
                    window2.close();
                }
                if (inputHandler.isInInputMode())
                {
                    if (event.key.code == sf::Keyboard::BackSpace)
                    {
                        inputHandler.removeCharacter();
                    }
                }
            }
            if (event.type == sf::Event::TextEntered)
            {
                if (inputHandler.isInInputMode() && event.text.unicode < 128)
                {
                    inputHandler.addCharacter(static_cast<char>(event.text.unicode));
                }
            }
        }

        if (currentState == MENU)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                currentState = OPENING_SELECTION;
                callTextToSpeech("Seleziona dalla combobox l'apertura desiderata con le frecce e fai return");
            }

            window2.clear(sf::Color::White);
            window2.draw(background);
            window2.draw(titleText);
            window2.draw(kingSprite);
            window2.draw(subtitleText);
            window2.draw(chooseText);
            window2.display();
        }
        else if (currentState == OPENING_SELECTION)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                comboBox.selectPrevious();
                int gameCount = comboBox.getSelectedGameCount();
                callTextToSpeech(comboBox.getSelectedFileName() + ", " + std::to_string(gameCount) + " partite");
                sf::sleep(sf::milliseconds(200));
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                comboBox.selectNext();
                int gameCount = comboBox.getSelectedGameCount();
                callTextToSpeech(comboBox.getSelectedFileName() + ", " + std::to_string(gameCount) + " partite");
                sf::sleep(sf::milliseconds(200));
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                selectedFileName = comboBox.getSelectedFileName();
                gameIndex = comboBox.getSelectedIndex();
                reader.readFile(pgnFiles[gameIndex]);
                totalGames = reader.getGamesCount();
                selectedGameNumber = 1;
                inputHandler.startInputMode();
                callTextToSpeech("Inserisci il numero di partita desiderato e fai return");
                currentState = GAME_NUMBER_SELECTION;
            }

            window2.clear(sf::Color::White);
            window2.draw(background);
            window2.draw(titleText);
            window2.draw(kingSprite);
            window2.draw(subtitleText);
            window2.draw(chooseText);
            comboBox.draw(window2);
            window2.display();
        }
        else if (currentState == GAME_NUMBER_SELECTION)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                int inputValue = inputHandler.getInputAsInt();
                if (inputValue > 0 && inputValue <= totalGames)
                {
                    selectedGameNumber = inputValue;
                    callTextToSpeech("Hai scelto la partita numero " + std::to_string(selectedGameNumber));
                    inputHandler.stopInputMode();
                    currentState = GAME_PLAYING;
                    sf::sleep(sf::milliseconds(500));
                }
                else
                {
                    callTextToSpeech("Numero partita non valido. Inserisci un numero tra 1 e " + std::to_string(totalGames));
                    inputHandler.clearInput();
                }
            }
            sf::Text gameNumberText("Partita: " + inputHandler.getInput() + " di " + std::to_string(totalGames), font, 32);
            gameNumberText.setPosition(250, 400);
            gameNumberText.setFillColor(sf::Color::Black);

            window2.clear(sf::Color::White);
            window2.draw(background);
            window2.draw(gameNumberText);
            window2.display();
        }
        else if (currentState == GAME_PLAYING)
        {
            std::string moves_it;
            const std::string *moves_4it = reader.getMoves(selectedGameNumber - 1);

            if (moves_4it != nullptr && !moves_4it->empty())
            {
                moves_it = reader.translateMoves(*moves_4it);
                if (!moves_it.empty())
                {
                    // reader.writeTrace("../logdir/trace.txt", moves_it);
                }
            }
            else
            {
                moves_it = "";
            }

            const std::string *moves = reader.getMoves(selectedGameNumber - 1);
            if (moves != nullptr && !moves->empty())
            {
                std::string infoMoves = reader.getInfoMoves(selectedGameNumber - 1);
                if (!infoMoves.empty())
                {
                    callTextToSpeech(infoMoves);
                }
            }

            // chiude questa finestra
            window2.close();
            return moves_it;
        }
    }
    return "";
}