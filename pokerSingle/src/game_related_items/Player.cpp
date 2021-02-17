
#include "Player.h"
#include "../card_related_items/Table.h"
#include "../tools/Functions.h"

#include "../Flags.h"

id_t Player::s_ids = 0;

Player::Player(const Player &other)
	: m_id(other.m_id), m_state(other.m_state), m_hand(other.m_hand), 
	m_chips(other.m_chips), m_chipsTaken(other.m_chipsTaken),
	m_callAmount(other.m_callAmount), m_baseRaise(other.m_baseRaise),
	m_bestHand(std::make_shared<HandAttempt>(*other.m_bestHand)), m_table(other.m_table),
	m_latestDecision(other.m_latestDecision), m_name(other.m_name) {}
Player::Player(Player &&other) noexcept
	: m_id(other.m_id), m_state(other.m_state), m_hand(std::move(other.m_hand)),
	m_chips(other.m_chips), m_chipsTaken(other.m_chipsTaken),
	m_callAmount(other.m_callAmount), m_baseRaise(other.m_baseRaise),
	m_bestHand(std::move(other.m_bestHand)), m_table(other.m_table),
	m_latestDecision(std::move(other.m_latestDecision)), m_name(std::move(other.m_name)) {
	other.m_id = 0;
	other.m_state = PlayerState::broke;
	other.m_chips = 0;
	other.m_chipsTaken = 0;
	other.m_callAmount = 0;
	other.m_baseRaise = 0;
	other.m_table = nullptr;
}
Player::Player(chips_t m_chips, const char *m_name, const Table *m_table)
	: m_id(++s_ids), m_state(PlayerState::folded), m_hand(), m_chips(m_chips),
	m_chipsTaken(0), m_callAmount(0), m_baseRaise(0),
	m_bestHand(std::make_shared<HandAttempt>()), m_table(m_table), m_latestDecision(),
	m_name(m_name) {}

void Player::reset(chips_t baseRaise, chips_t callAmount) {
	m_baseRaise = baseRaise;
	m_callAmount = callAmount;
	m_chipsTaken = 0;
	m_state = PlayerState::playing;

	m_latestDecision.reset();
	m_hand.clear();
	m_bestHand->wipe();
}

void Player::allIn() {
	chips_t amount = m_latestDecision.getAmount();
	if (amount != m_chips) {
		m_latestDecision.changeDecision(Action::raise, m_latestDecision.getAmount());
		raise();
		return;
	}
	// check if it's a possible raise, if not then call/fold
	if (amount <= m_callAmount) {
		m_latestDecision.changeDecision(Action::call, m_callAmount);
		call();
		return;
	}

	takeChips(m_chips, true);
}
void Player::raise() {
	if (m_latestDecision.getAmount() >= m_chips) {
		m_latestDecision.changeDecision(Action::allIn, m_chips);
		allIn();
		return;
	}

	// rounding the raise amount as you can only raise an amount that divides by the first 
	// call's amount. e.g: m_callAmount=25, m_baseRaise=5, attemptedRaise=97 would be rounded 
	// to 95
	chips_t amount = ((m_latestDecision.getAmount() - m_callAmount) / m_baseRaise) * 
		m_baseRaise	+ m_callAmount;
	if (amount > m_chips) { // cant use canCallAmount() here
		m_latestDecision.changeDecision(Action::allIn, m_chips);
		allIn();
		return;
	}

	// call method handles both < and == to m_callAmount
	if (amount <= m_callAmount) {
		m_latestDecision.changeDecision(Action::call, m_callAmount);
		call();
		return;
	}

	takeChips(amount);
}
void Player::call() {
	chips_t amount = m_latestDecision.getAmount();
	if (amount > m_callAmount) {
		m_latestDecision.changeDecision(Action::raise, amount);
		raise();
		return;
	}
	if (amount < m_callAmount) {
		m_latestDecision.changeDecision(Action::call, m_callAmount);
		amount = m_callAmount;
	}
	if (amount > m_chips) { // cant use canCallAmount() here
		m_latestDecision.changeDecision(Action::fold);
		fold();
		return;
	}
	takeChips(amount); // will return true
}

chips_t Player::finalAction() {
	m_chipsTaken = m_chips;
	m_chips = 0;
	m_state = PlayerState::broke;

	return m_chipsTaken;
}

void Player::takeChips(chips_t amount, bool allIn) {
	// since all in simply places in the chips
	m_chipsTaken = allIn ? amount : amount - m_latestDecision.getPreviousAmount();
#if DEBUG
	if (m_chipsTaken > m_chips)
		throw std::logic_error("tried taking more chips than the player has");
#endif
	m_chips -= m_chipsTaken;
}

Action Player::doAction() {
	switch (m_latestDecision.getAction()) {
	case Action::fold:
		fold();
		break;
	case Action::call:
		call();
		break;
	case Action::raise:
		raise();
		break;
	case Action::allIn:
		allIn();
		break;
	}
	return m_latestDecision.getAction();
}

void Player::checkHand() {
	if (m_table && m_table->areCardsOnTable())
		m_table->calculateHand(m_hand, m_bestHand);
}

Player &Player::operator=(const Player &other) {
	if (this == &other)
		return *this;

	m_id = other.m_id;
	m_state = other.m_state;
	m_hand = other.m_hand;

	m_chips = other.m_chips;
	m_chipsTaken = other.m_chipsTaken;

	m_callAmount = other.m_callAmount;
	m_baseRaise = other.m_baseRaise;

	*m_bestHand = *other.m_bestHand;
	m_table = other.m_table;

	m_latestDecision = other.m_latestDecision;
	m_name = other.m_name;

	return *this;
}

Player &Player::operator=(Player &&other) noexcept {
	if (this == &other)
		return *this;

	m_id = other.m_id;
	m_state = other.m_state;
	m_hand = std::move(other.m_hand);
	m_chips = other.m_chips;
	m_chipsTaken = other.m_chipsTaken;
	m_callAmount = other.m_callAmount;
	m_baseRaise = other.m_baseRaise;
	m_bestHand = std::move(other.m_bestHand);
	m_table = other.m_table;
	m_latestDecision = std::move(other.m_latestDecision);
	m_name = std::move(other.m_name);

	other.m_id = 0;
	other.m_state = PlayerState::broke;
	other.m_chips = 0;
	other.m_chipsTaken = 0;
	other.m_callAmount = 0;
	other.m_baseRaise = 0;
	other.m_table = nullptr;

	return *this;
}

std::ostream &operator<<(std::ostream &output, const Player &source) {
	output << source.m_name << " | id: " << source.m_id << " |";
	if (source.isFolded())
		output << " *folded* |";
	output << " cards: " << source.m_hand << " | chips: ";
	func::commas(output, source.m_chips) << '$' << std::endl << "player's best hand: ";
	if (source.isFolded())
		output << "*n/a*";
	else
		output << *source.m_bestHand;

	return output;
}
