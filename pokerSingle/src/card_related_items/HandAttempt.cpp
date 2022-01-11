
#include "pch.h"
#include "HandAttempt.h"

#include "../general/Flags.h"
#include "../tools/Functions.h"

HandAttempt::HandAttempt() noexcept
	: m_cards(), m_handRank(HandRank::highCard), m_hashcode(0) {}
HandAttempt::HandAttempt(const HandAttempt &other) noexcept
	: m_cards(), m_handRank(other.m_handRank), m_hashcode(other.m_hashcode) {
	for (size_type i = 0; i < s_size; i++)
		m_cards[i] = other.m_cards[i];
}
HandAttempt::HandAttempt(const Card *cards) noexcept
	: m_cards(), m_handRank(HandRank::highCard), m_hashcode(0) {
	for (size_type i = 0; i < s_size; i++)
		m_cards[i] = cards[i];

	calcFiveCards();
}

void HandAttempt::highCardHandle() {
	unsigned int relevance = (unsigned int)Relevance::first; // the highest card
	for (signed_size_type i = s_size - 1; i >= 0; i--) {
		addHash(m_cards[i], (Relevance)relevance);
		relevance--; // each card after that is less relevant
	}
}
void HandAttempt::twoPairHandle() {
	Card card;
	for (signed_size_type i = s_size - 1; i > 0; i--) {
		card = m_cards[i];
		if (card.getValue() == m_cards[i - 1].getValue()) { // the card is one of the pairs
			if (i == s_size - 1 || i == s_size - 2) // has to be one of these since the sequence is in order
				addHash(card, Relevance::first);
			else // the second pair
				addHash(card, Relevance::second);
			i--; // so the card's value wont be added twice
		}
		else // the lone card
			addHash(card, Relevance::third);
	}
}
bool HandAttempt::twoPairSequence() { // This method gets called after three of a kind so this case cannot happen
	size_type count = 0;
	for (size_type i = 1; i < s_size; i++)
		if (m_cards[i].getValue() == m_cards[i - 1].getValue())
			count++;
	return count > 1; // if count is 2 it has to be a two pair since it can't be three of a kind
}
// this method gets called only if a sequence is times of a kind, so only one value appears 
// more than once
void HandAttempt::timesOfaKindHandle() {
	Card same; // the card that appears more than once
	for (size_type i = 1; i < s_size; i++)
		if (m_cards[i].getValue() == m_cards[i - 1].getValue()) {
			same = m_cards[i];
			addHash(same, Relevance::first);
			break;
		}
	unsigned int relevance = (unsigned int)Relevance::second;
	for (signed_size_type i = s_size - 1; i >= 0; i--) // the cards are in order by value, from low to high
		if (m_cards[i] != same) {
			addHash(m_cards[i], (Relevance)relevance); 
			relevance--; // each card other than 'same' is less relevant than the other
		}
}
bool HandAttempt::fullHouseSequence() {
	size_type count = 1; // the amount of times a card is equal to the one before it
	for (size_type i = 1; i < s_size; i++) {
		if (m_cards[i].getValue() == m_cards[i - 1].getValue())
			count++;
		// this else is when a card isn't equal to the previous card.
		// since the card values are in order a full house will always start with two equal cards
		// so if count is 1 then it is not a full house, also as long as i is not the last cell 
		// then if the next card doesn't have this card's value then it's not a full house, for 
		// example: 22344 or 666JQ
		else if (count == 1 || (i != s_size - 1 && m_cards[i].getValue() != m_cards[i + 1].getValue()))
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
	for (size_type i = 1; i < s_size - 1; i++)
		if (m_cards[i] != relevantCard) {
			addHash(relevantCard, Relevance::second);
			return;
		}
}
HandAttempt::size_type HandAttempt::timesOfaKind() {
	size_type count = 1; // amount of times a card appears
	for (size_type i = 1; i < s_size; i++)
		if (m_cards[i].getValue() == m_cards[i - 1].getValue())
			count++;
		else if (count > 1) // once there are two or more of the same value it returns the amount
			return count;
	return count;
}
bool HandAttempt::straightSequence() {
	for (size_type i = 1; i < s_size; i++) {
		if (i == s_size - 1 && m_cards[i].getValue() == 1 && m_cards[i - 1].getValue() == 5) {
			// m_cards = 2345A so we swap it to A2345 so its m_hashcode will be differant than
			// this m_hashcode: 23456 and for aesthetics
			Card ace = m_cards[s_size - 1]; // Ace
			for (signed_size_type i = s_size - 1; i > 0; i--)
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
	for (size_type i = 1; i < s_size; i++)
		// once a card doesn't have the same symbol as the first card it isn't a flush
		if (m_cards[i].getSymbol() != symbol)
			return false;
	return true;
}
void HandAttempt::addHash(unsigned int amount, Relevance relevance) noexcept {
	m_hashcode += amount * (unsigned int)pow(14, (int)relevance);
}

void HandAttempt::calcFiveCards() {
	if (!std::is_sorted(begin(), end())) // if it isn't sorted then sort it
		std::sort(begin(), end());

	m_handRank = HandRank::highCard;
	m_hashcode = 0;
	size_type times = timesOfaKind();
	bool sameSymbol = sameSymbolSequence();
	bool straightCards = straightSequence();

	if (sameSymbol && straightCards) { // check if it's a straight flush
		if (m_cards[0].getValue() == 10) // check if it's a royal flush
			m_handRank = HandRank::royalFlush;
		else { // a straight flush
			addHash(m_cards[s_size - 1], Relevance::first);
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
		addHash(m_cards[s_size - 1], Relevance::first);
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
	
	addHash((unsigned int)m_handRank, Relevance::rank);
}
void HandAttempt::wipe() noexcept {
	m_handRank = HandRank::highCard;
	m_hashcode = 0;
}
const char *HandAttempt::handRankToString() const noexcept {
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

Card &HandAttempt::operator[](size_type place) {
#if DEBUG
	assert(!m_hashcode && "attempted to call operator[] on an already calculated HandAttempt");
	assert(place >= 0 && place < s_size && "HandAttempt array has only 5 cells");
#endif
	return m_cards[place];
}
HandAttempt &HandAttempt::operator=(const HandAttempt &other) noexcept {
	if (this == &other)
		return *this;

	for (size_type i = 0; i < s_size; i++)
		m_cards[i] = other.m_cards[i];

	m_handRank = other.m_handRank;
	m_hashcode = other.m_hashcode;

	return *this;
}

std::ostream &operator<<(std::ostream &output, const HandAttempt &source) noexcept {
	if (!source.m_hashcode)
		return output << "*n/a*";
	output << "hand: " << source.handRankToString();
#if DEBUG
	output << " | hashcode: " << source.m_hashcode;
#endif
	// func::commas(output, source.m_hashcode);
	output << " | cards:" << std::endl;
	for (HandAttempt::size_type i = 0; i < HandAttempt::s_size; i++)
		output << source.m_cards[i];
	return output;
}
