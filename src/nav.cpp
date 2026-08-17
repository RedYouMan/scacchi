// nav.cpp
// Il modulo di navigazione permette al non vedente di scoprire come sono posizionati i pezzi sulla scacchiera, di sapere il nome della casa su cui si trova e di giocare selezionando la casa di partenza e quella di arrivo del pezzo che vuole muovere
#include <sfml/graphics.hpp>
#include <sfml/window.hpp>
#include <sfml/system.hpp>
#include <  windows.h>
#include <iostream>
#include <cctype>
#include "globale.h"
#include "proto.h"
#include "optraining.h"
#include "chess.h"
#include <string>
#include <vector>
#include "engine.h"
#include <atomic>
#include "pgnreader.h"
#pragma warning(disable : 4244)

using namespace std;

bool playerOk(string player);
void navOnBoard(char who)
{
    string msg;
    const int DIM = 8;
    const int WINDOW_SIZE = 1000;
    const int TILE_SIZE = WINDOW_SIZE / DIM;

    sf::RenderWindow *window_app = getMyWindow();

    window_app->clear();
    drawBoard();
    window_app->display();

    sf::RectangleShape selector(sf::Vector2f(TILE_SIZE, TILE_SIZE));

    selector.setFillColor(sf::Color::Transparent);
    selector.setOutlineColor(sf::Color::White); // Bordo bianco
    selector.setOutlineThickness(4.f);

    int curRow = gRow, curCol = gCol;
    sf::Vector2i startPos(-1, -1);
    sf::Vector2i endPos(-1, -1);

    auto selectSquare = [&](int row, int col, int &counter)
    {
        callTextToSpeech(string("selezionata la casa\n"));
        if (counter == 0)
        {
            startPos = sf::Vector2i(col, row);
            sourceOnBoard.clear();
            sourceOnBoard.push_back(chessBoard[row][col].getChessPiece().getNamePiece()[0]);
            counter++;
            // aiuto per undoMove
            if (who_play == 'W')
            {
                who = 'W';
                who_play = '@';
            }
            else if (who_play == 'B')
            {
                who = 'B';
                who_play = '@';
            }
            else
            {
                who_play = '@';
            }
            if (who != chessBoard[row][col].getChessPiece().getColorPiece())
            {
                if (chessBoard[row][col].getBusySquare() == false)
                {
                    callTextToSpeech(string("Mossa non valida: casa di partenza vuota"));
                }
                else
                {
                    callTextToSpeech(string("Non puoi giocare con questo colore, devi rispettare il turno di gioco\n"));
                }
                counter = 0;
            }
            ChessUtility utility;
            string casa = utility.getSquarePuntoDiVista(row, col, puntoDiVista);
            sourceOnBoard.append(casa);
        }
        else
        {
            endPos = sf::Vector2i(col, row);
            destOnBoard.clear();
            destOnBoard.push_back(chessBoard[row][col].getChessPiece().getNamePiece()[0]);
            ChessUtility utility;
            string casa = utility.getSquarePuntoDiVista(row, col, puntoDiVista);
            destOnBoard.append(casa);
            counter++;
            selector.setPosition(-1 * TILE_SIZE, -1 * TILE_SIZE);
            startPos = sf::Vector2i(-1, -1);
            endPos = sf::Vector2i(-1, -1);
        }
    };

    bool moved = false;
    if (window_app->isOpen())
    {
        sf::Event event;
        int counter = 0;
        while (counter < 2 && window_app->waitEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                if (online)
                {
                    sendMove(roomOnline, playerOnline, "none");
                }

                window_app->close();
                deleteWindow();
                if (isEngineRunning())
                    stop();
                exit(0);
            }
            // NON USATO: TROPPO LENTO
            // evento fittizio scatenato da simpleRefresh e usato in nav.cpp quando avvengono loadfile
            if (event.type == sf::Event::MouseMoved)
            {
                // Se arriva l'evento, il sistema è attivo
                drawBoard();
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    int mouseCol = event.mouseButton.x / TILE_SIZE;
                    int mouseRow = event.mouseButton.y / TILE_SIZE;
                    if (mouseCol >= 0 && mouseCol < DIM && mouseRow >= 0 && mouseRow < DIM)
                    {
                        curRow = mouseRow;
                        curCol = mouseCol;
                        gRow = mouseRow;
                        gCol = mouseCol;
                        onSquare(gRow, gCol);
                        selectSquare(mouseRow, mouseCol, counter);
                        selector.setPosition(mouseCol * TILE_SIZE, mouseRow * TILE_SIZE);
                        window_app->clear();
                        setWindow(window_app);
                        drawBoard();
                        window_app->draw(selector);
                        window_app->display();
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed)
            {
                bool ctrlPressed = event.key.control;
                if (ctrlPressed)
                {

                    // Internet giocatori remoti
                    if (event.key.code == sf::Keyboard::I)
                    {

                        if (online)
                        {
                            gioca(); // per non farlo proseguire
                        }
                        else
                        {
                            if (isEngineRunning())
                            {
                                stop();
                            }
                            reg_to_file = false;
                            deleteUndo();

                            callTextToSpeech(string("Stai per giocare online\n"));
                            clear_globali();
                            cleanBoard();
                            init();
                            online = true; // deve stare dopo la clear_globali

                            callTextToSpeech(string("Fai Alt-Tab erispondi alle domande \n"));
                            Sleep(2000);
                            std::cout << "Colore : ";
                            callTextToSpeech(string("Inserisci il colore con cui vuoi giocare (1=Bianco, 2=Nero)\n"));

                            int valore = 0;
                            cin >> valore;
                            cin.ignore();
                            if (valore == 1)
                            {
                                colorOnline = "Bianco";
                            }
                            else if (valore == 2)
                            {
                                colorOnline = "Nero";
                            }
                            else
                            {
                                callTextToSpeech(string("Valore non valido, esco per farti ricominciare correttamente\n"));
                                exit(0);
                            }
                            callTextToSpeech(string("Hai scelto di giocare con il colore ") + colorOnline + string("\n"));

                            // devo assegnare startColor per far partire come bianco o nero
                            startColor = (valore == 1 ? 0 : 1);

                            if (startColor == 0)
                            {
                                callTextToSpeech(string("Comunica al nero il nome della room avuta assegnata. Inoltre  prima di fare la mossa attendi che il nero si connetta alla room \n"));
                            }

                            if (startColor == 1)
                            {
                                numMove = 1; // altrimenti il nero parte con 0
                            }
                            Sleep(2000);
                            if (startColor == 0)
                            {
                                std::cout << "Room assegnata dal server: ";
                                roomOnline = roomSuggested();
                                std::cout << roomOnline << std::endl;

                                if (roomOnline.empty())
                                {
                                    callTextToSpeech(string("Controlla che il serverScacchi sia attivo. Esco per consentirti di avviare il server\n"));
                                    exit(0);
                                }
                                Sleep(5000);
                                callTextToSpeech(string("Comunica al nero la room assegnata ") + roomOnline);
                            }
                            else
                            {
                                std::cout << "Room  : ";
                                callTextToSpeech(string("Inserisci il nome della room che ti ha comunicato il giocatore bianco, seguito da return\n"));
                                cin >> roomOnline;
                                cin.ignore();
                            }

                            // Modifica
                            playerOnline.clear();
                            while (true)
                            {
                                Sleep(2000);
                                cout << "player : ";
                                Sleep(3000);
                                callTextToSpeech(string("Inserisci il nome con cui vuoi giocare, seguito da return ma non usare spazi o trattini\n"));
                                std::getline(cin, playerOnline);
                                if (!cin)
                                {
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    playerOnline.clear();
                                    continue;
                                }
                                if (playerOnline.empty())
                                {
                                    continue;
                                }
                                if (playerOk(playerOnline))
                                {
                                    break;
                                }
                                Sleep(3000);

                                callTextToSpeech(string("Nome non valido, deve contenere solo caratteri alfanumerici e senza spazi o trattini\n"));
                            }
                            if (playerOnline.empty() || roomOnline.empty())
                            {
                                callTextToSpeech(string("Valori non validi, esco per farti ricominciare correttamente\n"));
                                exit(0);
                            }
                            Sleep(3000);
                            callTextToSpeech(string("Fai Alt-Tab per tornare alla scacchiera\n"));
                            myRefresh();
                            gioca();
                        }
                    }
                    // undo
                    if (event.key.code == sf::Keyboard::U)
                    {

                        undoMove();
                        myRefresh();
                        // window_app->display();
                        gioca();
                    }

                    if (event.key.code == sf::Keyboard::Q)
                    {
                        if (online)
                        {
                            sendMove(roomOnline, playerOnline, "none");
                        }

                        window_app->close();
                        deleteWindow();
                        callTextToSpeech(string("Grazie per aver giocato a scacchi con il nostro programma\n"));

                        if (isEngineRunning())
                        {
                            stop();
                        }
                        exit(0);
                    }
                    if (event.key.code == sf::Keyboard::L)
                    {
                        callTextToSpeech(string("Abbandona!"));
                        if (online)
                        {
                            sendMove(roomOnline, playerOnline, "quitAB");
                        }
                        if (who == 'W')
                        {
                            ultimaMossa.append("0-1");
                        }
                        else
                        {
                            ultimaMossa.append("1-0");
                        }
                        WriteGameToFile(nameFile, ultimaMossa, reg_to_file);
                        if (isEngineRunning())
                        {
                            stop();
                        }
                        reStart();
                    }

                    if (event.key.code == sf::Keyboard::T)
                    {
                        // OPENINGS TRAINER

                        if (online)
                        {
                            gioca();
                        }
                        else
                        {

                            deleteUndo();
                            reg_to_file = false;
                            numMove = 0;
                            clear_globali();
                            cleanBoard();
                            readChessParam();
                            init();
                            drawBoard();
                            callTextToSpeech(string("Caricamento OpeningsTrainer di Scacchi-it, attendi e poi alla fine premi invio\n"));

                            std::string partita = openingsTrainer();
                            if (partita.empty())
                            {
                                callTextToSpeech(string("apertura non caricata\n"));
                                return;
                            }
                            /*

                            In questo punto faccio vedere la partita su scacchiera procedendo in questo modo:
                            0. rilevo i runtime errore e qualsiasi eccezione indicandone il motivo
                            1. setto una variabile intera color_who numMosse a zero.
                            2. definisco un char whoche rappresenta chi gioca valorizzato a 'W' inizialmente ('W' o 'B')
                            3. in ciclo finche non termino la 15ma mossa, caratteristica dell'apertura, o termina la stringa partita faccio:
                            4. incremento numMosse
                            6. estraggo una mossa dalla stringa partita eliminando i 1. 2.etc
                            7. chiamo whatInitialSquare passando in input la mossa e who
                            8. chiamo insertMoveNav con who e poi successivamente chiamo drawboard
        9. se numMosse è pari dopo l'incremento faccio pausa attendendo lo space dell'utente avvisandolo di fare space
        10. cambio il valore di who
                            11. continuo il ciclo
                            12. alla fine del ciclo chiamo drawBoard per mostrare la posizione finale dell'apertura
                            */

                            try
                            {
                                PGNReader reader;
                                int numMosse = 0;
                                char who = 'W';
                                int maxMosse = 15;
                                int status = 0;
                                size_t pos = 0;
                                string mossa;
                                bool escapePressed = false;

                                while (numMosse < maxMosse && pos < partita.length() && !escapePressed)
                                {
                                    numMosse++;

                                    // Estrai mossa eliminando numeri e punti (1. 2. etc)

                                    // Prevenire autofrange sulla stringa partita: valutare prima se pos finirebbe fuori stringa
                                    // e solo dopo avanzare pos fino a trovare un carattere che non sia numero, punto o spazio
                                    while (pos < partita.length() && (isdigit(partita[pos]) || partita[pos] == '.' || partita[pos] == ' '))
                                        pos++;

                                    if (pos >= partita.length())
                                    {
                                        break;
                                    }
                                    // Estrai la mossa (generalmente 2-3 caratteri: e2 per un pedone, Ac2 per un alfiere, etc)
                                    size_t endPos = pos;
                                    while (endPos < partita.length() && !isspace(partita[endPos]) && partita[endPos] != '.')
                                        endPos++;

                                    mossa = partita.substr(pos, endPos - pos);
                                    pos = endPos;

                                    if (mossa.empty())
                                    {
                                        break;
                                    }

                                    // Estrai la casa iniziale e finale dalla mossa
                                    status = whatInitialSquare(mossa, who);
                                    if (status == 2)
                                    {
                                        break; // fine partita esco dal ciclo
                                    }
                                    // stampa sourceOnBoard e destOnBoard per debug
                                    // reader.writeTrace("..//logdir//trace.txt", "Mossa: " + mossa + ", Source: " + sourceOnBoard + ", Dest: " + destOnBoard + ", Who: " + who + "\n");
                                    // Inserisci la mossa sulla scacchiera
                                    status = insertMoveNav(who);
                                    if (status == 0)
                                    {
                                        callTextToSpeech(string("Errore nell'inserimento della mossa, la mossa ") + mossa + string(" non è valida, interrompo il caricamento dell'apertura. Premi esc per uscire dall'apertura\n"));
                                        break; // Esce dal ciclo while invece di return
                                    }
                                    drawBoard();
                                    whoPlayed = 'W'; // inizializzazione
                                    // Se numMosse è pari, pausa e aspetta lo spazio
                                    if (numMosse % 2 == 0)
                                    {
                                        numMove++;
                                        whoPlayed = 'B';
                                        callTextToSpeech(string("Premi SPAZIO per continuare oppure  esc per uscire dall'apertura \n"));
                                        while (window_app->isOpen())
                                        {
                                            sf::Event evt;
                                            if (window_app->waitEvent(evt))
                                            {
                                                if (evt.type == sf::Event::KeyPressed && evt.key.code == sf::Keyboard::Space)
                                                    break;

                                                if (evt.type == sf::Event::KeyPressed && evt.key.code == sf::Keyboard::Escape)
                                                {
                                                    escapePressed = true;
                                                    break;
                                                }
                                                if (evt.type == sf::Event::Closed)
                                                {
                                                    window_app->close();
                                                    return;
                                                }
                                            }
                                        }
                                    }

                                    // Cambia il colore del giocatore
                                    who = (who == 'W') ? 'B' : 'W';
                                }

                                if (!escapePressed)
                                {
                                    window_app->clear();
                                    drawBoard();
                                    window_app->display();

                                    callTextToSpeech(string("Apertura caricata con successo\n"));
                                    numMove++; // devo incrementarla di 1 per gesfile
                                }
                            }
                            catch (const exception &e)
                            {
                                callTextToSpeech(string("Errore nel caricamento dell'apertura: ") + e.what() + "\n");
                            }
                        }
                    }
                    if (event.key.code == sf::Keyboard::A)
                    {

                        if (online)
                        {
                            callTextToSpeech(string("Non puoi forzare aperture in modalità online\n"));
                            gioca();
                        }
                        else
                        {

                            deleteUndo();
                            reg_to_file = false;

                            forzaApertura();
                            callTextToSpeech(string("Fai Alt-Tab per tornare alla scacchiera\n"));
                            Sleep(2000);
                            gioca();
                        }
                    }
                    if (event.key.code == sf::Keyboard::S)
                    {
                        if (online)
                        {
                            callTextToSpeech(string("Non si sospende la partita su online"));
                            gioca();
                        }
                        else
                        {
                            if (sospendi_test == true)
                                isProblem = false;
                            if (isProblem)
                            {
                                callTextToSpeech(string("I problemi non si sospendono\n"));
                            }
                            else
                            {
                                callTextToSpeech(string("Fai Alt-Tab per andare sulla console e inserire il nome del file di sospensione\n"));
                                Sospendi(false);
                                if (isEngineRunning())
                                {
                                    stop();
                                }
                            }
                        }
                    }
                    if (event.key.code == sf::Keyboard::G)
                    {

                        // non deve cambiare numero di mossa e chi gioca
                        // prelevo stato degli arrocchi
                        // inizialmente sopra è il nero e sotto il bianco per iniziare a comprendere ma tutto ciò è dipendente solo da chi sta sopra e chi sotto all'atto del ctrlG

                        bool sopra00 = chessBoard[0][7].getValid00();
                        bool sopra000 = chessBoard[0][0].getValid000();
                        bool sotto00 = chessBoard[7][7].getValid00();
                        bool sotto000 = chessBoard[7][0].getValid000();

                        puntoDiVista = (puntoDiVista == "Bianco" ? "Nero" : "Bianco");
                        msg = "Hai scelto il punto di vista " + puntoDiVista + "\n";
                        callTextToSpeech(msg);
                        Sospendi(true);
                        loadFile(2, true);
                        // inverto lo stato degli arrocchi

                        /*ad es. bianco che va  sopra e nero sotto*/
                        chessBoard[0][0].setValid00(sotto00);
                        chessBoard[0][7].setValid000(sotto000);
                        // ad es. il nero
                        chessBoard[7][0].setValid00(sopra00);
                        chessBoard[7][7].setValid000(sopra000);

                        startGrafica();
                        drawBoard();
                        gioca();
                    }
                    if (event.key.code == sf::Keyboard::X)
                        apriUrl();

                    if (event.key.code == sf::Keyboard::N)
                    {
                        if (online)
                        {
                            sendMove(roomOnline, playerOnline, "none");
                        }

                        // Inizializzazione della scacchiera
                        if (isEngineRunning())
                        {
                            stop();
                            callTextToSpeech(string("Fermo il computer\n"));
                        }
                        reg_to_file = false;
                        clear_globali();
                        cleanBoard();
                        readChessParam();
                        init();
                        drawBoard();
                        callTextToSpeech(string("Scacchiera inizializzata\n"));
                        gioca();
                    }

                    if (event.key.code == sf::Keyboard::J)
                    {

                        if (online)
                        {
                            callTextToSpeech(string("Non puoi risolvere problemi in modalità online\n"));
                            gioca();
                        }
                        else
                        {

                            deleteUndo();
                            setWindow(window_app);
                            gestione_Problemi(2);
                        }
                    }
                    if (event.key.code == sf::Keyboard::Z)
                    {

                        if (online)
                        {
                            callTextToSpeech(string("Non puoi giocare contro il computer in modalità online\n"));
                            gioca();
                        }
                        else
                        {
                            deleteUndo();
                            if (start())
                            {
                                callTextToSpeech(string("Il computer risulta pronto agiocare\n"));
                                setParam(maxThreads, maxHashTable, multiPV);

                                reg_to_file = false;
                                callTextToSpeech(string("Fai Alt-Tab per andare sulla console e rispondi alle domande"));
                                whatColorComputer();
                                setSkill(skill);
                                callTextToSpeech(string("Fai Alt-Tab per tornare alla scacchiera\n"));
                                Sleep(3000);
                                init();
                                gioca();
                            }
                            else
                            {
                                callTextToSpeech(string("computer is not ready to play for problems!\n"));
                            }
                        }
                    }
                    if (event.key.code == sf::Keyboard::P)
                    {

                        if (online)
                        {
                            callTextToSpeech(string("Non puoi risolvere problemi in modalità online\n"));
                            gioca();
                        }
                        else
                        {
                            who_play = '@';
                            deleteUndo();
                            setWindow(window_app);
                            gestione_Problemi(1);
                        }
                    }
                    if (event.key.code == sf::Keyboard::R)
                    {

                        if (online)
                        {
                            callTextToSpeech(string("Non puoi riprendere partite in modalità online\n"));
                            gioca();
                        }
                        else
                        {
                            who_play = '@';
                            deleteUndo();

                            if (isProblem)
                            {
                                callTextToSpeech(string("I problemi non si riprendono da una sospensione\n"));
                            }
                            else
                            {
                                clear_globali();
                                callTextToSpeech(string("Fai Alt-Tab per andare sulla console e inserire il nome del file da riprendere\n"));
                                loadFile(2, false);
                                checkKings();
                                checkNumbers();
                                checkMate();
                                myRefresh();
                                callTextToSpeech(string("Fai Alt-Tab per tornare sulla scacchiera\n"));
                                gioca();
                            }
                        }
                    }
                    if (event.key.code == sf::Keyboard::W)
                    {
                        if (online)
                        {
                            callTextToSpeech(string("Non si registra la partita quando sei online"));
                        }
                        else
                        {

                            deleteUndo();
                            if (isProblem)
                            {
                                callTextToSpeech(string("Con i problemi non si registra\n"));
                            }
                            else if (numMove <= 1)
                            {
                                clear_globali();
                                callTextToSpeech(string("Fai Alt-Tab per spostarti sulla console e rispondi alle domande"));
                                insertNameFile();
                                init();
                                startGrafica();
                                drawBoard();
                                callTextToSpeech(string("Fai Alt-Tab per spostarti sulla scacchiera"));
                                Sleep(2000);
                                gioca();
                            }
                            else
                            {
                                callTextToSpeech(string("Non puoi registrare a partita già in corso\n"));
                            }
                        }
                    }
                    if (event.key.code == sf::Keyboard::H)
                        help();
                }

                if (event.key.code == sf::Keyboard::Left && curCol > 0)
                {
                    curCol--;
                    moved = true;
                }
                else if (event.key.code == sf::Keyboard::Right && curCol < DIM - 1)
                {
                    curCol++;
                    moved = true;
                }
                else if (event.key.code == sf::Keyboard::Up && curRow > 0)
                {
                    curRow--;
                    moved = true;
                }
                else if (event.key.code == sf::Keyboard::Down && curRow < DIM - 1)
                {
                    curRow++;
                    moved = true;
                }
                else
                {
                    moved = false;
                }

                if (moved)
                {
                    gCol = curCol;
                    gRow = curRow;
                    onSquare(gRow, gCol);
                    // selector.setPosition(curCol * TILE_SIZE, curRow * TILE_SIZE);
                    window_app->clear();
                    setWindow(window_app);
                    drawBoard();
                    window_app->draw(selector);
                    window_app->display();
                }

                if (event.key.code == sf::Keyboard::Enter)
                {
                    selectSquare(curRow, curCol, counter);
                    selector.setPosition(curCol * TILE_SIZE, curRow * TILE_SIZE);
                    window_app->clear();
                    setWindow(window_app);
                    drawBoard();
                    window_app->draw(selector);
                    window_app->display();
                }
            }
        }
        setWindow(window_app);
    }
    return;
}
#pragma warning(disable : 4244)

