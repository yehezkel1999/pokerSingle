
#include "Deck.h"
#include "CustomExceptions.h"
#include "random.h"

#include "Flags.h"


Deck::Deck() 
	: m_deck(new Card[MAX_SIZE]), m_curAmount(MAX_SIZE) {
	newDeck();
}
Deck::Deck(const Deck &other) 
	: m_deck(new Card[MAX_SIZE]), m_curAmount(other.m_curAmount) {
	for (int i = 0; i < MAX_SIZE; i++)
		m_deck[i] = other.m_deck[i];
}


void Deck::newDeck() {
	m_curAmount = MAX_SIZE;
	int count = 0;
	for (int i = (int)Symbol::club; i < (int)Symbol::null; i++)
		for (int j = 1; j <= 13; j++) {
			m_deck[count] = Card(j, (Symbol)i);
			count++;
		}
}
void Deck::shuffle() {
	random::randomize<Card>(m_deck, m_curAmount);
}

const Card Deck::takeTopCard() {
	if (isEmpty())
		throw std::out_of_range("cant take card, deck empty");

	Card card(m_deck[m_curAmount - 1]);
	m_deck[m_curAmount - 1] = Card();
	m_curAmount--;

	return card;
}
const Card Deck::takeRandomCard() {
	if (isEmpty())
		throw std::out_of_range("cant take card, m_deck empty");

	int randomNum = random::randInt(0, m_curAmount);
	Card card = m_deck[randomNum];

	for (int i = randomNum; i < m_curAmount - 1; i++)
		m_deck[i] = m_deck[i + 1];

	m_deck[m_curAmount - 1] = Card();
	m_curAmount--;

	return card;
}
void Deck::placeCardBottom(const Card &card) {
	if (isFull())
		throw std::out_of_range("cant place card, m_deck full");

	for (int i = m_curAmount; i >= 0; i--)
		m_deck[i] = m_deck[i - 1];

	m_deck[0] = card;
	m_curAmount++;
}
void Deck::placeCardRandom(const Card &card) {
	if (isFull())
		throw std::out_of_range("cant place card, m_deck full");

	int randomNum = random::randInt(0, m_curAmount);

	for (int i = m_curAmount; i >= randomNum; i--)
		m_deck[i] = m_deck[i - 1];

	m_deck[randomNum] = card;
	m_curAmount++;
}

Deck &Deck::operator=(const Deck &other) {
	if (this == &other)
		return *this;

	m_curAmount = other.m_curAmount;
	for (int i = 0; i < MAX_SIZE; i++)
		m_deck[i] = other.m_deck[i];

	return *this;
}
Deck &Deck::operator-=(int num) {
	if (!canTake(num))
		throw std::out_of_range("cant throw this amount of cards, m_deck would be empty");
	for (int i = 0; i < num; i++)
		takeTopCard();
	m_curAmount -= num;

	return *this;
}
bool Deck::operator==(const Deck &other) const {
	if (this->m_curAmount != other.m_curAmount)
		return false;
	for (int i = 0; i < m_curAmount; i++)
		if (this->m_deck[i] != other.m_deck[i])
			return false;

	return true;
}

std::ostream &operator<<(std::ostream &output, const Deck &source) {
	int cardsInLine = 6; // how many cards fit in one line in the terminal
	output << "Deck: " << source.m_curAmount << " cards; contents:" << std::endl;
	
	for (int i = 0; i < source.m_curAmount; i++) {
		output << source.m_deck[i];
		if (cardsInLine != 0 && i != 0 && i % cardsInLine == 0)
			output << std::endl;
	}

	return output;
}


Deck::~Deck() {
	if (m_deck)
		delete[] m_deck;
}
