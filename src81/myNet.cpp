#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cctype>
#include <direct.h>
#pragma comment(lib, "Ws2_32.lib")
#include "engine.h"
#include "proto.h"

std::string trim(const std::string &value);
static std::string SERVER_ADDR = "127.0.0.1";
static unsigned short SERVER_PORT = 65432;

static bool loadServerConfig(std::string &serverAddr, unsigned short &serverPort)
{
    const char *configPath = "..//serverScacchi//server.cnf";
    char cwd[MAX_PATH];
    if (_getcwd(cwd, MAX_PATH) != nullptr)
    {
        // std::cout << "Directory corrente: " << cwd << std::endl;
    }
    else
    {
        std::cout << "Impossibile ottenere directory corrente." << std::endl;
    }
    // std::cout << "Percorso file configurazione cercato: " << configPath << std::endl;
    std::ifstream configFile(configPath);
    if (!configFile.is_open())
    {
        std::cout << "File di configurazione non trovato. Utilizzo valori di default:" << std::endl;
        std::cout << "Indirizzo server (default): " << serverAddr << std::endl;
        std::cout << "Porta server (default): " << serverPort << std::endl;
        return false;
    }

    bool gotAddress = false;
    bool gotPort = false;
    std::string line;
    while (std::getline(configFile, line))
    {
        line = trim(line);
        if (line.empty() || line[0] == '#')
            continue;

        size_t pos = line.find('=');
        if (pos == std::string::npos)
            pos = line.find(':');
        if (pos == std::string::npos)
            continue;

        std::string key = trim(line.substr(0, pos));
        std::string value = trim(line.substr(pos + 1));

        if (key.rfind("\xEF\xBB\xBF", 0) == 0)
            key.erase(0, 3);

        for (char &c : key)
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

        if (key == "serveraddr" || key == "serveraddress" || key == "ip" || key == "ipaddress" || key == "addr" || key == "address" || key == "host" || key == "hostname" || key == "server_ip" || key == "server_address")
        {
            serverAddr = value;
            // std::cout << "Caricato indirizzo server dal file: " << serverAddr << std::endl;
            gotAddress = true;
        }
        else if (key == "serverport" || key == "port" || key == "portnumber")
        {
            try
            {
                unsigned long portValue = std::stoul(value);
                if (portValue > 0 && portValue <= 65535)
                {
                    serverPort = static_cast<unsigned short>(portValue);
                    // std::cout << "Caricata porta server dal file: " << serverPort << std::endl;

                    gotPort = true;
                }
            }
            catch (...)
            {
            }
        }

        if (gotAddress && gotPort)
            break;
    }

    if (!gotAddress)
        std::cout << "Indirizzo server non trovato nel file. Utilizzo default: " << serverAddr << std::endl;
    if (!gotPort)
        std::cout << "Porta server non trovata nel file. Utilizzo default: " << serverPort << std::endl;

    return true;
}

