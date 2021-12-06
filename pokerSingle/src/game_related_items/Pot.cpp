
#include "pch.h"
#include "Pot.h"

#include "../general/Flags.h"
#include "../tools/Functions.h"


chips_t Contributer::baseChange(chips_t newBase) {
	_remainder = _amount > newBase ? _amount - newBase : 0;
	if (_remainder)
		_amount = newBase;
	return _remainder;
}

id_t Pot::s_ids = 1;

Pot::Pot(p_vec::size_type capacity) : /* this constructor will always be called first */
	m_id(s_ids), m_base(0), m_state(PotState::start), m_reason(), m_amount(0), m_eligible() {
	s_ids = 1;

	if (capacity)
		m_eligible.reserve(capacity);
}
Pot::Pot(const Pot &other, p_ptr player, PotState state, chips_t amount) :
	m_id(++s_ids), m_base(0), m_state(state), m_reason(player), m_amount(amount),
	m_eligible() {
	m_eligible.reserve(other.m_eligible.capacity() - 1);

	for (const Contributer &contributer : other.m_eligible)
		if (contributer._player != player && isEligible(contributer))
			m_eligible.push_back(contributer);

#if DEBUG
	// this constructor is rached when someone cant call, so if the pot has no players in 
	// it then something went wrong
	if (m_eligible.empty())
		assert("empty pot");
#endif

	chips_t max = 0;
	for (Contributer &contributer : m_eligible) {
		contributer._amount = contributer._remainder;
		contributer._remainder = 0;

		if (contributer._amount > max)
			max = contributer._amount;
	}

	if (!isOpen())
		m_base = max;
}
Pot::Pot(const Pot &other, p_ptr player, chips_t amount, p_ptr reason) :
	m_id(++s_ids), m_base(0), m_state(PotState::start), m_reason(reason), m_amount(amount),
	m_eligible() {
	m_eligible.reserve(other.m_eligible.capacity() - 1);

	m_eligible.push_back(Contributer(player, amount));
}

Pot::Pot(Pot &&other) noexcept :
	m_id(other.m_id), m_amount(other.m_amount), m_base(other.m_base), m_state(other.m_state),
	m_reason(std::move(other.m_reason)), m_eligible(std::move(other.m_eligible)) {

	other.m_id = 0;
	other.m_amount = 0;
	other.m_base = 0;
	other.m_state = PotState::finished;
}
Pot &Pot::operator=(Pot &&other) noexcept {
	if (this == &other)
		return  *this;

	m_id = other.m_id;
	m_amount = other.m_amount;
	m_base = other.m_base;
	m_state = other.m_state;
	m_reason = std::move(other.m_reason);
	m_eligible = std::move(other.m_eligible);

	other.m_id = 0;
	other.m_amount = 0;
	other.m_base = 0;
	other.m_state = PotState::finished;

	return *this;
}

void Pot::reset() {
	m_base = 0;
	m_state = PotState::start;
	m_reason = p_ptr();
	m_amount = 0;
	m_eligible.clear();
}
chips_t Pot::setBase() {
	for (Contributer &contributer : m_eligible)
		if (!contributer._player->isFolded())
			return m_base = contributer._amount;
	return m_base = 0;
}

bool Pot::isEligible(const Contributer &contributer) const {
	Player &player = *contributer._player;
	
	//bool stateDependent = /* if pot is open */ isOpen() ? /* then */

		/* if the call amount can be taken from the player */
	//	player.canCallAmount(player.getCallAmount()) /* TODO fix: this is probably not */
	// the current call amount */ || /* or */
		/* the call amount has already been taken from the player */
	//	player.chipsTakenThisRound() == player.getCallAmount() :

	//	/* else (pot is locked / finished) */ true;
	
	// player is not eligible if he is either folded or doesn't have anything to contribute
	// to this pot (remainder)
	return !player.isFolded() && contributer._remainder /* && stateDependent */;
}
Pot::c_it Pot::exists(p_ptr player) {
	for (c_it it = m_eligible.begin(); it != m_eligible.end(); it++)
		if (it->_player == player)
			return it;
	return m_eligible.end(); // not found
}
Pot::c_it Pot::getOrPush(p_ptr player) {
	c_it it = exists(player);
	if (it == m_eligible.end()) {
		m_eligible.push_back(Contributer(player));
		it = m_eligible.end() - 1;
	}
	return it;
}

Pot::cc_it Pot::firstNonFolded() const {
	for (cc_it it = m_eligible.begin(); it != m_eligible.end(); it++)
		if (!it->_player->isFolded())
			return it;
	return m_eligible.end();
}
chips_t Pot::nonFolded() const {
	chips_t count = 0;
	for (const Contributer &constributer : m_eligible)
		if (!constributer._player->isFolded())
			count++;
	return count;
}

schips_t Pot::addToPot(p_ptr adder, chips_t amount) {
	if (m_state == PotState::finished)
		return amount;

	c_it it = m_eligible.end();


	if (m_state == PotState::start)
		if (m_eligible.empty() || adder != firstNonFolded()->_player) {
			m_eligible.push_back(Contributer(adder));
			it = m_eligible.end() - 1;
		}
		else
			m_state = PotState::open;

	// if the pot's open then it will never push_back the player, only when the pot's locked
	if (it == m_eligible.end())
		it = getOrPush(adder);

	if (isOpen()) {
		addAmountAndTrack(it, amount);
		return 0;
	}
	// m_state == PotState::locked

	if (m_base > it->_amount + amount) // need to split *this* pot
		return -1 * amount;

	chips_t contributed = it->contribute(m_base);
	m_amount += contributed;
	return amount - contributed; // the remainder left from what was taken from the player's chips
}
void Pot::noCheckAdd(p_ptr adder, chips_t amount) {
	addAmountAndTrack(getOrPush(adder), amount);
}
void Pot::addAmountAndTrack(c_it it, chips_t amount) {
	m_amount += amount; // add amount to pot
	it->_amount += amount; // keep track that he contributed the amount
}

