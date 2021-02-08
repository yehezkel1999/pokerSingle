#include "ConsolePlayer.h"

ConsolePlayer::ConsolePlayer()
	: Player() {}
ConsolePlayer::ConsolePlayer(const ConsolePlayer &other)
	: Player(other) {}
ConsolePlayer::ConsolePlayer(chips_t chips, const Table *table)
	: Player(chips, "The player", table) {}

const Decision &ConsolePlayer::doTurn() {
	chips_t input;
	chips_t call = Player::getCallAmount();
	std::cout << "Your cards: " << Player::getHand() << " | your chips: " << Player::getChips()
		<< std::endl << "Input 0 to fold, 1 to call (" << call << "$), more to raise TO "
		<< "(base raise is " << Player::getBaseRaise() << "$)" << std::endl
		<< "In order to raise, input an amount that is the *call amount* plus *the base "
		"raise time n*" << std::endl << "your best hand: " << Player::getBestHand() 
		<< std::endl;
	std::cin >> input;

	while (true) {
		if (input == 0)
			return m_latestDecision.newDecision(Action::fold);
		else if (input == 1)
			return m_latestDecision.newDecision(Action::call, Player::getCallAmount());
		else if (input < call) {
			std::cout << "invalid input, if you want to raise, "
				"enter something higher than the current call amount (";
			std::cout << call;
			std::cout << "$)\nOtherwise enter 0 to fold, 1 to call";
		}
		else if (input == call)
			return m_latestDecision.newDecision(Action::call, call);
		else if ((input - getCallAmount()) % getBaseRaise() != 0)
			std::cout << "invalid input, you raised an uneven amount "
			"(compared to the call amount plus the base raise times n)";
		else
			return m_latestDecision.newDecision(Action::raise, input);

		std::cin >> input;
	}
	return m_latestDecision.newDecision(Action::fold); // just fo return paths..
}

const Decision &ConsolePlayer::notEnoughChips() {
	chips_t input;
	std::cout << "Your cards: " << Player::getHand() << " | your chips: " << Player::getChips()
		<< std::endl << "You do not have enough chips to call, input 0 to fold or 1 to call "
		"your remaining chips to stay in the hand" << std::endl;
	std::cin >> input;
	return input == 0 ? m_latestDecision.newDecision(Action::fold) :
		m_latestDecision.newDecision(Action::raise, input);
}
