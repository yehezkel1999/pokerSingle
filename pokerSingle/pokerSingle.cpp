
// pokerSingle.cpp : Defines the entry point for the console application.
//

#define SIZE 5

#include "Game.h"
#include "Bot.h"

#include "Flags.h"


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
	Card::sortCards(cards, SIZE);

	HandAttempt h1(cards);
	std::cout << h1 << std::endl << std::endl;

	cards[0] = Card(9, Symbol::diamond);
	cards[1] = Card(13, Symbol::diamond);
	cards[2] = Card(12, Symbol::diamond);
	cards[3] = Card(11, Symbol::diamond);
	cards[4] = Card(10, Symbol::diamond);
	Card::sortCards(cards, SIZE);

	HandAttempt h2(cards);
	std::cout << h2 << std::endl << std::endl;
	if (cards)
		delete[] cards;
}
void testTable() {
	Table t;
	int size = 10;
	Player *players = new Bot[size];

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
	Game game(10);
	game.start();
}

int main() {
	// testDeck();
	// testCalculations();
	// testTable();

	testGame();

	// testFixedHands();

	int x;
	std::cin >> x; // for exe pause

	return 0;
}