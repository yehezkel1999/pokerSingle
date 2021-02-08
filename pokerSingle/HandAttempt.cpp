
#include "HandAttempt.h"

#include "Flags.h"

HandAttempt::HandAttempt()
	: m_cards(), m_handRank(HandRank::highCard), m_hashcode(0) {}
HandAttempt::HandAttempt(const HandAttempt &other)
	: m_cards(), m_handRank(other.m_handRank), m_hashcode(other.m_hashcode) {
	for (int i = 0; i < size; i++)
		m_cards[i] = other.m_cards[i];
}
HandAttempt::HandAttempt(const Card *m_cards)
	: m_cards(), m_handRank(HandRank::highCard), m_hashcode(0) {
	for (int i = 0; i < size; i++)
		this->m_cards[i] = m_cards[i];

	calcFiveCards();
}

void HandAttempt::highCardHandle() {
	int relevance = (int)Relevance::first; // the highest card
	for (int i = size - 1; i >= 0; i--) {
		addHash(m_cards[i], (Relevance)relevance);
		relevance--; // each card after that is less relevant
	}
}
void HandAttempt::twoPairHandle() {
	int count = 0;
	Card card;
	for (int i = size - 1; i > 0; i--) {
		card = m_cards[i];
		if (card.getValue() == m_cards[i - 1].getValue()) { // the card is one of the pairs
			if (i == size - 1 || i == size - 2) // has to be one of these since the sequence is in order
				addHash(card, Relevance::first);
			else // the second pair
				addHash(card, Relevance::second);
			i--;
		}
		else // the lone card
				addHash(card, Relevance::third);
	}
}
bool HandAttempt::twoPairSequence() { // This method gets called after three of a kind so this case cannot happen
	int count = 0;
	for (int i = 1; i < size; i++)
		if (m_cards[i].getValue() == m_cards[i - 1].getValue())
			count++;
	return count > 1; // if count is 2 it has to be a two pair since it can't be three of a kind
}
// this method gets called only if a sequence is times of a kind, so only one value appears 
// more than once
void HandAttempt::timesOfaKindHandle() {
	Card same; // the card that appears more than once
	for (int i = 1; i < size; i++)
		if (m_cards[i].getValue() == m_cards[i - 1].getValue()) {
			same = m_cards[i];
			addHash(same, Relevance::first);
			break;
		}
	int relevance = (int)Relevance::second;
	for (int i = size - 1; i >= 0; i--) // the m_cards are in order by value, from low to high
		if (m_cards[i] != same) {
			addHash(m_cards[i], (Relevance)relevance); 
			relevance--; // each card other than 'same' is less relevant than the other
		}
}
bool HandAttempt::fullHouseSequence() {
	int count = 1; // the amount of times a card is equal to the one before it
	for (int i = 1; i < size; i++) {
		if (m_cards[i].getValue() == m_cards[i - 1].getValue())
			count++;
		// this else is when a card isn't equal to the previous card.
		// since the card values are in order a full house will always start with two equal m_cards
		// so if count is 1 then it is not a full house, also as long as i is not the last cell 
		// then if the next card doesn't have this card's value then it's not a full house, for 
		// example: 22344 or 666JQ
		else if (count == 1 || (i != size - 1 && m_cards[i].getValue() != m_cards[i + 1].getValue()))
			return false;
	}
	return count == 4; // if this is true then it must be a full house since this method 
					   // is called after four of a kind
}
void HandAttempt::fullHouseHandle() {
	// the third card is always the card value of the three of a card in the full house, thus making it
	// more relevent than the other m_cards
	Card relevantCard = m_cards[2];
	addHash(relevantCard, Relevance::first);
	// i starts at one since the first 2 m_cards are always the same
	for (int i = 1; i < size - 1; i++)
		if (m_cards[i] != relevantCard) {
			addHash(relevantCard, Relevance::second);
			return;
		}
}
int HandAttempt::timesOfaKind() {
	int count = 1; // amount of times a card appears
	for (int i = 1; i < size; i++)
		if (m_cards[i].getValue() == m_cards[i - 1].getValue())
			count++;
		else if (count > 1) // one there are two or more of the same value it returns the amount
			return count;
	return count;
}
bool HandAttempt::straightSequence() {
	for (int i = 1; i < size; i++) {
		if (i == size - 1 && m_cards[i].getValue() == 1 && m_cards[i - 1].getValue() == 5) {
			// m_cards = 2345A so we swap it to A2345 so its m_hashcode will be differant than
			// this m_hashcode: 23456 and for aesthetics
			Card ace = m_cards[size - 1]; // Ace
			for (int i = size - 1; i > 0; i--)
				m_cards[i] = m_cards[i - 1];
			m_cards[0] = ace;
			return true;
		}
		// once a card's value isn't one more than the previous one it's not a straight
		if (m_cards[i].getValue() != Card::getOpposingValue(m_cards[i - 1] + 1))
			return false;
	}
	return true;
}
bool HandAttempt::sameSymbolSequence() {
	Symbol symbol = m_cards[0].getSymbol(); // the first symbol
	for (int i = 1; i < size; i++)
		// once a card doesn't have the same symbol as the first card it isn't a flush
		if (m_cards[i].getSymbol() != symbol)
			return false;
	return true;
}
void HandAttempt::addHash(int amount, Relevance relevance) {
	m_hashcode += amount * (int)pow(14, (int)relevance);
}

