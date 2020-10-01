# Macau-multiplayer-card-game
This is multiplayer card game for 2-4 players. <br>
Tools and software used: Python, Pycharm, wxPython, pygame. <br>

## Rules and gameplay 
Rules are similar to rules descriped in <a href="https://en.wikipedia.org/wiki/Macau_(card_game)">Wikipedia</a>, but there are some differences.
In this implementation it is not possible for player to not to follow the rules. <br>
* Players (up to 4) are dealt 5 cards each. The first card in the discard pile is selected.
* The winner of the game is the first player to have no cards; in instances of more than 2 players, gameplay continues until only one player is left with cards.
* The next card played must be of the same suit or the same value as the card on the top of the discard pile. 
* If the player cannot play a card, he must draw from the deck.
* 2, 3, King of Spades and King od Hearts are brave action cards. If one of them is played, the next player in the sequence must pick up some cards (respectively 2, 3, 5, 5) unless they have a brave card, in which case they can add this to the original number of card to pick and the next player in the sequence must pick up all cards and so on.
* If a 4 is played, the next player in the sequence must miss a go, unless they have a 4, in which case they can add this to the original 4 and the next player in sequence miss 2 goes.
* If a Jack is played, the player placing the Jack can call for a non-action card value. The player in sequence must either play the card value called or place another Jack down and call a different value. The call ends when player, who started call places requested card.
* If an Ace is played, the player playing the Ace can call a suit different what it is in play. The player in sequence must play the card suit called or place anothed Ace.
* If a King of Spades is played, the sequence for one move is reversed, i.e. clockwise play becomes anti-clockwise.
<br>

## Demo
<img src="https://github.com/Budziaszek/Macau-multiplayer-card-game/blob/master/demo/demo.gif" alt="Demo" width="600">
<img src="https://github.com/Budziaszek/Macau-multiplayer-card-game/blob/master/demo/demo2.gif" alt="Demo" width="600">
<img src="https://github.com/Budziaszek/Macau-multiplayer-card-game/blob/master/demo/demo3.png" alt="Demo" width="300">

