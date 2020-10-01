//
// Created by Magdalena on 18.09.2020.
//

#include "GUI.h"

GUI::GUI() {
    width = 600;
    height = 600;
    window = nullptr;

    backgroundColor = sf::Color(0, 102, 0, 255);

    if (!font.loadFromFile("../resources/font.ttf")) {
        windowInformation("ERROR", "Could not find required resource: font.ttf");
        window->close();
    }
    if (!images.load("../resources/cards.png", sf::Vector2u(71, 96))) {
        windowInformation("ERROR", "Could not find required resource: cards.png");
        window->close();
    }

    images.initializePositions((int) width, (int) height);

    playButton = Button("Play", font, 50);
    centerText(playButton.getText(), (float) width, (float) height);
    playButton.move(0, -25);

    serverButton = Button("Start server", font, 50);
    centerText(serverButton.getText(), (float) width, (float) height);
    serverButton.move(0, 25);

    discardButton = Button("Discard", font, 20);
    centerText(discardButton.getText(), (float) width, (float) height);
    discardButton.move(-100, 100);

    drawButton = Button("Draw", font, 20);
    centerText(drawButton.getText(), (float) width, (float) height);
    drawButton.move(0, 100);

    finishButton = Button("Finish", font, 20);
    centerText(finishButton.getText(), (float) width, (float) height);
    finishButton.move(100, 100);

    author = sf::Text("Magdalena Budziaszek", font, 15);
    sf::FloatRect textRect = author.getLocalBounds();
    author.setOrigin(textRect.left + textRect.width, textRect.top + textRect.height);
    author.setPosition(sf::Vector2f(width - 5, height - 5));
    author.setFillColor(sf::Color(96, 96, 96));
    author.setOutlineColor(sf::Color(32, 32, 32));

    jackOptions = new int[7]{Card::five, Card::six, Card::seven, Card::eight, Card::nine, Card::ten, Card::queen};
    aceOptions = new int[4]{Card::spade, Card::club, Card::heart, Card::diamond};

    selectRequest = sf::Text("Select request", font, 25);
    centerText(&selectRequest, 400, 200);
    selectRequest.setFillColor(sf::Color::White);
    selectRequest.move(sf::Vector2f(0, -80));

    noRequest = Button("No request", font, 20);
    centerText(noRequest.getText(), 400, 200);
    noRequest.getText()->move(sf::Vector2f(70, 70));

    requestInformation = sf::Text("Request: ", font, 15);
    requestInformation.setFillColor(sf::Color::White);
    requestInformation.setPosition(sf::Vector2f((float) width / 2 - 50, (float) height / 2 - 130));

    cardsToDraw = sf::Text("Cards to draw: ", font, 15);
    cardsToDraw.setFillColor(sf::Color::White);
    cardsToDraw.setPosition(sf::Vector2f((float) width / 2 - 50, (float) height / 2 - 110));

    turnsToLose = sf::Text("Turns to lose: ", font, 15);
    turnsToLose.setFillColor(sf::Color::White);
    turnsToLose.setPosition(sf::Vector2f((float) width / 2 - 50, (float) height / 2 - 90));
}


bool GUI::welcomeScreen() {
    window = new sf::RenderWindow(sf::VideoMode(width, height), "Macau", sf::Style::Close);

    sf::Vector2f mousePosition = sf::Vector2f();
    while (window->isOpen()) {
        if(gameState.finished()) {
            windowInformation("INFORMATION", "Game finished on place " + to_string(gameState.getPlace()));
            gameState.setPlace(-1);
        }
        window->clear(backgroundColor);
        window->draw(playButton);
        window->draw(serverButton);
        window->draw(author);
        window->display();
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
                return false;
            }
            mousePosition = getMousePosition(window);
            if (playButton.checkMouseEvent(mousePosition, event))
                playButtonClicked();
            else if (serverButton.checkMouseEvent(mousePosition, event)) {
                window->close();
                return true;
            }
        }
    }
    return false;
}

void GUI::playButtonClicked() {
    //FreeConsole();
    while (!serverConnection.connect(enterInformation("Enter server ip: "))) {
        if (window->isOpen())
            windowInformation("ERROR", "Could not connect with server! Try again.");
        else
            return;
    }
    waitForPlayers();
    play();
    serverConnection.disconnect();
    window->close();
}

void GUI::waitForPlayers() {
    sf::Text info = sf::Text("Waiting for others players to connect", font, 40);
    info.setFillColor(sf::Color::White);
    centerText(&info, (float) width, (float) height);

    while (window->isOpen() && !gameState.receivedStartPacket()) {
        window->clear(backgroundColor);
        window->draw(info);
        window->display();
        while (window->pollEvent(event))
            if (event.type == sf::Event::Closed)
                window->close();
        if (serverConnection.checkSelector())
            serverConnection.receive(&gameState);
    }
}

