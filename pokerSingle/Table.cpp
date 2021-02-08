
#include "Table.h"

#include "HandAttempt.h"
#include "Hand.h"
#include "Functions.h"

#include "Flags.h"

Table::Table()
	: m_deck(), m_table(), m_curAmount(0) {
	m_deck.shuffle();
}

void Table::reset() noexcept {
	for (size_type i = 0; i < m_curAmount; i++)
		m_table[i] = Card();
	m_curAmount = 0;
	m_deck.newDeck();
	m_deck.shuffle();
}
const Card Table::takeCardFromDeck() noexcept {
	return m_deck.takeTopCard();
}
void Table::drawAllCardsToTable() noexcept {
	for (size_type i = m_curAmount; i < Table::s_size; i++)
		drawCardToTable();
}
void Table::drawCardToTable() noexcept {
	m_table[m_curAmount] = m_deck.takeTopCard();
	m_curAmount++;
}
void Table::firstDrawToTable() noexcept {
	for (size_type i = 0; i < 3; i++)
		drawCardToTable();
}
Stage Table::currentStage() const noexcept {
	switch (m_curAmount) {
	case 5:
		return Stage::river;
	case 4:
		return Stage::turn;
	case 3:
		return Stage::flop;
	}
	return Stage::preFlop;
}

// only accepts 6 card sequences, allocates and returns memory
void Table::calcSixCards(const Card *cards, std::shared_ptr<HandAttempt> handAttempt) const {
	HandAttempt five, max;
	HandAttempt::size_type count = 5;

	for (HandAttempt::size_type i = 0; i < HandAttempt::s_size ; i++)
		five[i] = cards[i];
	five.calcFiveCards();

	max = five;
	for (HandAttempt::signed_size_type i = count - 1; i >= 0; i--) { // count will always be one more than i
		five.wipe(); // wipes it so [] operator won't throw an exception
		five[i] = cards[count]; // every loop five replaces the i cell with the card that 
		count--;				// wasn't in the sequence the last loop
		five.calcFiveCards();
		if (five > max)
			max = five;
	}

	*handAttempt = HandAttempt(max);
}
// only accepts 7 card sequences, allocates and returns memory
void Table::calcSevenCards(const Card *cards, std::shared_ptr<HandAttempt> handAttempt) const {
	HandAttempt five, max;
	HandAttempt::size_type count = 0;

	for (HandAttempt::size_type i = 0; i < 7; i++)
		for (HandAttempt::size_type j = 0; j < 7; j++) {
			if (j == i) // eliminates cards[i] card for 6 creations
				continue;
			for (HandAttempt::size_type k = 0; k < 7 && count < HandAttempt::s_size; k++) {
				if (k == i || k == j) // elimitnates a card for every sequence that doesn't
					continue;		  // have the cards[i] card (6 sequences)
				five[count] = cards[k];
				count++;
			}
			five.calcFiveCards();
			if (five > max)
				max = five;
			count = 0; // resets count to rebuild five
			five.wipe(); // wipes it so [] operator won't throw an exception
		}
	*handAttempt = HandAttempt(max);
}
void Table::calculateHand(const Hand& hand, std::shared_ptr<HandAttempt> handAttempt) const {
#if DEBUG
	if (!m_curAmount)
		throw std::logic_error("attempted to calculate a player's hand with no cards on the m_table");
#endif
	size_type size = 2 + m_curAmount;
	Card cards[7];
	cards[0] = hand.getFirstCard();	cards[1] = hand.getSecondCard();
	for (size_type i = 2; i < size; i++)
		cards[i] = m_table[i - 2];
	func::sort(cards, size);

	/**
	 * This switch acts for cases 5, 6 and 7 as there can only be 5 cards open on the m_table
	 * and a player can have only 2 cards.
	 * This switch creates a 5 card array from all the combinations of cards from the original
	 * cards array.
	 */
	switch (size) {
	case 5:
		*handAttempt = HandAttempt(cards);
		break;
	case 6:
		calcSixCards(cards, handAttempt);
		break;
	case 7:
		calcSevenCards(cards, handAttempt);
		break;
	}
}

std::ostream &operator<<(std::ostream &output, const Table &source) noexcept {
	output << "Table, open cards: ";
	if (!source.m_curAmount)
		output << "*no open cards yet*";
	for (Table::size_type i = 0; i < source.m_curAmount; i++)
		output << source.m_table[i];
	return output;
}
