
#include "pch.h"
#include "PotsHandler.h"

#include "../general/Flags.h"
#include "../tools/Functions.h"

PotsHandler::PotsHandler() 
	: m_output(nullptr), m_pots(), m_latest(nullptr), m_reason() {}
void PotsHandler::create(std::ostream *output, p_vec::size_type players) {
	m_output = output;

	// probable amount of pots that will be used
	m_pots.reserve(players > 5 ? 4 : 3);
	m_latest = &(*m_pots.push(Pot(players)));
}
PotsHandler::PotsHandler(PotsHandler &&other) noexcept
	: m_output(m_output), m_pots(std::move(other.m_pots)), m_latest(other.m_latest),
	m_reason(std::move(other.m_reason)) {
	other.m_output = nullptr;
	other.m_latest = nullptr;
}
PotsHandler &PotsHandler::operator=(PotsHandler &&other) noexcept {
	if (this == &other)
		return *this;

	m_output = m_output;
	m_pots = std::move(other.m_pots);
	m_latest = other.m_latest;
	m_reason = std::move(other.m_reason);

	other.m_output = nullptr;
	other.m_latest = nullptr;

	return *this;
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

bool PotsHandler::allInCase(pot_it it, p_ptr locker, chips_t amount) {
	bool opened = false;
	if (!it->isOpen()) { // if it's already locked then a new pot is needed
		// contribute amount to current locked pot
		amount = it->addToPot(locker, amount);
		// open a new pot and lock it
		it = openNewPot(locker, 0);
		lockPot(it, locker, amount, true);
		opened = true;
	}
	else
		lockPot(m_pots.end() - 1, locker, amount, true);
	return opened;
}
PotsHandler::pot_it PotsHandler::openNewPot(pot_it it, p_ptr locker, PotState state, chips_t remainder) {
	// if it's locked then the new one will be locked, as well as have a base:
	// [base] = [remainder] / [amount of players]
	it = m_pots.push(Pot(*it, locker, state, remainder));
	m_latest = &(*it);

	return it;
}
PotsHandler::pot_it PotsHandler::openNewPot(p_ptr player, chips_t leftOver) {
	pot_it it = m_pots.push(Pot(m_pots.back(), player, leftOver, m_reason));
	m_latest = &(*it);
	return it;
}
bool PotsHandler::lockPot(pot_it it, p_ptr locker, chips_t amount, bool allIn) {
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
	chips_t remainder = it->lockPot(locker, amount);

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

bool PotsHandler::addToPots(p_ptr player, chips_t amount, bool cantCall) {
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
			return allInCase(it, player, leftOver);

		if (cantCall && it == m_pots.end() - 1) // cant call && last iteration
			leftOver *= -1;
		else
			leftOver = it->addToPot(player, leftOver);

		if (cantCall && leftOver < 0) // this means that this pot needs to be split
			return lockPot(it, player, (chips_t) (-1 * leftOver));
	}

	// a player previously (not necessarily the previous player) called all his chips or 
	// went all in so a new pot is needed
	if (leftOver) {
		openNewPot(player, leftOver);
		return true;
	}
	return false;
}

void PotsHandler::declareWinnersAndDistributeChips() {
	auto &output = *m_output;
	std::vector<std::pair<std::pair<p_ptr, chips_t>, std::vector<pot_it>>> winnersOfPots; // relates players to pots they won

	output << std::endl;
	for (auto it = m_pots.begin(); it != m_pots.end(); it++) {
		const auto &potWinners = it->calcWinners();

		if (potWinners.size() > 1) {
			declarePotMultipleWinners(*it, potWinners);
			std::for_each(potWinners.begin(), potWinners.end(), [] (auto &winPair) 
				{ winPair.first->addChips(winPair.second); }); // add the players' winnings
			continue;
		}
		#if DEBUG
		if (potWinners.empty()) assert("pot has no winner");
		#endif // DEBUG

		// beyond to point the pot only has one winner
		auto &winnerWithAmountPair = potWinners.front();
		auto winnerIt = winnersOfPots.begin();
		for (; winnerIt != winnersOfPots.end(); winnerIt++)
			if (winnerIt->first.first.get() == winnerWithAmountPair.first.get())
				break;

		if (winnerIt != winnersOfPots.end()) { // player already won a pot
			winnerIt->first.second += winnerWithAmountPair.second; // adding the current pot winnings to the players winnings
			winnerIt->second.push_back(it); // adding the pot to the list of pots the player won
		}
		else // player is a new winner
			winnersOfPots.emplace_back(winnerWithAmountPair, std::vector<pot_it>(1, it));
	}
	std::for_each(winnersOfPots.begin(), winnersOfPots.end(), [this, &output] (auto &winnerInfo) {
		declareWinner(winnerInfo);
		winnerInfo.first.first->addChips(winnerInfo.first.second);
		output << std::endl << std::endl; });
}
void PotsHandler::declareWinner(const std::pair<std::pair<p_ptr, chips_t>, std::vector<pot_it>> &winnerInfo) {
	auto &output = *m_output;
	auto &winner = winnerInfo.first.first;

	// pots:
	output << "The winner of ";
	auto last = winnerInfo.second.end() - 1;
	std::for_each(winnerInfo.second.begin(), winnerInfo.second.end(), [last, &output] (pot_it potIt) {
		potIt->potDeclareName(output); 
		output << ((potIt == *last) ? "" : (potIt == *(last - 1)) ? " and " : ", ");
		});

	// winner:
	output << " is (wins ";
	func::commas(output, winnerInfo.first.second);
	*m_output << "$): ";
	output << winner->getName() << " | cards: " << winner->getHand() << " | with the hand: " << std::endl
		<< winner->getBestHand() << std::endl;
}
void PotsHandler::declarePotMultipleWinners(const Pot &pot, const std::vector<std::pair<p_ptr, chips_t>> &winners) {
	auto &output = *m_output;

	output << "The winners of ";
	pot.potDeclareName(output) << " (";
	func::commas(output, winners.front().second) << "$ each) are: ";

	for (auto &winnerPair : winners) {
		output << winnerPair.first->getName();
		if (&winnerPair != &winners.back()) // if not last
			output << ", ";
	}

	output << std::endl << "with the hand: " << winners.front().first->getBestHand();
}

std::ostream &operator<<(std::ostream &output, const PotsHandler &source) {
#if !DEBUG
	if (source.m_pots.size() == 1)
		return output << "Pot: " << source.m_pots.front().getAmount() << '$';
#endif
	output << "Pots: " << std::endl;
	auto it = source.m_pots.begin();
	for (; it != source.m_pots.end() - 1; it++)
		output << *it << std::endl;
	return output << *it;
}
