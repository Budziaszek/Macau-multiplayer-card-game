//
// Created by Magdalena on 18.09.2020.
//

#include "GUI.h"

GUI::GUI() {
    backgroundColor = sf::Color(0, 102, 0, 255);
    font.loadFromFile("../resources/font.ttf");
}

bool GUI::welcomeScreen() {
    window = new sf::RenderWindow(sf::VideoMode(800, 600), "Makao", sf::Style::Close);

    author.setFont(font);
    author.setCharacterSize(15);
    author.setFillColor(sf::Color(96, 96, 96));
    author.setOutlineColor(sf::Color(32, 32, 32));
    author.setPosition(650, 580);
    author.setString("Magdalena Budziaszek");

    sf::Event event;
    sf::Text text;
    text.setString("Graj");
    text.setFillColor(sf::Color::White);
    text.setFont(font);
    text.setCharacterSize(50);
    text.setPosition(340, 250);

    while (window->isOpen()) {
        while (window->pollEvent(event)) {
            if (text.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
                text.setFillColor(sf::Color::Yellow);
            else
                text.setFillColor(sf::Color::White);

            window->clear(backgroundColor);
            window->draw(text);
            window->draw(author);
            window->display();

            if (event.type == sf::Event::Closed)
                window->close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                if (text.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window)))) {
                    cout << "GRAJ" << endl;
                    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                        FreeConsole();
                    while (!serverConnection.connect(enterInformation("Wpisz ip serwera: ")));
                    cout << "Rozpoczecie gry" << endl;
                    window->clear(backgroundColor);
                    text.setString("Oczekiwanie na pozostalych graczy");
                    text.setCharacterSize(40);
                    text.setFillColor(sf::Color::White);
                    text.setPosition(100, 250);
                    window->draw(text);
                    window->display();
                    play();
                    window->close();
                }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
                sf::Keyboard::isKeyPressed(sf::Keyboard::Right))//server
            {
                window->close();
                return true;
            }
        }
    }
    return false;
}

