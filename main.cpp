#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include "src/Cards/Deck.h"
#include "src/graphics/GUI.h"

using namespace std;

int main()
{
    srand(time(NULL));
    GUI client;
    if (client.welcomeScreen())
    {
        Server server;
        server.startListening();
        int numberOfPlayers;
        do {
            cout << "Podaj liczbe graczy (2-4): ";
            cin >> numberOfPlayers;
        } while (numberOfPlayers > 4 || numberOfPlayers < 2);
        server.waitForPlayers(numberOfPlayers);
        server.game();
    }
    return 0;
}