using namespace std;

void onSquare(int row, int col)
{
    ChessUtility utility;
    string casa = utility.getSquarePuntoDiVista(row, col, puntoDiVista);
    callTextToSpeech(casa);
    if (chessBoard[gRow][gCol].getBusySquare() == true)
    {
        string pezzo = chessBoard[gRow][gCol].getChessPiece().getNamePiece();
        callTextToSpeech(string(pezzo.c_str()));
    }
    else
    {
        callTextToSpeech(string("vuota\n"));
    }
    return;
}

// Funzione per aprire un URL nel browser predefinito
void apriUrl()
{
    const std::string url = "https://github.com/RedYouMan/redyouman.github.io/raw/main/_posts/tutorials/scacchiGr.txt";

    string comando;
#ifdef _WIN32 // Se siamo su Windows
    // Comando specifico per Windows usando ShellExecute (più robusto)
    // Nota: questo richiede <windows.h> e potrebbe essere più complesso,
    // ma per semplicità usiamo system() che funziona anche su Windows per certi casi.
    // Per una soluzione Windows nativa, vedi ShellExecute nel codice commentato.
    comando = "start \"\" \"" + url + "\"";
    // Per Windows, si potrebbe anche usare:
    // ShellExecute(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
#elif __APPLE__ // Se siamo su macOS
    comando = "open \"" + url + "\"";
#else           // Su Linux e altri sistemi Unix-like
    comando = "xdg-open \"" + url + "\"";
#endif

    // Esegue il comando nel terminale del sistema operativo
    int risultato = system(comando.c_str());

    if (risultato != 0)
    {
        std::cerr << "Errore nell'apertura dell'URL. Codice di errore: " << risultato << std::endl;
    }
}