void GUI::play() {
    if(!images.load("../resources/cards.png", sf::Vector2u(71, 96)))
        return;
    sf::Vector2i basicCardsPosition = sf::Vector2i(110, 450);

    initializeButtons();

    while (!serverConnection.receiveStartPacket(&gameStatus));

    sf::Event event;
    while (window->isOpen()) {
        window->clear(backgroundColor);
        int cardsXChange = checkCardsInterval(gameStatus.getNumberOfCards());
        showPlayersCards(basicCardsPosition, cardsXChange);
        showStatement();
        if (gameStatus.isItsMyTurn())
            showButtons();
        showCardOnTable();
        showOtherPlayersCards();
        window->draw(author);
        window->display();

        while (window->pollEvent(event)) {
            if (gameStatus.isItsMyTurn()) {
                if (event.type == sf::Event::MouseButtonPressed &&
                    event.mouseButton.button == sf::Mouse::Button::Left) {
                    if (sf::Mouse::getPosition(*window).x > basicCardsPosition.x && sf::Mouse::getPosition(*window).x <
                                                                                    (basicCardsPosition.x +
                                                                                     gameStatus.getNumberOfCards() *
                                                                                     cardsXChange)) {
                        if (sf::Mouse::getPosition(*window).y > basicCardsPosition.y &&
                            sf::Mouse::getPosition(*window).y < basicCardsPosition.y + 96) {
                            int whichCard = (sf::Mouse::getPosition(*window).x - basicCardsPosition.x) / cardsXChange;
                            if (gameStatus.getBonus() == Server::Jack) {
                                if (gameStatus.getCard(whichCard).getFigure() == gameStatus.getFigureRequest() ||
                                        gameStatus.getCard(whichCard).getFigure() == Card::Jack)//tylko do figury lub walet
                                    gameStatus.getCard(whichCard);
                            } else if (gameStatus.getBonus() == Server::Four) {
                                if (gameStatus.getCard(whichCard).getFigure() == 4 && !gameStatus.isWaiting())
                                    gameStatus.getCard(whichCard);
                            } else if (gameStatus.getBonus() == Server::Ace) {
                                if (gameStatus.getCard(whichCard).getColor() == gameStatus.getColorRequest() ||
                                        gameStatus.getCard(whichCard).getFigure() == Card::Ace) //tylko zadany kolor lub as
                                    gameStatus.getCard(whichCard);
                            } else if (!gameStatus.isWaiting()) {
                                if (gameStatus.getCard(whichCard).getColor() == gameStatus.getCardOnTable().getColor() ||
                                        gameStatus.getCard(whichCard).getFigure() ==
                                            gameStatus.getCardOnTable().getFigure())//tylko do koloru lub figury
                                    if (gameStatus.getFigureOfAlreadyThrown() == 0
                                    || gameStatus.getFigureOfAlreadyThrown() == gameStatus.getCard(whichCard).getFigure())
                                        //w jednym ruchu tylko te same figury
                                        if (gameStatus.getBonus() <= 0 || (gameStatus.getCard(whichCard).getFigure() == 2 ||
                                                gameStatus.getCard(whichCard).getFigure() == 3 ||
                                                           (gameStatus.getCard(whichCard).getFigure() == Card::King &&
                                                            (gameStatus.getCard(whichCard).getColor() == Card::heart ||
                                                                    gameStatus.getCard(whichCard).getColor() ==
                                                             Card::spade)))) //waleczna na waleczna
                                            gameStatus.getCard(whichCard);
                            }
                        }
                    }

                    if (drawButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))) &&
                        gameStatus.isSomethingWasThrown() == false && gameStatus.getBonus() != Server::Four && !gameStatus.isWaiting()) {
                        serverConnection.drawCards(&gameStatus);
                        gameStatus.setBonus(0);
                        finishTurn();
                    }
                    if (makeMoveButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))) &&
                        (gameStatus.isSomethingWasDone() || gameStatus.getBonus() == Server::Four || gameStatus.isWaiting())) {
                        if (!gameStatus.isFourWasThrown() && gameStatus.getTurnsToLose() > 0)
                            gameStatus.setWaiting(true);
                        else {
                            if (gameStatus.isJackWasThrown())
                                if (realizeJackMove())
                                    gameStatus.setBonus(Server::Jack);
                            if (gameStatus.isAceWasThrown())
                                if (realizeAceMove())
                                    gameStatus.setBonus(Server::Ace);
                        }
                        finishTurn();

                    }
                    if (discardButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))) &&
                        !gameStatus.isWaiting())
                        serverConnection.discard(gameStatus.discardMove());

                }
            }
            if (event.type == sf::Event::Closed)
                window->close();
            if (!gameStatus.isItsMyTurn())
                serverConnection.checkTurn(&gameStatus);
        }
    }
    serverConnection.disconnect();
}

void GUI::showPlayersCards(sf::Vector2i basicCardsPosition, int cardsXChange) {
    images.setPosition(sf::Vector2i(basicCardsPosition.x, basicCardsPosition.y)); //delete if works without
    int cardsYChange = checkCardsInterval(gameStatus.getNumberOfCards());

    for (int i = 0; i < gameStatus.getNumberOfCards(); i++) {
        images.setNumber((gameStatus.getCard(i).getColor() - 1) * 13 + gameStatus.getCard(i).getFigure() - 1);
        if (gameStatus.getCard(i).chosen == true && gameStatus.isItsMyTurn()) {
            images.setPosition(
                    sf::Vector2i(basicCardsPosition.x + i * cardsXChange, basicCardsPosition.y - cardsYChange));
        } else
            images.setPosition(sf::Vector2i(basicCardsPosition.x + i * cardsXChange, basicCardsPosition.y));
        window->draw(images);
    }
}