void GUI::checkGameButtons(sf::Vector2f mousePosition) {
    if (discardButton.checkMouseEvent(mousePosition, event)) {
        if (gameState.canDiscard(gameState.getSelectedCard()))
            serverConnection.discard(&gameState);
    } else if (drawButton.checkMouseEvent(mousePosition, event)) {
        if (gameState.canDraw())
            serverConnection.drawRequest(&gameState);
    } else if (finishButton.checkMouseEvent(mousePosition, event)) {
        if (gameState.canFinish()) {
            if (gameState.getCardOnTable().getFigure() == Card::jack)
                realizeRequest("Jack (figure request)", true, jackOptions, 7);
            if (gameState.getCardOnTable().getFigure() == Card::ace)
                realizeRequest("Ace (color request)", false, aceOptions, 4);
            serverConnection.finishTurn(&gameState);
        }
    }
}

void GUI::checkCardSelectionChange(sf::Vector2f mousePosition) {
    float selectedShift = 50;
    float step = checkCardsInterval(gameState.getNumberOfCards());

    int n = (int) gameState.getNumberOfCards();
    int c = (int) gameState.getNumberOfCards() / 2;

    float minX = (float) images.getXPosition(0) - (float) c * step;
    float maxX = (float) images.getXPosition(0) + (float) (n - c - 1) * step + images.getSize().x;

    if(gameState.getNumberOfCards() % 2 == 0) {
        minX += (float) images.getSize().x / 2;
        maxX += (float) images.getSize().x / 2;
    }

    float minY = images.getYPosition(0);
    float maxY = images.getYPosition(0) + images.getSize().y;

    if (mousePosition.x >= minX && mousePosition.x <= maxX) {
        int cardNumber = floor((mousePosition.x - minX) / step);
        cardNumber = cardNumber < n ? cardNumber : n - 1;
        float shift = 0;
        if (gameState.getCard(cardNumber).isSelected())
            shift = selectedShift;
        if (mousePosition.y >= minY - shift && mousePosition.y <= maxY - shift)
            gameState.getCards().changeCardSelection(cardNumber);
    }
}


void GUI::play() {
    sf::Vector2f mousePosition = sf::Vector2f();

    while (window->isOpen()) {
        window->clear(backgroundColor);
        window->draw(author);
        showCardOnTable();
        showCards();
        if (gameState.isTurn())
            showGameButtons();
        showOtherPlayersCards();
        updateAndDisplayInformation();
        window->display();
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();
            mousePosition = getMousePosition(window);
            if (gameState.isTurn()) {
                checkGameButtons(mousePosition);
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left)
                    checkCardSelectionChange(mousePosition);
            }
        }

        if (serverConnection.checkSelector())
            serverConnection.receive(&gameState);
    }
}

void GUI::showCards() {
    sf::Vector2f centerCardPosition = sf::Vector2f(images.getXPosition(0), images.getYPosition(0));
    float selectedShift = 50;
    float step = checkCardsInterval(gameState.getNumberOfCards());

    if(gameState.getNumberOfCards() % 2 == 0)
        centerCardPosition.x += (float) images.getSize().x / 2;
    int c = (int) gameState.getNumberOfCards() / 2;
    for (int i = 0; i < gameState.getNumberOfCards(); i++) {
        images.setCard(gameState.getCard(i));
        float x = centerCardPosition.x + step * (float) (i - c);
        if (gameState.getCard(i).isSelected() && gameState.isTurn())
            images.setPosition(sf::Vector2i(x, centerCardPosition.y - selectedShift));
        else
            images.setPosition(sf::Vector2i(x, centerCardPosition.y));
        window->draw(images);
    }
}

void GUI::showCardOnTable() {
    unsigned int row = gameState.getCardOnTable().getColor() - 1;
    images.setNumber(row * 13 + gameState.getCardOnTable().getFigure() - 1);
    images.setPosition(sf::Vector2i((float) width / 2.0f - (float) images.getSize().x / 2,
                                    (float) height / 2.0f - (float) images.getSize().y / 2));
    window->draw(images);
}

void GUI::showOtherPlayersCards() {
    sf::RectangleShape *cardBack = images.getCardBack();

    for (int i = 1; i < 4; i++) {
        float step = checkCardsInterval(gameState.getNumberOfOtherCards()[i - 1]);
        int c = (int) gameState.getNumberOfOtherCards()[i - 1] / 2;
        cardBack->setRotation(images.getRotation(i));
        for (int j = 0; j < gameState.getNumberOfOtherCards()[i - 1]; j++) {
            cardBack->setPosition(sf::Vector2f(
                    images.getXPosition(i) + (float) (j - c) * images.getXStep(i) * step,
                    images.getYPosition(i) + (float) (j - c) * images.getYStep(i) * step));
            window->draw(*cardBack);
        }
    }
}

