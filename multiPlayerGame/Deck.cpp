#include "Deck.h"

Deck::Deck()
{
	lastBackInDeck = 52;
	firstInDeck = 0;
	cards = new Card[52];
	for (int i = 1; i < 5; i++)
		for (int j = 1; j < 14; j++)
			cards[(i-1) * 13 + (j-1)] = Card(i, j);
}
void Deck::show()
{
	cout << "DECK (color, figure)" << endl;
	for (int i = 0; i < 52; i++)
	{
		cout << cards[i].color << " " << cards[i].figure << endl;
	}
}
void Deck::swapRandomCards()
{
	unsigned int first = rand() % lastBackInDeck;
	unsigned int second;
	do {
		second = rand() % lastBackInDeck;
	} while (first == second);

	Card temp = cards[first];
	cards[first] = cards[second];
	cards[second] = temp;
}
void Deck::shuffle(unsigned int times)
{
	for (int i = 0; i < times; i++)
	{
		swapRandomCards();
	}
}
PlayerCards* Deck::deal(unsigned int numberOfPlayers, unsigned int numberOfCardsForEach)
{
	PlayerCards* playersCards = new PlayerCards[numberOfPlayers];
	for (int i = 0; i < numberOfCardsForEach; i++)
	{
		for (int j = 0; j < numberOfPlayers; j++)
		{
			playersCards[j].cards[i] = this->cards[firstInDeck];
			playersCards[j].numberOfCards++;		
			this->cards[firstInDeck].color = Card::noColor;
			this->cards[firstInDeck].figure = Card::noFigure;
			firstInDeck++;
		}
	}
	return playersCards;
}
Card Deck::giveOne()
{
	Card temp = cards[firstInDeck];
	this->cards[firstInDeck].color = Card::noColor;
	this->cards[firstInDeck].figure = Card::noFigure;
	firstInDeck++;
	if (firstInDeck == 52)
	{
		firstInDeck = 0;
		shuffle();
	}
	return temp;
}
void Deck::getOne(Card card)
{
	if (lastBackInDeck == 51)
		lastBackInDeck = 0;
	else
		lastBackInDeck++;

	this->cards[lastBackInDeck].color = card.color;
	this->cards[lastBackInDeck].figure = card.figure;
}

PlayerCards::PlayerCards()
{
	numberOfCards = 0;
	cards = new Card[52];
}
Card PlayerCards::checkOne(int which)
{
	return cards[which];
}
void PlayerCards::show()
{
	for (int i = 0; i < numberOfCards; i++)
	{
		cout << cards[i].color << " " << cards[i].figure << endl;
	}
}
void PlayerCards::draw(Deck &deck)
{
	cards[numberOfCards] = deck.giveOne();
	numberOfCards++;
}
void PlayerCards::draw(Card card)
{
	cards[numberOfCards] = card;
	numberOfCards++;
}
Card PlayerCards::discard()
{
	for (int i = 0; i < numberOfCards; i++)
	{
		if (cards[i].chosen == true)
		{
			Card thisOne = cards[i];
			for (; i < numberOfCards - 1; i++)
			{
				cards[i] = cards[i + 1];
			}
			numberOfCards--;
			cards[i].chosen = false;
			return thisOne;
		}
	}
	return Card();
}
void PlayerCards::discard(unsigned int which, Deck& deck)
{
	deck.getOne(cards[which]);
	cards[which].chosen = false;
	for (int i = which; i < numberOfCards - 1; i++)
	{
		cards[i] = cards[i + 1];
	}
	numberOfCards--;
}
void PlayerCards::discard(Card card, Deck& deck)
{
	deck.getOne(card);
	for (int i = 0; i < numberOfCards; i++)
	{
		if (cards[i].color == card.color)
			if (cards[i].figure == card.figure)
			{
				cards[i].chosen = true;
				discard();
				return;
			}
	}
}
void PlayerCards::changeChoose(int which)
{
	cards[which].chosen = !cards[which].chosen;
}
void PlayerCards::resetChoosen()
{
	for (int i = 0; i < numberOfCards; i++)
		cards[i].chosen = false;
}