void GUI::showCardOnTable() {
    images.setNumber((gameStatus.getCardOnTable().getColor() - 1) * 13 + gameStatus.getCardOnTable().getFigure() - 1);
    images.setPosition(sf::Vector2i(400 - 35, 300 - 48));
    window->draw(images);
}

void GUI::showOtherPlayersCards() {
    sf::RectangleShape cardDown(sf::Vector2f(71, 96));
    cardDown.setFillColor(sf::Color(153, 0, 0, 255));
    cardDown.setOutlineThickness(-1);
    cardDown.setOutlineColor(sf::Color::Black);

    int cardsXChange = checkCardsInterval(gameStatus.getNumberOfOtherCards()[0]);

    cardDown.setRotation(90);
    for (int i = 0; i < gameStatus.getNumberOfOtherCards()[0]; i++)//wyswietl karty gracza po lewej
    {
        cardDown.setPosition(sf::Vector2f(100, 50 + i * cardsXChange));
        window->draw(cardDown);
    }

    cardsXChange = checkCardsInterval(gameStatus.getNumberOfOtherCards()[1]);

    cardDown.setRotation(0);
    for (int i = 0; i < gameStatus.getNumberOfOtherCards()[1]; i++)//wyswietl karty gracza u gory
    {
        cardDown.setPosition(sf::Vector2f(150 + i * cardsXChange, 50));
        window->draw(cardDown);
    }

    cardsXChange = checkCardsInterval(gameStatus.getNumberOfOtherCards()[2]);

    cardDown.setRotation(90);
    for (int i = 0; i < gameStatus.getNumberOfOtherCards()[2]; i++)//wyswietl karty z prawej
    {
        cardDown.setPosition(sf::Vector2f(800 - 50, 600 - 150 - i * cardsXChange));
        window->draw(cardDown);
    }

}

void GUI::showStatement() {
    sf::Text bonusToTake;
    bonusToTake.setFont(font);
    bonusToTake.setCharacterSize(15);
    bonusToTake.setPosition(200, 400);

    string communicatToTake = "Kart do wziecia: ";
    string communicatRequest = "Zadanie: ";
    string communicatWait = "Kolejki do odstania: ";
    string communicatWin = "Gratulacje. Zajales miejsce ";
    string communicatLose = "Przegrales! Zajales miejsce ";

    if (gameStatus.getBonus() > 0) {
        bonusToTake.setString(communicatToTake + std::to_string(gameStatus.getBonus()));
        window->draw(bonusToTake);
    }
    if (gameStatus.getBonus() == Server::Jack) {
        if (gameStatus.getFigureRequest() == Card::Queen)
            bonusToTake.setString(communicatRequest + "Q");
        else
            bonusToTake.setString(communicatRequest + std::to_string(gameStatus.getFigureRequest()));
        window->draw(bonusToTake);
    }
    if (gameStatus.getBonus() == Server::Ace) {
        string colorStringRequest;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
            if (gameStatus.getColorRequest() == 1)
                colorStringRequest = "trefl (koniczyna)";
            if (gameStatus.getColorRequest() == 2)
                colorStringRequest = "kier (serce)";
            if (gameStatus.getColorRequest() == 3)
                colorStringRequest = "pik (lisc)";
            if (gameStatus.getColorRequest() == 4)
                colorStringRequest = "karo (diament)";
        } else {
            if (gameStatus.getColorRequest() == 1)
                colorStringRequest = "trefl";
            if (gameStatus.getColorRequest() == 2)
                colorStringRequest = "kier";
            if (gameStatus.getColorRequest() == 3)
                colorStringRequest = "pik";
            if (gameStatus.getColorRequest() == 4)
                colorStringRequest = "karo";
        }

        bonusToTake.setString(communicatRequest + colorStringRequest);
        window->draw(bonusToTake);
    }
    if (gameStatus.getTurnsToLose() > 0) {
        bonusToTake.setString(communicatWait + std::to_string(gameStatus.getTurnsToLose()));
        window->draw(bonusToTake);
    }
    if (gameStatus.isWaitPermanently()) {
        if (gameStatus.getNumberOfCards() > 0)
            bonusToTake.setString(communicatLose + std::to_string(gameStatus.getMyPlace()));
        else
            bonusToTake.setString(communicatWin + std::to_string(gameStatus.getMyPlace()));
        window->draw(bonusToTake);
    }
}