void GUI::showInformation() {
}

void GUI::showGameButtons() {
    if (gameState.finished())
        return;
    window->draw(drawButton);
    window->draw(discardButton);
    window->draw(finishButton);
}

void GUI::centerText(sf::Text *text, float windowWidth, float windowHeight) {
    sf::FloatRect textRect = text->getLocalBounds();
    text->setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text->setPosition(sf::Vector2f(windowWidth / 2.0f, windowHeight / 2.0f));
}

void GUI::windowInformation(const string& title, const string &info) {
    auto *errorWindow = new sf::RenderWindow(sf::VideoMode(400, 200),
                                             title, sf::Style::Close);
    sf::Text text = sf::Text(info, font, 20);
    centerText(&text, 400, 200);
    text.setFillColor(sf::Color::Red);

    while (errorWindow->isOpen()) {
        errorWindow->clear(sf::Color::White);
        errorWindow->draw(text);
        errorWindow->display();
        while (errorWindow->pollEvent(event))
            if (event.type == sf::Event::Closed)
                errorWindow->close();
    }
}

void GUI::initializeRequestButtons(Button tab[], bool figure, int options[], int size) {
    for (int i = 0; i < size; i++) {
        if (figure)
            tab[i] = Button(Card::figureToString(options[i]), font, 20);
        else
            tab[i] = Button(Card::colorToString(options[i]), font, 20);
        centerText(tab[i].getText(), 400, 20);
        tab[i].move(-60, 45 + (float) i * 20);
    }
}

bool GUI::realizeRequest(const string &request, bool figure, int options[], int size) {
    auto *miniWindow = new sf::RenderWindow(sf::VideoMode(400, 200), request, sf::Style::Close);

    Button tab[size];
    initializeRequestButtons(tab, figure, options, size);

    while (miniWindow->isOpen()) {
        while (miniWindow->pollEvent(event)) {
            miniWindow->clear(backgroundColor);
            miniWindow->draw(selectRequest);
            for (int i = 0; i < size; i++)
                miniWindow->draw(tab[i]);
            miniWindow->draw(noRequest);
            miniWindow->display();

            if (event.type == sf::Event::Closed) {
                miniWindow->close();
                return false;
            }
            sf::Vector2f mousePosition = getMousePosition(miniWindow);
            if (noRequest.checkMouseEvent(mousePosition, event)) {
                miniWindow->close();
                return false;
            }
            for (int i = 0; i < size; i++)
                if (tab[i].checkMouseEvent(mousePosition, event)) {
                    if (figure)
                        gameState.setRequest(Card(Card::noColor, options[i]));
                    else
                        gameState.setRequest(Card(options[i], Card::noFigure));
                    miniWindow->close();
                    return true;
                }
        }
    }
    return false;
}

float GUI::checkCardsInterval(unsigned int numberOfCards) {
    float min_step = 50;
    float step = floor(250.f / (float) numberOfCards);
    return step > min_step ? min_step : step;
}

string GUI::enterInformation(string informationName, bool hidden) {
    string information;
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
                    if (!hidden)
                        informationName += static_cast<char>(event.text.unicode);
                    else
                        informationName += "*";
                }
            }
            if (event.type == sf::Event::Closed)
                window->close();
            if (event.key.code == sf::Keyboard::Return && information.length() > 0)
                return information;
        }
    }
    return "";
}

sf::Vector2f GUI::getMousePosition(sf::RenderWindow *w) {
    // This function was created because of some problems with getPosition(relativeTo).
    // Normally code below can be used instead:
    // mousePosiion = sf::Vector2f(sf::Mouse::getPosition(*window));

    return sf::Vector2f(sf::Mouse::getPosition().x - w->getPosition().x,
                        sf::Mouse::getPosition().y - w->getPosition().y - ::GetSystemMetrics(SM_CYCAPTION));

}

void GUI::updateAndDisplayInformation() {
    if (gameState.isThereFigureRequest())
        requestInformation.setString("Request: " + Card::figureToString((int) gameState.getRequest().getFigure()));
    else if (gameState.isThereColorRequest())
        requestInformation.setString("Request: " + Card::colorToString((int) gameState.getRequest().getColor()));
    else
        requestInformation.setString(string("Request: ") + string("-"));

    cardsToDraw.setString("Cards to draw: "
                          + (gameState.getBonus() > 0 ? to_string(gameState.getBonus()) : to_string(1)));


    if (!gameState.isWaiting() || gameState.finished())
        turnsToLose.setString("Turns to lose: "
                              + (gameState.isFourRequired() ? to_string(gameState.getTurnsToLose()) : to_string(0)));
    else
        turnsToLose.setString("Turns to lose (waiting): " + to_string(gameState.getWaiting()));


    window->draw(requestInformation);
    window->draw(cardsToDraw);
    window->draw(turnsToLose);
}

