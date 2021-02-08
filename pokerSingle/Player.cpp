
#include "Player.h"
#include "Table.h"

#include "Flags.h"

int Player::s_ids = 0;

Player::Player()
	: m_id(++s_ids), m_state(PlayerState::folded), m_hand(), m_chips(0), m_chipsTaken(0),
	m_callAmount(0), m_baseRaise(0), m_bestHand(std::make_shared<HandAttempt>()),
	m_table(nullptr), m_latestDecision(), m_name() {}
Player::Player(const Player &other)
	: m_id(other.m_id), m_state(PlayerState::folded), m_hand(other.m_hand), 
	m_chips(other.m_chips), m_chipsTaken(other.m_chipsTaken),
	m_callAmount(other.m_callAmount), m_baseRaise(other.m_baseRaise),
	m_bestHand(std::make_shared<HandAttempt>(*other.m_bestHand)), m_table(other.m_table),
	m_latestDecision(other.m_latestDecision), m_name(other.m_name) {}
Player::Player(int m_chips, const char *m_name, const Table *m_table)
	: m_id(++s_ids), m_state(PlayerState::folded), m_hand(), m_chips(m_chips),
	m_chipsTaken(0), m_callAmount(0), m_baseRaise(0),
	m_bestHand(std::make_shared<HandAttempt>()), m_table(m_table), m_latestDecision(),
	m_name(m_name) {}

void Player::reset(int m_baseRaise, int m_callAmount) {
	this->m_baseRaise = m_baseRaise;
	this->m_callAmount = m_callAmount;
	m_chipsTaken = 0;
	m_state = PlayerState::playing;

	m_latestDecision.reset();
	m_hand.clear();
	m_bestHand->wipe();
}

bool Player::allIn() {
	int amount = m_latestDecision.getAmount();
	if (amount != m_chips) {
		m_latestDecision.changeDecision(Action::raise, m_latestDecision.getAmount());
		raise();
		return false;
	}
	// check if it's a possible raise, if not then call/fold
	if (amount <= m_callAmount) {
		m_latestDecision.changeDecision(Action::call, m_callAmount);
		call();
		return false;
	}
	// since takeChips subtracts the latest decision's previous amount
	int newAmount = m_chips + m_latestDecision.getPreviousAmount();
	m_latestDecision.changeDecision(Action::allIn, newAmount);
	takeChips(newAmount);
	return true;
}
bool Player::raise() {
	if (m_latestDecision.getAmount() == m_chips)
		m_latestDecision.changeDecision(Action::allIn, m_chips);

	// rounding the raise amount as you can only raise an amount that divides by the first 
	// call's amount. e.g: m_callAmount=25, m_baseRaise=5, attemptedRaise=97 would be rounded 
	// to 95
	int amount = ((m_latestDecision.getAmount() - m_callAmount) / m_baseRaise) * m_baseRaise 
		+ m_callAmount;
	if (amount > m_chips) { // cant use canCallAmount() here
		m_latestDecision.changeDecision(Action::allIn, m_chips);
		amount = m_chips;
	}

	// call method handles both < and == to m_callAmount
	if (amount <= m_callAmount) {
		m_latestDecision.changeDecision(Action::call, m_callAmount);
		call();
		return false;
	}

	takeChips(amount);
	return true;
}
bool Player::call() {
	int amount = m_latestDecision.getAmount();
	if (amount > m_callAmount) {
		m_latestDecision.changeDecision(Action::raise, amount);
		return raise();
	}
	if (amount < m_callAmount) {
		m_latestDecision.changeDecision(Action::call, m_callAmount);
		amount = m_callAmount;
	}
	if (amount > m_chips) { // cant use canCallAmount() here
		m_latestDecision.changeDecision(Action::fold);
		fold();
		return false;
	}
	return takeChips(amount); // will return true
}

int Player::finalAction() {
	m_chipsTaken = m_chips;
	m_chips = 0;
	m_state = PlayerState::broke;

	return m_chipsTaken;
}

bool Player::takeChips(int amount) {
	m_chipsTaken = amount - m_latestDecision.getPreviousAmount();
	if (m_chipsTaken > m_chips)
		return false;
	m_chips -= m_chipsTaken;
	return m_chipsTaken;
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

std::ostream &operator<<(std::ostream &output, const Player &source) {
	output << source.m_name << " | id: " << source.m_id << " |";
	if (source.isFolded())
		output << " *folded* |";
	output << " cards: " << source.m_hand << " | chips: ";
	output << source.m_chips << '$' << std::endl << "player's best hand: ";
	if (source.isFolded())
		output << "*n/a*";
	else
		output << *source.m_bestHand;

	return output;
}

void Player::sortPlayers(Player *players, int size) {
	Player *temp;
	for (int i = 1; i < size; i++) {
		if (i == 0)
			i++;
		if (*players[i].m_bestHand < *players[i - 1].m_bestHand) {
			temp = &players[i];
			players[i] = players[i - 1];
			players[i - 1] = *temp;
			i -= 2;
		}
	}
}

Player::~Player() {
}