void GUI::showButtons() {
    if (gameStatus.isWaitPermanently())
        return;
    if (drawButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
        drawButton.setFillColor(sf::Color::Yellow);
    else
        drawButton.setFillColor(sf::Color::White);

    if (makeMoveButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
        makeMoveButton.setFillColor(sf::Color::Yellow);
    else
        makeMoveButton.setFillColor(sf::Color::White);

    if (discardButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
        discardButton.setFillColor(sf::Color::Yellow);
    else
        discardButton.setFillColor(sf::Color::White);

    if (gameStatus.isSomethingWasDone() || gameStatus.isWaiting() || gameStatus.getBonus() == Server::Four)
        window->draw(makeMoveButton);
    if (!gameStatus.isWaiting())
        window->draw(discardButton);
    if (!gameStatus.isSomethingWasThrown() && gameStatus.getBonus() != Server::Four && !gameStatus.isWaiting())
        window->draw(drawButton);
}

void GUI::finishTurn() {
    if (!gameStatus.isWaiting()) {

        if (gameStatus.getNumberOfCards() == 0) {
            gameStatus.setWaitPermanently(true);
            serverConnection.victory(&gameStatus);
        }

        serverConnection.finishTurn(&gameStatus);

        if (gameStatus.getBonus() == Server::Four && gameStatus.isFourWasThrown())
            gameStatus.setTurnsToLose(0);
        gameStatus.reset();
        gameStatus.resetChoosenCard();
        cout << "Zakonczono kolejke" << endl;
    } else {
        if (gameStatus.getBonus() == Server::wait) {
            serverConnection.loseTurn();
            gameStatus.setItsMyTurn(false);
            gameStatus.setSomethingWasDone(false);
            cout << "Kolejka zakonczona (utrata) - kolejna" << endl;
        } else {
            serverConnection.finish();
            gameStatus.setBonus(Server::wait);
            gameStatus.setItsMyTurn(false);
            gameStatus.setSomethingWasDone(true);
            cout << "Kolejka zakonczona (utrata) - pierwsza" << endl;
        }
        gameStatus.setTurnsToLose(gameStatus.getTurnsToLose() - 1);
        if (gameStatus.getTurnsToLose() == 0)
            gameStatus.setWaiting(false);
    }
}

bool GUI::realizeJackMove() {
    sf::RenderWindow *miniWindow = new sf::RenderWindow(sf::VideoMode(200, 200), "Zadanie (Walet)", sf::Style::Close);

    sf::Event event;
    sf::Text tak, nie;
    tak.setString("Tak");
    tak.setFillColor(sf::Color::White);
    tak.setFont(font);
    tak.setCharacterSize(20);
    tak.setPosition(50, 100);

    nie.setString("Nie");
    nie.setFillColor(sf::Color::White);
    nie.setFont(font);
    nie.setCharacterSize(20);
    nie.setPosition(150, 100);

    sf::Text information;
    information.setString("Czy chcesz zazadac figury?");
    information.setFillColor(sf::Color::White);
    information.setFont(font);
    information.setCharacterSize(15);
    information.setPosition(10, 20);

    while (miniWindow->isOpen()) {
        while (miniWindow->pollEvent(event)) {
            if (tak.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*miniWindow))))
                tak.setFillColor(sf::Color::Yellow);
            else
                tak.setFillColor(sf::Color::White);

            if (nie.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*miniWindow))))
                nie.setFillColor(sf::Color::Yellow);
            else
                nie.setFillColor(sf::Color::White);

            miniWindow->clear(backgroundColor);
            miniWindow->draw(tak);
            miniWindow->draw(nie);
            miniWindow->draw(information);
            miniWindow->display();

            if (event.type == sf::Event::Closed) {
                miniWindow->close();
                return false;
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                if (nie.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*miniWindow)))) {
                    miniWindow->close();
                    return false;
                }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                if (tak.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*miniWindow)))) {

                    sf::Text tab[7];
                    tab[0].setString("5");
                    tab[1].setString("6");
                    tab[2].setString("7");
                    tab[3].setString("8");
                    tab[4].setString("9");
                    tab[5].setString("10");
                    tab[6].setString("Q");

                    for (int i = 0; i < 7; i++) {
                        tab[i].setFillColor(sf::Color::White);
                        tab[i].setFont(font);
                        tab[i].setCharacterSize(20);
                        tab[i].setPosition(50, 20 + i * 15);
                    }

                    while (miniWindow->isOpen()) {
                        while (miniWindow->pollEvent(event)) {
                            for (int i = 0; i < 7; i++) {

                                if (tab[i].getGlobalBounds().contains(
                                        sf::Vector2f(sf::Mouse::getPosition(*miniWindow))))
                                    tab[i].setFillColor(sf::Color::Yellow);
                                else
                                    tab[i].setFillColor(sf::Color::White);
                            }
                            for (int i = 0; i < 7; i++) {
                                if (event.type == sf::Event::MouseButtonPressed &&
                                    event.mouseButton.button == sf::Mouse::Left)
                                    if (tab[i].getGlobalBounds().contains(
                                            sf::Vector2f(sf::Mouse::getPosition(*miniWindow)))) {
                                        if (i == 6)
                                            gameStatus.setFigureRequest(Card::Queen);
                                        else
                                            gameStatus.setFigureRequest(i + 5);
                                        miniWindow->close();
                                    }
                            }

                            miniWindow->clear(backgroundColor);
                            for (int i = 0; i < 7; i++)
                                miniWindow->draw(tab[i]);
                            miniWindow->display();
                        }
                    }

                    return true;
                }
        }
    }
    return false;
}