void HandAttempt::calcFiveCards() {
	if (!Card::isSorted(m_cards, size)) // if it isn't sorted then sort it
		Card::sortCards(m_cards, size);

	m_handRank = HandRank::highCard;
	m_hashcode = 0;
	int times = timesOfaKind();
	bool sameSymbol = sameSymbolSequence();
	bool straightCards = straightSequence();

	if (sameSymbol && straightCards) { // check if it's a straight flush
		if (m_cards[0].getValue() == 10) // check if it's a royal flush
			m_handRank = HandRank::royalFlush;
		else { // a straight flush
			addHash(m_cards[size - 1], Relevance::first);
			m_handRank = HandRank::straightFlush;
		}
	} 
	else if (times == 4) { // check if it's a four of a kind
		timesOfaKindHandle();
		m_handRank = HandRank::fourOfaKind;
	} 
	else if ((times == 2 || times == 3) && fullHouseSequence()) { // check if it's a full house
		fullHouseHandle();
		m_handRank = HandRank::fullHouse;
	} 
	else if (sameSymbol) { // check if it's a flush
		highCardHandle();
		m_handRank = HandRank::flush;
	}
	else if (straightCards) { // check if it's a straight
		addHash(m_cards[size - 1], Relevance::first);
		m_handRank = HandRank::straight;
	}
	else if (times == 3) { // check if it's a three of a kind
		timesOfaKindHandle();
		m_handRank = HandRank::threeOfaKind;
	}
	else if (times == 2 && twoPairSequence()) { // check if it's a two pair
		twoPairHandle();
		m_handRank = HandRank::twoPair;
	}
	else if (times == 2) { // check if it's a pair
		timesOfaKindHandle();
		m_handRank = HandRank::pair;
	}
	else { // it's a high card hand
		highCardHandle();
		m_handRank = HandRank::highCard;
	}
	
	addHash((int)m_handRank, Relevance::rank);
}
void HandAttempt::wipe() {
	m_handRank = HandRank::highCard;
	m_hashcode = 0;
}
const char *HandAttempt::handRankToString() const {
	switch (m_handRank) {
	case HandRank::highCard:
		return "high card";
	case HandRank::pair:
		return "pair";
	case HandRank::twoPair:
		return "two pair";
	case HandRank::threeOfaKind:
		return "three of a kind";
	case HandRank::straight:
		return "straight";
	case HandRank::flush:
		return "flush";
	case HandRank::fullHouse:
		return "full house";
	case HandRank::fourOfaKind:
		return "four of a kind";
	case HandRank::straightFlush:
		return "straight flush";
	case HandRank::royalFlush:
		return "royal flush";
	}
	return "nullptr";
}

Card &HandAttempt::operator[](int place) {
#if DEBUG
	if (m_hashcode)
		throw std::logic_error("attempted to call operator[] on an already calculated HandAttempt");
	if (place < 0 || place > size - 1)
		throw std::out_of_range("HandAttempt array has only 5 cells");
#endif
	return m_cards[place];
}
HandAttempt &HandAttempt::operator=(const HandAttempt &other) {
	if (this == &other)
		return *this;

	for (int i = 0; i < size; i++)
		m_cards[i] = other.m_cards[i];

	m_handRank = other.m_handRank;
	m_hashcode = other.m_hashcode;

	return *this;
}

std::ostream &operator<<(std::ostream &output, const HandAttempt &source) {
	if (!source.m_hashcode)
		return output << "*n/a*";
	output << "hand: " << source.handRankToString();
#if DEBUG
	output << " | hashcode: " << source.m_hashcode;
#endif
	// func::commas(output, source.m_hashcode);
	output << " | cards:" << std::endl;
	for (int i = 0; i < source.size; i++)
		output << source.m_cards[i];
	return output;
}

HandAttempt::~HandAttempt() {
}