chips_t Pot::lockPot(p_ptr locker, chips_t amount) {
	if (m_state != PotState::finished)
		m_state = PotState::locked;

	c_it it = getOrPush(locker);

	// amount -= it->_player->latestChipsTaken(); ?
	addAmountAndTrack(it, amount);

	m_base = it->_amount;

	chips_t remainder, sum = 0;
	for (Contributer &contributer : m_eligible) {
		remainder = contributer.baseChange(m_base);
		m_amount -= remainder;
		sum += remainder;
	}
	return sum;
}

void Pot::calcWinner(std::ostream &output) {
	// will happen a lot so this saves time
	if (oneNonFoldedLeft()) {
		declareWinner(output, oneLeft(), true);
		return;
	}

	for (Contributer &contributer : m_eligible) // calculate every unfolded player's hand
		if (!contributer._player->isFolded())
			contributer._player->checkHand();

	// putting a player that isn't folded in winner so it will be initialized

	c_it winner = m_eligible.end();
	for (c_it it = m_eligible.begin(); it != m_eligible.end(); it++)
		if (!it->_player->isFolded()) {
			winner = it;
			break;
		}
#if DEBUG
	if (winner == m_eligible.end())
		assert("empty pot");
#endif

	// calculate winner first so the highest hashcode will be known before going into the 
	// moreThanOneWinner method
	for (c_it it = m_eligible.begin(); it != m_eligible.end(); it++)
		if (!it->_player->isFolded() && it->_player->getBestHand() > winner->_player->getBestHand())
			winner = it;

	output << std::endl;
	if (moreThanOneWinner(output, *winner->_player))
		return;

	declareWinner(output, *winner->_player);
}
bool Pot::oneNonFoldedLeft() const {
	chips_t count = 0;
	for (const Contributer &constributer : m_eligible) {
		if (!constributer._player->isFolded())
			count++;
		if (count > 1)
			return false;
	}
	return true;
}
Player &Pot::oneLeft() {
	for (Contributer &contributer : m_eligible)
		if (!contributer._player->isFolded())
			return *contributer._player;
#if DEBUG
	assert("oneLeft method called but there were no players left");
#endif
}
chips_t Pot::winnerCount(const Player &winner) {
	chips_t count = 0;
	for (Contributer &contributer : m_eligible)
		if (contributer._player->getBestHand() == winner.getBestHand())
			count++;
	return count;
}
bool Pot::moreThanOneWinner(std::ostream &output, const Player &winner) {
	chips_t count = winnerCount(winner);

	if (count < 2)
		return false;

	chips_t amount = m_amount; // so when this pot is printed the amount will be intact
	chips_t winnings = (chips_t) ((float) m_amount / (float) count);
	output << "The winners of ";
	potDeclareName(output) << " (";
	func::commas(output, winnings) << "$ each) are: ";
	for (Contributer &contributer : m_eligible)
		if (contributer._player->getBestHand() == winner.getBestHand()) {
			output << contributer._player.get()->getName();
			if (count == 1) { // the last winner
				contributer._player->addChips(amount); // so all the chips would actually get distributed
				break;
			}
			output << ", ";

			amount -= winnings;
			contributer._player->addChips(winnings);
			count--;
		}
	output << std::endl << "with the hand: " << winner.getBestHand();
	return true;
}
void Pot::declareWinner(std::ostream &output, Player &winner, bool oneNonFolded) {
	output << "The winner of ";
	potDeclareName(output) << " is (wins ";
	func::commas(output, m_amount);
	output << "$): ";
	output << winner.getName();

	if (oneNonFolded)
		output << ", as this is the only non folded player left in the pot";
	else
		output << " | cards: "<< winner.getHand() << " | with the hand: " << std::endl 
		<< winner.getBestHand();

	winner.addChips(m_amount);
}

bool Pot::operator<(const Pot &other) const {
	return m_eligible.size() == other.m_eligible.size() ? 
		m_state > other.m_state /* PotState::finished > PotState::locked > isOpen() */
		: m_eligible.size() > other.m_eligible.size();
}

const char *Pot::stateToString() const {
	switch (m_state) {
	case PotState::start:
		return "start";
	case PotState::open:
		return "open";
	case PotState::locked:
		return "locked";
	case PotState::finished:
		return "finished";
	}
	return "null";
}
std::ostream &Pot::potDeclareName(std::ostream &output) const {
	return m_id == 1 ? output << "The Main Pot" : output << "Pot #" << m_id;
}
std::ostream &operator<<(std::ostream &output, const Pot &source) {
	source.potDeclareName(output) << ": ";
	func::commas(output, source.m_amount) << "$ | {";

	chips_t count = source.nonFolded();
	if (!count)
		return output << '}';

	for (const Contributer &contributer : source.m_eligible) {
		if (count == 1) {
			output << contributer._player->getName() << '}';
			break;
		}
		if (!contributer._player->isFolded()) {
			output << contributer._player->getName() << ", ";
			count--;
		}
	}
#if DEBUG
	output << " | state: " << source.stateToString();
#endif
	return output;
}
