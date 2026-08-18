#include <SFML/Graphics.hpp>
#include "chess.h"
#include "globale.h"
#include <vector>
#include <memory>
#include "proto.h"
#pragma warning(disable : 4244)

using namespace std;

/*
MODALITA' D'USO
Si usa inizialmente startGrafica() che :
se non è valorizzato il puntatore alla finestra scacchi, setta la variabile globale window e apre la finestra e lancia drawBoard.
Se window è già valorizzata esce senza aprire la window.
Conviene sempre dopo la startGraficachiamare drawBoard, per i casi in cui il puntatore window è già valorizzato, in questo modo
drawBoard prima di visualizzare i pezzi sulla scacchiera recupera da sola il puntatore window con getMyWindow.

navOnBoard recupera anche essa con getMyWindow il puntatore window e lo assegna a window_app, lavora e poi prima di chiamare drawBoard aggiorna con setWindow il puntatore window passando window_app.
Se ci si attiene a tale strategia la grafica fa correttamente il suo compito.
Per le animazioni si usa animationBoard al posto di drawBoard.

Da notare che per caricare i problemi e le sospensioni, per il refresh, si usa myRefresh() che utilizza il trucco
animationBoard(i,j,i,j)dove i,j sono riferiti ad un pezzo presente
*/

// Dimensioni scacchiera
const int spritesize = 150;
const int SIZE = 1000; // Dimensione quadrata
const int squaresize = SIZE / 8;

// puntatore alla finestra globale, inizialmente null
sf::RenderWindow *window = nullptr;

// Funzione helper per convertire coordinate scacchiera (0-7) in pixel
sf::Vector2f getCoords(int col, int row)
{
    return sf::Vector2f(col * squaresize, row * squaresize);
}

void startGrafica()
{
    if (window != nullptr)
    {
        return;
    }
    window = new sf::RenderWindow(sf::VideoMode(squaresize * 8, squaresize * 8), "Scacchi-it");
    //  limita il framerate per non sovraccaricare la CPU
    // dicendo al sistema di non mettere in 'freeze' il programma che altrimenti si blocca e non risponde
    window->setFramerateLimit(60);

    if (window->isOpen())
    {
        drawBoard();
    }
    return;
}

void drawBoard()
{
    sf::Color colorWhite(240, 217, 181); // Crema/Bianco
    sf::Color colorWood(181, 136, 99);   // Marrone/Legno

    // riprendo la finestra globale
    sf::RenderWindow *window_draw = getMyWindow();
    if (window_draw->isOpen())
    {

        window_draw->clear();
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                sf::RectangleShape square(sf::Vector2f(squaresize, squaresize));
                square.setPosition(i * squaresize, j * squaresize);
                square.setFillColor((i + j) % 2 == 0 ? colorWhite : colorWood);
                window_draw->draw(square);
            }
        }
        sf::Texture texture;
        sf::Sprite sprite;
        texture.setSmooth(true);
        string img;
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (chessBoard[i][j].getBusySquare() == true)
                {
                    // trovato pezzo, stabiliamo cosa è e carichiamo immagine
                    img.clear();
                    img = "..//pictures//";
                    img.append(chessBoard[i][j].getChessPiece().getNameImg());
                    //  carichiamo immagine da pictures
                    if (texture.loadFromFile(img.c_str()) == false)
                    {
                        printf("Errore caricamento %s da directory pictures\n", img.c_str());
                        callTextToSpeech(string("Errore directory pictures non trovata o immagini non trovate\n"));
                        exit(1); // Errore nel caricamento, salta questo pezzo
                    }
                    sprite.setTexture(texture);
                    sprite.setScale(((float)squaresize / spritesize), ((float)squaresize / spritesize));
                    sprite.setPosition(j * squaresize, i * squaresize);
                    window_draw->draw(sprite);
                }
            }
        }

        window_draw->display();
    }
    setWindow(window_draw);
    return;
}

void setWindow(sf::RenderWindow *p)
{
    window = p;
}

sf::RenderWindow *getMyWindow()
{
    return window;
}

void deleteWindow()
{
    delete window;
}