bool GUI::realizeAceMove() {
    sf::RenderWindow *miniWindow = new sf::RenderWindow(sf::VideoMode(200, 200), "Zadanie (As)", sf::Style::Close);

    sf::Event event;
    sf::Text tak, nie;
    tak.setString("Tak");
    tak.setFillColor(sf::Color::White);
    tak.setFont(font);
    tak.setCharacterSize(20);
    tak.setPosition(50, 100);

    nie.setString("Nie");
    nie.setFillColor(sf::Color::White);
    nie.setFont(font);
    nie.setCharacterSize(20);
    nie.setPosition(150, 100);

    sf::Text information;
    information.setString("Czy chcesz zazadac koloru?");
    information.setFillColor(sf::Color::White);
    information.setFont(font);
    information.setCharacterSize(15);
    information.setPosition(10, 20);

    while (miniWindow->isOpen()) {
        while (miniWindow->pollEvent(event)) {
            if (tak.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*miniWindow))))
                tak.setFillColor(sf::Color::Yellow);
            else
                tak.setFillColor(sf::Color::White);

            if (nie.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*miniWindow))))
                nie.setFillColor(sf::Color::Yellow);
            else
                nie.setFillColor(sf::Color::White);

            miniWindow->clear(backgroundColor);
            miniWindow->draw(tak);
            miniWindow->draw(nie);
            miniWindow->draw(information);
            miniWindow->display();

            if (event.type == sf::Event::Closed) {
                miniWindow->close();
                return false;
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                if (nie.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*miniWindow)))) {
                    miniWindow->close();
                    return false;
                }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                if (tak.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*miniWindow)))) {

                    sf::Text tab[4];
                    tab[0].setString("trefl");
                    tab[1].setString("kier ");
                    tab[2].setString("pik ");
                    tab[3].setString("karo");

                    sf::Text tabBig[4];
                    tabBig[0].setString("trefl (koniczyna)");
                    tabBig[1].setString("kier (serce)");
                    tabBig[2].setString("pik (lisc)");
                    tabBig[3].setString("karo (diament)");

                    for (int i = 0; i < 4; i++) {
                        tab[i].setFillColor(sf::Color::White);
                        tab[i].setFont(font);
                        tab[i].setCharacterSize(20);
                        tab[i].setPosition(45, 30 + i * 15);

                        tabBig[i].setFillColor(sf::Color::White);
                        tabBig[i].setFont(font);
                        tabBig[i].setCharacterSize(20);
                        tabBig[i].setPosition(45, 30 + i * 15);
                    }

                    while (miniWindow->isOpen()) {
                        while (miniWindow->pollEvent(event)) {
                            for (int i = 0; i < 4; i++) {

                                if (tab[i].getGlobalBounds().contains(
                                        sf::Vector2f(sf::Mouse::getPosition(*miniWindow)))) {
                                    tab[i].setFillColor(sf::Color::Yellow);
                                    tabBig[i].setFillColor(sf::Color::Yellow);
                                } else {
                                    tab[i].setFillColor(sf::Color::White);
                                    tabBig[i].setFillColor(sf::Color::Yellow);
                                }
                            }
                            for (int i = 0; i < 4; i++) {
                                if (event.type == sf::Event::MouseButtonPressed &&
                                    event.mouseButton.button == sf::Mouse::Left)
                                    if (tab[i].getGlobalBounds().contains(
                                            sf::Vector2f(sf::Mouse::getPosition(*miniWindow)))) {
                                        gameStatus.setColorRequest(i + 1);
                                        miniWindow->close();
                                    }
                            }

                            miniWindow->clear(backgroundColor);
                            for (int i = 0; i < 4; i++) {
                                if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
                                    miniWindow->draw(tabBig[i]);
                                else
                                    miniWindow->draw(tab[i]);
                            }
                            miniWindow->display();
                        }
                    }

                    return true;
                }
        }
    }
    return false;
}

