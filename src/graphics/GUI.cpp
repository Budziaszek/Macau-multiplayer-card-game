//
// Created by Magdalena on 18.09.2020.
//

#include "GUI.h"

GUI::GUI() {
    width = 600;
    height = 600;
    window = nullptr;
    event = sf::Event();

    backgroundColor = sf::Color(0, 102, 0, 255);

    if (!font.loadFromFile("../resources/font.ttf")) {
        error("Could not find required resource: font.ttf");
        window->close();
    }
    if (!images.load("../resources/cards.png", sf::Vector2u(71, 96))) {
        error("Could not find required resource: cards.png");
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
}

bool GUI::welcomeScreen() {
    window = new sf::RenderWindow(sf::VideoMode(width, height), "Macau", sf::Style::Close);

    sf::Vector2f mousePosition = sf::Vector2f();
    while (window->isOpen()) {
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
            mousePosition = sf::Vector2f(sf::Mouse::getPosition(*window));
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
            error("Could not connect with server! Try again.");
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

    } else if (drawButton.checkMouseEvent(mousePosition, event)) {

    } else if (finishButton.checkMouseEvent(mousePosition, event)) {

    }
}

void GUI::checkCardSelectionChange(sf::Vector2f mousePosition) {
    sf::Vector2f centerCardPosition = sf::Vector2f(images.getXPosition(0), images.getYPosition(0));
    float selectedShift = 50;
    float step = checkCardsInterval(gameState.getNumberOfCards());

    int n = (int) gameState.getNumberOfCards();
    int c = (int) gameState.getNumberOfCards() / 2;

    float minX = (float) images.getXPosition(0) - (float) c * step;
    float maxX = (float) images.getXPosition(0) + (float) (n - c  - 1) * step + images.getSize().x;

    float minY = images.getYPosition(0);
    float maxY = images.getYPosition(0) + images.getSize().y;

    if (mousePosition.x >= minX && mousePosition.x <= maxX) {
        int cardNumber = floor((mousePosition.x - minX) / step);
        cardNumber = cardNumber < n ? cardNumber : n - 1;
        float shift = 0;
        if(gameState.getCard(cardNumber).isSelected())
            shift = selectedShift;
        if (mousePosition.y >= minY - shift  && mousePosition.y <= maxY - shift )
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
        window->display();

        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();
            mousePosition = sf::Vector2f(sf::Mouse::getPosition(*window));
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

void GUI::showStatement() {
}

void GUI::showGameButtons() {
    if (gameState.finished())
        return;
    window->draw(drawButton);
    window->draw(discardButton);
    window->draw(finishButton);
}

void GUI::finishTurn() {

}

void GUI::centerText(sf::Text *text, float windowWidth, float windowHeight) {
    sf::FloatRect textRect = text->getLocalBounds();
    text->setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text->setPosition(sf::Vector2f(windowWidth / 2.0f, windowHeight / 2.0f));
}

void GUI::error(const string &info) {
    auto *errorWindow = new sf::RenderWindow(sf::VideoMode(400, 200),
                                             "ERROR", sf::Style::Close);
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

bool GUI::realizeJackMove() {

}

bool GUI::realizeAceMove() {

}

float GUI::checkCardsInterval(unsigned int numberOfCards) {
    return floor(250.f / (float) numberOfCards);
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

