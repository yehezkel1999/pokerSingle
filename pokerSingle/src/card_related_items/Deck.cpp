
#include "Deck.h"
#include "../tools/CustomExceptions.h"
#include "../tools/random.h"

#include "../Flags.h"


Deck::Deck() 
	: m_deck(new Card[s_maxSize]), m_curAmount(s_maxSize) {
	newDeck();
}
Deck::Deck(const Deck &other) 
	: m_deck(new Card[s_maxSize]), m_curAmount(other.m_curAmount) {
	for (size_type i = 0; i < s_maxSize; i++)
		m_deck[i] = other.m_deck[i];
}
Deck::Deck(Deck &&other) noexcept
	: m_deck(other.m_deck), m_curAmount(other.m_curAmount) {
	other.m_deck = nullptr;
	other.m_curAmount = 0;
}


void Deck::newDeck() noexcept {
	m_curAmount = s_maxSize;
	size_type count = 0;
	for (size_type i = (size_type)Symbol::club; i < (size_type)Symbol::null; i++)
		for (size_type j = 1; j <= 13; j++) {
			m_deck[count] = Card(j, (Symbol)i);
			count++;
		}
}
void Deck::shuffle() {
	random::randomize<Card>(m_deck, m_curAmount);
}

const Card Deck::takeTopCard() {
#if DEBUG
	if (isEmpty())
		assert("cant take card, deck empty");
#endif
	Card card(m_deck[m_curAmount - 1]);
	m_deck[m_curAmount - 1] = Card();
	m_curAmount--;

	return card;
}
const Card Deck::takeRandomCard() {
#if DEBUG
	if (isEmpty())
		assert("cant take card, m_deck empty");
#endif
	size_type randomNum = random::randInt(0, m_curAmount);
	Card card = m_deck[randomNum];

	for (size_type i = randomNum; i < m_curAmount - 1; i++)
		m_deck[i] = m_deck[i + 1];

	m_deck[m_curAmount - 1] = Card();
	m_curAmount--;

	return card;
}
void Deck::placeCardBottom(const Card &card) {
	if (isFull())
		assert("cant place card, m_deck full");

	for (signed_size_type i = m_curAmount; i >= 0; i--)
		m_deck[i] = m_deck[i - 1];

	m_deck[0] = card;
	m_curAmount++;
}
void Deck::placeCardRandom(const Card &card) {
#if DEBUG
	if (isFull())
		assert("cant place card, m_deck full");
#endif
	size_type randomNum = random::randInt(0, m_curAmount);

	for (signed_size_type i = m_curAmount; i >= randomNum; i--)
		m_deck[i] = m_deck[i - 1];

	m_deck[randomNum] = card;
	m_curAmount++;
}

Deck &Deck::operator=(const Deck &other) noexcept {
	if (this == &other)
		return *this;

	m_curAmount = other.m_curAmount;
	for (size_type i = 0; i < s_maxSize; i++)
		m_deck[i] = other.m_deck[i];

	return *this;
}
Deck &Deck::operator=(Deck &&other) noexcept {
	if (this == &other)
		return *this;

	m_deck = other.m_deck;
	m_curAmount = other.m_curAmount;

	other.m_deck = nullptr;
	other.m_curAmount = 0;

	return *this;
}
Deck &Deck::operator-=(size_type num) {
	if (!canTake(num))
		assert("cant throw this amount of cards, m_deck would be empty");
	for (size_type i = 0; i < num; i++)
		takeTopCard();
	m_curAmount -= num;

	return *this;
}
bool Deck::operator==(const Deck &other) const noexcept {
	if (this->m_curAmount != other.m_curAmount)
		return false;
	for (size_type i = 0; i < m_curAmount; i++)
		if (this->m_deck[i] != other.m_deck[i])
			return false;

	return true;
}

std::ostream &operator<<(std::ostream &output, const Deck &source) noexcept {
	unsigned int cardsInLine = 6; // how many cards fit in one line in the terminal
	output << "Deck: " << source.m_curAmount << " cards; contents:" << std::endl;
	
	for (unsigned int i = 0; i < source.m_curAmount; i++) {
		output << source.m_deck[i];
		if (cardsInLine != 0 && i != 0 && i % cardsInLine == 0)
			output << std::endl;
	}

	return output;
}


Deck::~Deck() noexcept {
	if (m_deck)
		delete[] m_deck;
}