int GUI::checkCardsInterval(int numberOfCards) {
    if (numberOfCards <= 8)
        return 50;
    if (numberOfCards < 16)
        return 30;
    if (numberOfCards < 24)
        return 20;
    if (numberOfCards < 32)
        return 10;
    else
        return 5;
}

void GUI::initializeButtons() {
    discardButton.setFont(font);
    discardButton.setString("Rzuc");
    discardButton.setCharacterSize(20);
    discardButton.setPosition(130, 550);

    drawButton.setFont(font);
    drawButton.setString("Ciagnij");
    drawButton.setCharacterSize(20);
    drawButton.setPosition(250, 550);

    makeMoveButton.setFont(font);
    makeMoveButton.setString("Zakoncz ruch");
    makeMoveButton.setCharacterSize(20);
    makeMoveButton.setPosition(370, 550);
}

string GUI::enterInformation(string informationName, bool hidden) {
    std::string information = "";
    sf::Event event;
    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(30);
    text.setPosition(50, 100);

    while (window->isOpen()) {
        while (window->pollEvent(event)) {
            text.setString(informationName);
            window->clear(backgroundColor);
            window->draw(text);
            window->draw(author);
            window->display();

            if (event.type == sf::Event::TextEntered && event.key.code != sf::Keyboard::Return) {
                if (event.text.unicode == '\b' && information.length() > 0) {
                    information.erase(information.size() - 1, 1);
                    informationName.erase(informationName.size() - 1, 1);
                } else if (event.key.code != sf::Keyboard::Return) {
                    information += static_cast<char>(event.text.unicode);
                    if (hidden == false)
                        informationName += static_cast<char>(event.text.unicode);
                    else
                        informationName += "*";
                }
            }
            if (event.type == sf::Event::Closed) {
                window->close();
                return "";
            }

            if (event.key.code == sf::Keyboard::Return && information.length() > 0)
                return information;
        }
    }
    return "";
}