void whatColorComputer()
{
    string msg;
    Sleep(2000);
    printf("Dati da inserire\n");
    printf("colore : ");
    Sleep(1500);
    callTextToSpeech(string("Con quale colore deve giocare il computer, rispondi e fai return (1=Bianco, 2=Nero )?\n"));
    int risposta;
    cin >> risposta;
    cin.ignore();
    if (risposta == 1)
    {
        stockfish_color = "Bianco";
        puntoDiVista = "Bianco";
    }
    else if (risposta == 2)
    {
        stockfish_color = "Nero";
        puntoDiVista = "Bianco";
    }
    else
    {
        callTextToSpeech(string("Sono ammessi solo i valori 1 o 2, esco per farti ricominciare correttamente\n"));
        exit(0);
    }
    callTextToSpeech(string());
    msg = "Il computer gioca in " + stockfish_color + "\n";
    callTextToSpeech(msg);
    printf("skill : ");
    Sleep(1500);
    callTextToSpeech(string("Che livello di skill vuoi giocare, rispondi e fai return (da 0 a 20)?\n"));
    cin >> skill;
    cin.ignore();
    if (skill < 0 || skill > 20)
    {
        callTextToSpeech(string("valore non valido, assegnato skill 0\n"));
        skill = 0;
    }
    else
    {
        msg.clear();
        msg = "Scelto skill " + to_string(skill) + "\n";
        callTextToSpeech(msg);
    }

    return;
}

void gestione_Problemi(int caso)
{

    /*
    funzione chiamata in navOnBoard nel caso 1 per cntrlP,
    nel caso 2 per cntrlJ
    */

    clear_globali();
    isProblem = true;
    callTextToSpeech(string("Fai Alt - Tab per spostarti sulla console e inserire il nome del problema"));
    cleanBoard();

    loadFile(1, false);

    checkKings();
    checkNumbers();
    checkMate();

    callTextToSpeech(string("Fai Alt-Tab per tornare sulla scacchiera e attendi il caricamento grafico del problema"));
    myRefresh();
    if (caso == 2)
    {
        Sleep(3000);
        risolviProblema(startColor);
    }
    else
    {
        gioca();
    }

    return;
}

bool playerOk(string player)
{
    bool valido = true;
    for (char c : player)
    {
        if (!isalnum(c))
        {
            valido = false;
            break; // Trovato un carattere non valido, esci dal ciclo
        }
    }

    if (valido)
    {
        // cout << "Nome player accettato: " << player << endl;
    }
    else
    {
        cout << "il nome del player deve contenere solo lettere e numeri " << endl;
    }
    return valido;
}