static bool initializeWinsock()
{
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

static void cleanupWinsock()
{
    WSACleanup();
}

static std::string trim(const std::string &value)
{
    size_t start = 0;
    while (start < value.size() && std::isspace(static_cast<unsigned char>(value[start])))
        ++start;
    size_t end = value.size();
    while (end > start && std::isspace(static_cast<unsigned char>(value[end - 1])))
        --end;
    return value.substr(start, end - start);
}

static bool connectToServer(SOCKET &sock)
{
    loadServerConfig(SERVER_ADDR, SERVER_PORT);
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET)
        return false;

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR.c_str(), &serverAddr.sin_addr);

    if (connect(sock, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
    {
        closesocket(sock);
        sock = INVALID_SOCKET;
        return false;
    }
    return true;
}

static bool sendRequest(const std::string &request, std::string &response)
{
    if (!initializeWinsock())
        return false;

    SOCKET sock = INVALID_SOCKET;
    bool ok = false;
    if (connectToServer(sock))
    {
        int totalSent = 0;
        int toSend = static_cast<int>(request.size());
        while (totalSent < toSend)
        {
            int sent = send(sock, request.c_str() + totalSent, toSend - totalSent, 0);
            if (sent == SOCKET_ERROR || sent == 0)
                break;
            totalSent += sent;
        }

        if (totalSent == toSend)
        {
            response.clear();
            char buffer[512];
            while (true)
            {
                int bytes = recv(sock, buffer, static_cast<int>(sizeof(buffer) - 1), 0);
                if (bytes <= 0)
                    break;
                response.append(buffer, bytes);
                if (response.find('\n') != std::string::npos)
                    break;
            }

            while (!response.empty() && (response.back() == '\n' || response.back() == '\r'))
                response.pop_back();
            response = trim(response);

            ok = !response.empty();
        }
        closesocket(sock);
    }
    cleanupWinsock();
    return ok;
}

bool sendMove(const std::string &room, const std::string &player, const std::string &move)
{
    if (isEngineRunning())
    {
        stop();
        callTextToSpeech(string("Non puoi giocare online mentre l'engine è in esecuzione, ferma l'engine per poter giocare online\n"));
        return false;
    }

    std::string roomValue = trim(room);
    std::string playerValue = trim(player);
    std::string moveValue = trim(move);
    std::string request = "SEND " + roomValue + " " + playerValue + " " + moveValue + "\n";
    std::string response;

    int attempts = 0;
    const int maxAttempts = 2;

    while (attempts < maxAttempts)
    {
        attempts++;
        if (sendRequest(request, response))
        {
            if (response.rfind("OK", 0) == 0)
                return true;
            if (attempts >= maxAttempts)
                break;
        }
        else if (attempts >= maxAttempts)
        {
            break;
        }
        Sleep(200);
    }

    if (!response.empty())
        std::cerr << "sendMove server response: " << response << std::endl;
    return false;
}

bool sendJoin(const std::string &room, const std::string &player, std::string &response)
{
    std::string request = "JOIN " + room + " " + player + "\n";
    return sendRequest(request, response);
}

std::string receiveMove(const std::string &room, const std::string &player)
{
    std::string request = "RECV " + room + " " + player + "\n";
    std::string response;
    if (!sendRequest(request, response))
        return std::string();
    return response;
}

int test_main_net()
{
    std::cout << "Room condivisa (es. room1): ";
    std::string room;
    std::getline(std::cin, room);

    std::cout << "ID client unico (es. A, B, player1, player2): ";
    std::string player;
    std::getline(std::cin, player);

    if (room.empty() || player.empty())
    {
        std::cout << "Room e ID client devono essere valorizzati." << std::endl;
        return 1;
    }

    std::string joinResponse;
    if (!sendJoin(room, player, joinResponse) || joinResponse.rfind("OK", 0) != 0)
    {
        std::cout << "Errore JOIN al server: " << joinResponse << std::endl;
        return 1;
    }

    std::cout << "Controllo se ci sono mosse in arrivo..." << std::endl;
    std::string incoming = receiveMove(room, player);
    if (!incoming.empty())
    {
        if (incoming == "NONE")
        {
            std::cout << "Nessuna mossa in arrivo al momento." << std::endl;
        }
        else if (incoming.rfind("ERROR", 0) == 0)
        {
            std::cout << "Errore RECV dal server: " << incoming << std::endl;
            return 1;
        }
        else
        {
            std::cout << "Mossa ricevuta dal server: " << incoming << std::endl;
        }
    }
    else
    {
        std::cout << "Errore comunicazione con il server." << std::endl;
        return 1;
    }

    std::cout << "Inserisci la mossa: ";
    std::string mossa;
    std::getline(std::cin, mossa);

    std::cout << "Invio mossa al server: " << mossa << std::endl;
    if (!sendMove(room, player, mossa))
    {
        std::cout << "Errore invio mossa al server." << std::endl;
        return 1;
    }

    std::cout << "Mossa inviata con successo." << std::endl;
    return 0;
}

std::string roomSuggested()
{
    std::string request = "SUGGEST\n";
    std::string response;
    if (!sendRequest(request, response))
        return std::string();
    return response;
}
