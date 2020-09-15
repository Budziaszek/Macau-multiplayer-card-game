#include "Player.h"

unsigned int Player::iid;
Player::Player(string givenName)
{
	name = givenName;
	id = iid;
	iid++;
}
unsigned int Player::getId()
{
	return id;
}
string Player::getName()
{
	return name;
}
void Player::setName(string givenName)
{
	name = givenName;
}
