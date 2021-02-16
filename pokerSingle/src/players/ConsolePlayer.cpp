#include "ConsolePlayer.h"

ConsolePlayer::ConsolePlayer(const ConsolePlayer &other)
	: Player(other) {}
ConsolePlayer::ConsolePlayer(ConsolePlayer &&other) noexcept
	: Player(std::move(other)) {}
ConsolePlayer::ConsolePlayer(chips_t chips, const Table *table)
	: Player(chips, "The player", table) {}

const Decision &ConsolePlayer::doTurn() {
	chips_t input;
	chips_t call = Player::getCallAmount();

	std::cout << "Your cards: " << Player::getHand() << " | your chips: " << Player::getChips();
	std::cout << std::endl << "Your best hand: " << Player::getBestHand() << std::endl;
	std::cout << "Input 0 to fold, 1 to ";
	if (!m_latestDecision.difference())
		std::cout << "check";
	else
		std::cout << "call (" << call << "$)";

	std::cout << ", more to raise TO (base raise is " << Player::getBaseRaise() << "$)" 
		<< std::endl << "In order to raise, input an amount that is ";

	if (m_latestDecision.difference())
		std::cout << call << "$ plus ";

	std::cout << Player::getBaseRaise() << "$ times n*" << std::endl;

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
			std::cout << "$)\nOtherwise enter 0 to fold, 1 to "
				<< m_latestDecision.difference() ? "call" : "check";
		}
		else if (input == call)
			return m_latestDecision.newDecision(Action::call, call);
		else if ((input - getCallAmount()) % getBaseRaise() != 0)
			std::cout << "invalid input, you raised an uneven amount "
			"(compared to the call amount plus the base raise times n)";
		else
			return m_latestDecision.newDecision(Action::raise, input);

		std::cout << std::endl;
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

ConsolePlayer &ConsolePlayer::operator=(ConsolePlayer &&other) noexcept {
	if (this == &other)
		return *this;

	Player::operator=(std::move(other));

	return *this;
}
ConsolePlayer &ConsolePlayer::operator=(const ConsolePlayer &other) {
	if (this == &other)
		return *this;

	Player::operator=(other);

	return *this;
}
