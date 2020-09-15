#ifndef __DECK_h
#define __DECK_h
#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

class CardsImages : public sf::Drawable, public sf::Transformable
{
public:
	bool load(const std::string& tileset, sf::Vector2u tileSize)
	{
		if (!Tileset.loadFromFile(tileset))
			return false;

		vertex = new sf::VertexArray[52];
		size = tileSize;

		for (unsigned int tileNumber = 0; tileNumber < 52; tileNumber++)
		{		
			vertex[tileNumber].setPrimitiveType(sf::Quads);
			vertex[tileNumber].resize(4);
			sf::Vertex* quad = &(vertex[tileNumber][0]);

			int n = tileNumber % (Tileset.getSize().x / tileSize.x);
			int m = tileNumber / (Tileset.getSize().x / tileSize.x);

			quad[0].texCoords = sf::Vector2f(n * tileSize.x, m * tileSize.y);
			quad[1].texCoords = sf::Vector2f((n + 1) * tileSize.x, m * tileSize.y);
			quad[2].texCoords = sf::Vector2f((n + 1) * tileSize.x, (m + 1) * tileSize.y);
			quad[3].texCoords = sf::Vector2f(n * tileSize.x, (m + 1) * tileSize.y);

			quad[0].position = sf::Vector2f(0, 0);
			quad[1].position = sf::Vector2f(tileSize.x, 0);
			quad[2].position = sf::Vector2f(tileSize.x, tileSize.y);
			quad[3].position = sf::Vector2f(0, tileSize.y);
		}
		return true;
	}
	void setNumber(int cardNumber)
	{
		number = cardNumber;
	}
	void setPosition(sf::Vector2i point)
	{
		sf::Vertex* quad = &(vertex[number][0]);

		quad[0].position = sf::Vector2f(point.x, point.y);
		quad[1].position = sf::Vector2f(point.x + size.x, point.y);
		quad[2].position = sf::Vector2f(point.x + size.x, point.y + size.y);
		quad[3].position = sf::Vector2f(point.x, point.y + size.y);
	}
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		states.texture = &Tileset;
		target.draw(vertex[number], states);
	}
	int number;
	sf::Vector2u size;
	sf::VertexArray* vertex;
	sf::Texture Tileset;
};

class Card
{		
	friend class Deck;
	friend class PlayerCards;
public:
	bool chosen;
	enum colors
	{
		noColor = 0,
		club = 1, //trefl
		heart, //kier
		spade, //pik
		diamond //karo
	};
	enum figures
	{
		noFigure = 0,
		Ace = 1, 
		Two,
		Three,
		Four,
		Five,
		Six,
		Seven,
		Eight,
		Nine, 
		Ten,
		Jack,
		Queen,
		King, 
	};

private:
	unsigned int color;
	unsigned int figure;
	

	
public:
	Card(unsigned int givenColour = noColor, unsigned int givenFigure = noFigure);
	int getColor();
	int getFigure();
};

class Deck
{
public:
	Deck();
	void show();
	void shuffle(unsigned int times = 100);
	PlayerCards* deal(unsigned int numberOfPlayers, unsigned int numberOfCardsForEach);
	Card giveOne();
	void getOne(Card card);
private:
	Card* cards;
	unsigned int firstInDeck;
	unsigned int lastBackInDeck;
	void swapRandomCards();
};

class PlayerCards
{
	friend class Deck;
public:
	PlayerCards();
	Card checkOne(int which);	
	void changeChoose(int which);
	void resetChoosen();
	unsigned int numberOfCards;	
	void show();
	void draw(Deck &deck);
	void draw(Card card);
	Card discard();
	void discard(unsigned int which, Deck& deck);
	void discard(Card card, Deck& deck);
private:
	Card* cards;

};


#endif