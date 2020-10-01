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

    Button sortByFigure;
    Button sortByColor;

    sf::Text requestInformation;
    sf::Text cardsToDraw;
    sf::Text turnsToLose;

    CardsImages images;

    sf::Font font;
    sf::Color backgroundColor;

    ServerConnection serverConnection;
    GameState gameState;

    int *jackOptions;
    int *aceOptions;

    sf::Text selectRequest;
    Button noRequest;

    void showCards();

    void showOtherPlayersCards();

    void showCardOnTable();

    void showInformation();

    void showGameButtons();

    void checkGameButtons(sf::Vector2f mousePosition);

    void checkCardSelectionChange(sf::Vector2f mousePosition);

    static float checkCardsInterval(unsigned int numberOfCards);

    void initializeRequestButtons(Button tab[], bool figure, int options[], int size);

    bool realizeRequest(const string &request, bool figure, int options[], int size);

    string enterInformation(string informationName, bool hidden = false);

    void playButtonClicked();

    void waitForPlayers();

    void windowInformation(const string& title, const string &info);

    static void centerText(sf::Text *text, float windowWidth, float windowHeight);

    sf::Vector2f getMousePosition(sf::RenderWindow *w);

    void updateAndDisplayInformation();
};


#endif //MACAU_GUI_H
