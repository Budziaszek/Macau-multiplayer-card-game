#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "src/graphics/GUI.h"

using namespace std;

int main()
{
    GUI client;
    if (client.welcomeScreen())
    {
        Server server;
        server.start();

    }
    return 0;
}