void animationBoard(int startRow, int startCol, int endRow, int endCol)
{
    sf::Color colorWhite(240, 217, 181); // Crema/Bianco
    sf::Color colorWood(181, 136, 99);   // Marrone/Legno
    sf::RenderWindow *window_draw = getMyWindow();

    if (!window_draw->isOpen())
        return;

    // === CARICAMENTO TEXTURE DEL PEZZO IN MOVIMENTO ===
    sf::Texture pezzoTexture;
    string img = "..//pictures//";

    img.append(chessBoard[startRow][startCol].getChessPiece().getNameImg());
    if (!pezzoTexture.loadFromFile(img.c_str()))
    {
        printf("Errore caricamento %s da directory pictures\n", img.c_str());
        callTextToSpeech(string("Errore directory pictures non trovata o immagini non trovate\n"));
        exit(1); // Errore nel caricamento
    }

    sf::Sprite pezzoSprite;
    pezzoSprite.setTexture(pezzoTexture);
    pezzoSprite.setScale(
        (float)squaresize / pezzoTexture.getSize().x,
        (float)squaresize / pezzoTexture.getSize().y);

    // === POSIZIONI INIZIALI E FINALI ===
    sf::Vector2f startPos = getCoords(startCol, startRow);
    sf::Vector2f endPos = getCoords(endCol, endRow);
    sf::Vector2f currentPos = startPos;
    pezzoSprite.setPosition(currentPos);

    // === PARAMETRI ANIMAZIONE ===
    float speed = 8.0f; // Pixel per frame
    bool isMoving = true;

    // === PRE-CARICA TUTTI I PEZZI STATICI (NON IN MOVIMENTO) ===
    vector<sf::Texture> staticTextures;
    vector<sf::Sprite> staticSprites;
    staticTextures.reserve(16); // Ottimizzazione alloc memoria
    staticSprites.reserve(16);

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            // Salta il pezzo in movimento e le caselle vuote
            if ((i == startRow && j == startCol) || !chessBoard[i][j].getBusySquare())
                continue;

            sf::Texture staticTexture;
            string staticImg = "..//pictures//";
            staticImg.append(chessBoard[i][j].getChessPiece().getNameImg());

            if (staticTexture.loadFromFile(staticImg.c_str()))
            {
                staticTextures.push_back(staticTexture);
                sf::Sprite staticSprite;
                staticSprite.setTexture(staticTextures.back());
                staticSprite.setScale(
                    (float)squaresize / spritesize,
                    (float)squaresize / spritesize);
                staticSprite.setPosition(j * squaresize, i * squaresize);
                staticSprites.push_back(staticSprite);
            }
        }
    }

    // === LOOP PRINCIPALE ANIMAZIONE ===
    while (isMoving && window_draw->isOpen())
    {
        // Gestisci gli eventi (chiusura finestra, ecc...)
        sf::Event event;
        while (window_draw->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window_draw->close();
                return;
            }
        }

        // === CALCOLO NUOVA POSIZIONE ===
        if (isMoving)
        {
            sf::Vector2f direction = endPos - currentPos;
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (distance > speed)
            {
                // Movimento proporzionale verso la destinazione
                currentPos.x += (direction.x / distance) * speed;
                currentPos.y += (direction.y / distance) * speed;
            }
            else
            {
                // Arrivato a destinazione
                currentPos = endPos;
                isMoving = false;
            }
            pezzoSprite.setPosition(currentPos);
        }

        // === DISEGNO SCENA ===
        window_draw->clear();

        // Disegna la scacchiera
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                sf::RectangleShape square(sf::Vector2f(squaresize, squaresize));
                square.setPosition(j * squaresize, i * squaresize);
                square.setFillColor((i + j) % 2 == 0 ? colorWhite : colorWood);
                window_draw->draw(square);
            }
        }

        // Disegna tutti i pezzi attuali (tranne quello in movimento)
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                // Salta il pezzo in movimento e le caselle vuote
                if ((i == startRow && j == startCol) || !chessBoard[i][j].getBusySquare())
                    continue;

                sf::Texture texture;
                string staticImg = "..//pictures//";
                staticImg.append(chessBoard[i][j].getChessPiece().getNameImg());

                if (texture.loadFromFile(staticImg.c_str()))
                {
                    sf::Sprite staticSprite;
                    staticSprite.setTexture(texture);
                    staticSprite.setScale(
                        (float)squaresize / spritesize,
                        (float)squaresize / spritesize);
                    staticSprite.setPosition(j * squaresize, i * squaresize);
                    window_draw->draw(staticSprite);
                }
            }
        }

        // Disegna il pezzo in movimento (sempre sopra gli altri)
        window_draw->draw(pezzoSprite);

        window_draw->display();
    }
}
void myRefresh()
{

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (chessBoard[i][j].getBusySquare())
            {
                animationBoard(i, j, i, j);
                break;
            }
        }
    }

    return;
}
