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
#include "../cards/CardsImages.h"
#include "../cards/PlayerCards.h"
#include "../connection/Server.h"

using namespace std;

class GameStatus;
class ServerConnection;

class GUI {
public:
    GUI();

    bool welcomeScreen();

    void play();

private:
    sf::RenderWindow *window;

    sf::Text makeMoveButton;
    sf::Text drawButton;
    sf::Text discardButton;
    sf::Text author;

    CardsImages images;

    sf::Font font;
    sf::Color backgroundColor;

    ServerConnection serverConnection;
    GameStatus gameStatus;

    void finishTurn();

    void showPlayersCards(sf::Vector2i basicCardsPosition, int cardsXChange);

    void showOtherPlayersCards();

    void showCardOnTable();

    void showStatement();

    void showButtons();

    int checkCardsInterval(int numberOfCards);

    bool realizeJackMove();

    bool realizeAceMove();

    string enterInformation(string informationName, bool hidden = false);

    void initializeButtons();
};


#endif //MACAU_GUI_H
