//
// Created by Magdalena on 18.09.2020.
//

#ifndef MACAU_GUI_H
#define MACAU_GUI_H

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include "../cards/Card.h"
#include "CardsImages.h"
#include "../cards/PlayerCards.h"
#include "../connection/Server.h"
#include "Button.h"

using namespace std;

class GameState;

class ServerConnection;

class GUI {
public:
    GUI();

    bool welcomeScreen();

    void play();

private:
    unsigned int width;
    unsigned int height;
    sf::RenderWindow *window;
    sf::Event event;

    Button playButton;
    Button serverButton;
    Button finishButton;
    Button drawButton;
    Button discardButton;
    sf::Text author;

    CardsImages images;

    sf::Font font;
    sf::Color backgroundColor;

    ServerConnection serverConnection;
    GameState gameState;

    void finishTurn();

    void showCards();

    void showOtherPlayersCards();

    void showCardOnTable();

    void showStatement();

    void showGameButtons();

    void checkGameButtons(sf::Vector2f mousePosition);

    void checkCardSelectionChange(sf::Vector2f mousePosition);

    static float checkCardsInterval(unsigned int numberOfCards);

    bool realizeJackMove();

    bool realizeAceMove();

    string enterInformation(string informationName, bool hidden = false);

    void playButtonClicked();

    void waitForPlayers();

    void error(const string &info);

    static void centerText(sf::Text *text, float windowWidth, float windowHeight);
};


#endif //MACAU_GUI_H
