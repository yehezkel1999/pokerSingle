
// pokerSingle.cpp : Defines the entry point for the console application.
//

#define SIZE 5

#include "game_related_items/Game.h"
#include "tools/Functions.h"
#include "tools/random.h"
#include "players/Bot.h"


void testDeck() {
	Deck *d1 = new Deck();
	std::cout << "d1: " << *d1 << std::endl << std::endl;

	d1->shuffle();
	std::cout << "d1 shuffled: " << *d1 << std::endl << std::endl;

	Card c1 = d1->takeTopCard();
	std::cout << "card taken from top d1: " << c1 << std::endl;
	std::cout << *d1 << std::endl << std::endl;
	d1->placeCardRandom(c1);
	std::cout << "card placed randomly in d1: " << c1 << std::endl;
	std::cout << *d1 << std::endl << std::endl;

	Card c2 = d1->takeRandomCard();
	std::cout << "card taken from random: " << c2 << std::endl;
	std::cout << *d1 << std::endl << std::endl;
	d1->placeCardBottom(c2);
	std::cout << "card placed bottem in d1: " << c2 << std::endl;
	std::cout << *d1 << std::endl << std::endl;

	delete d1;

}
void testCalculations() {
	Card *cards;

	cards = new Card[SIZE];
	cards[0] = Card(1, Symbol::diamond);
	cards[1] = Card(13, Symbol::diamond);
	cards[2] = Card(12, Symbol::diamond);
	cards[3] = Card(11, Symbol::diamond);
	cards[4] = Card(10, Symbol::diamond);
	func::sort(cards, SIZE);

	HandAttempt h1(cards);
	std::cout << h1 << std::endl << std::endl;

	cards[0] = Card(9, Symbol::diamond);
	cards[1] = Card(13, Symbol::diamond);
	cards[2] = Card(12, Symbol::diamond);
	cards[3] = Card(11, Symbol::diamond);
	cards[4] = Card(10, Symbol::diamond);
	func::sort(cards, SIZE);

	HandAttempt h2(cards);
	std::cout << h2 << std::endl << std::endl;
	if (cards)
		delete[] cards;
}
void testTable() {
	Table t;
	int size = 10;
	Player *players;
	// players = new Bot[size];

	for (int i = 0; i < size; i++) {
		// players[i].setTable(&t);
		// players[i].drawHand(t.takeCardFromDeck(), t.takeCardFromDeck());
	}

	t.firstDrawToTable();
	std::cout << "t: " << t << std::endl << std::endl;
	for (int i = 0; i < size; i++)
		players[i].checkHand();
	std::cout << std::endl << std::endl;
	Player::sortPlayers(players, size);
	for (int i = 0; i < size; i++)
		std::cout << players[i] << std::endl << std::endl;

	t.drawCardToTable();
	std::cout << "t: " << t << std::endl << std::endl;
	for (int i = 0; i < size; i++)
		players[i].checkHand();
	std::cout << std::endl << std::endl;
	Player::sortPlayers(players, size);
	for (int i = 0; i < size; i++)
		std::cout << players[i] << std::endl << std::endl;

	t.drawCardToTable();
	std::cout << "t: " << t << std::endl << std::endl;
	for (int i = 0; i < size; i++)
		players[i].checkHand();
	std::cout << std::endl << std::endl;
	Player::sortPlayers(players, size);
	for (int i = 0; i < size; i++)
		std::cout << players[i] << std::endl << std::endl;
}

/*
void testFixedHands() {
	Card *cards = new Card[SIZE];
	Game game(0, &cout);


	cards[0] = Card(1, Symbol::diamond);
	cards[1] = Card(5, Symbol::spade);
	cards[2] = Card(8, Symbol::club);
	cards[3] = Card(3, Symbol::heart);
	cards[4] = Card(10, Symbol::diamond);
	Card::sortCards(cards, SIZE);
	game.addFixedPlayer(HandAttempt(cards));

	cards[0] = Card(1, Symbol::diamond);
	cards[1] = Card(5, Symbol::spade);
	cards[2] = Card(8, Symbol::club);
	cards[3] = Card(3, Symbol::heart);
	cards[4] = Card(10, Symbol::diamond);
	Card::sortCards(cards, SIZE);
	game.addFixedPlayer(HandAttempt(cards));

	cards[0] = Card(1, Symbol::diamond);
	cards[1] = Card(5, Symbol::spade);
	cards[2] = Card(8, Symbol::club);
	cards[3] = Card(3, Symbol::heart);
	cards[4] = Card(9, Symbol::diamond);
	Card::sortCards(cards, SIZE);
	game.addFixedPlayer(HandAttempt(cards));


	game.start();


	if (cards)
		delete[] cards;
}
*/


void testGame() {
	int players = 10;
	bool bots = true;

#if !DEBUG
	int input;

	std::cout << "Welcome to my poker tournament game." << std::endl;
	std::cout << "Input 0 if you want to play or 1 if you want to watch bots play each other"
		<< std::endl;
	std::cin >> input;
	while (input < 0 || input > 1) {
		std::cout << "Invalid input, enter 0 if you want to play or 1 if you want to watch"
			" bots play each other " << std::endl;
		std::cin >> input;
	}
	bots = input;

	std::cout << "Input the total amount of players you want to be included in the game"
		" (2 - 10)"	<< std::endl;
	std::cin >> input;
	while (input < 2 || input > 10) {
		std::cout << "Invalid input, there can only be a minimum of 2 players and a maximum"
			" 10 players" << std::endl;
		std::cin >> input;
	}
	players = input;
#endif

	Game game(players, bots);
	game.start();
}
void testCommas() {
	chips_t num = 0;
	for (int i = 0; i < 100; i++) {
		num += (chips_t) random::randInt(0, 3) * 1000000000;
		num += (chips_t) random::randInt(0, 9) * 1000000;
		num += (chips_t) random::randInt(0, 9) * 1000;
		num += random::randInt(0, 9);
		std::cout << "num: " << num << ", commas: ";
		func::commas(std::cout, num) << std::endl;
	}
}

int main() {
	// testDeck();
	// testCalculations();
	// testTable();
	// testFixedHands();
	// testCommas();
	
	testGame();

	int x;
	std::cin >> x; // for exe pause

	return 0;
}