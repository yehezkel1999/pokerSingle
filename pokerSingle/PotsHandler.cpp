
#include "Flags.h"
#include "PotsHandler.h"

PotsHandler::PotsHandler() : m_output(nullptr), m_pots(), m_latest(nullptr), m_reason() {
}
void PotsHandler::create(std::ostream *output, const p_vec &players) {
	m_output = output;

	// probable amount of pots that will be used
	m_pots.reserve(players.size() > 5 ? 4 : 3);
	m_latest = &(*m_pots.push(Pot(players)));
}

void PotsHandler::reset(const p_vec &players) {
	if (m_pots.size() > 1)
		m_pots.erase(m_pots.begin() + 1, m_pots.end());
	m_pots.front().reset();
	Pot::s_ids = 1;
}
void PotsHandler::postStage() {
	for (pot_it it = m_pots.begin(); it != m_pots.end(); it++) {
		it->setBase();
		if (it == m_pots.end() - 1)
			break;
		// once all the players have been established all the pots (except the last one) 
		// will be maked as finished as all the players have contributed to them the max 
		// amount
		it->finished(); 
	}
}

PotsHandler::pot_it PotsHandler::openNewPot(pot_it it, p_ptr locker, PotState state, size_t remainder) {
	// if it's locked then the new one will be locked, as well as have a base:
	// [base] = [remainder] / [amount of players]
	it = m_pots.push(Pot(*it, locker, state, remainder));
	m_latest = &(*it);
	m_reason = locker;

	return it;
}
PotsHandler::pot_it PotsHandler::openNewPot(p_ptr player, size_t leftOver) {
	pot_it it = m_pots.push(Pot(m_pots.back(), player, leftOver));
	m_latest = &(*it);
	return it;
}
bool PotsHandler::lockPot(pot_it it, p_ptr locker, size_t amount, bool allIn) {
	/**
	 * Player cannot call:
	 *     1. change the current pot's base to the max amount this player can call plus the old 
	 *        of this pot: [old base] + [max player call] and lock it there.
	 *     2. leave in that pot: the [base] * [amount of players in pot] (including the player who 
	 *        can't call) and save the rest of the chips in the pot: [total] - [base] * [players].
	 *     3. add a new pot with the saved value in it and add all the players from the 
	 *        previous pot to it.
	 * 
	 * aka: splitting this pot into 2 pots where the first one the player can be in and 
	 * the second one the player is not.
	 */

	bool opened = false;
	PotState state = it->getState();
	// so chips of folded players wouldn't get lost
	size_t remainder = it->lockPot(locker, amount);

	// save player as the reason a new pot was opened in case a new pot wont be opened 
	// right now 
	m_reason = locker;

	// if the pot was open and after it's been locked the only player that contributed to 
	// the pot is this player then another pot will only be need to be opened once someone 
	// adds more than this amount to the pots

	// fix for all special occurrences: only if remainder has a value then a new pot is 
	// needed at the moment, but not if someone went all in
	if (remainder && !allIn) {
		openNewPot(it, locker, state, remainder);
		opened = true;
	}
	return opened;
}

bool PotsHandler::addToPots(p_ptr player, size_t amount, bool cantCall) {
	if (!amount)
		return false;

	bool allIn = !cantCall && player->getChips() == 0;
	int leftOver = amount;
	for (pot_it it = m_pots.begin(); it != m_pots.end(); it++) {
		// this means the player can't be included in this pot as he had the exact amount 
		// to be in the previous pot, also since this player already contributed all his 
		// chips a new pot does not need to be opened
		if (cantCall && leftOver == 0)
			break;

		// player called all his chips / went all in: need to lock the pot, but not open 
		// a new pot
		if (allIn && it == m_pots.end() - 1)
			return lockPot(m_pots.end() - 1, player, leftOver, true);

		if (cantCall && it == m_pots.end() - 1) // cant call && last iteration
			leftOver *= -1;
		else
			leftOver = it->addToPot(player, leftOver);

		if (cantCall && leftOver < 0) // this means that this pot needs to be split
			return lockPot(it, player, (size_t) (-1 * leftOver));
	}

	// a player previously (not necessarily the previous player) called all his chips or 
	// went all in so a new pot is needed
	if (leftOver) {
		openNewPot(player, leftOver);
		return true;
	}
	return false;
}
void PotsHandler::calcWinners(bool oneLeft) {
	for (Pot &pot : m_pots) {
		pot.calcWinner(*m_output);
		*m_output << std::endl << std::endl;
	}
}

std::ostream &operator<<(std::ostream &output, const PotsHandler &source) {
#if !DEBUG
	if (source.m_pots.size() == 1)
		return output << "pot: " << source.m_pots.front().getAmount() << '$';
#endif
	output << "pots: " << std::endl;
	auto it = source.m_pots.begin();
	for (; it != source.m_pots.end() - 1; it++)
		output << *it << std::endl;
	return output << *it;
}

PotsHandler::~PotsHandler() {}
