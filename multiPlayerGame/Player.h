#ifndef __PLAYER_h
#define __PLAYER_h

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Deck.h"
using namespace std;

class Player
{
public:
	Player(string givenName = "UNKNOWN");
	unsigned int getId();
	string getName();
	void setName(string givenName);
private:
	string name;
	unsigned int id;
	static unsigned int iid;
};

#endif
