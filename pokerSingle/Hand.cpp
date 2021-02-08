
#include "Hand.h"


Hand::Hand() :m_one(), m_two() {
}
Hand::Hand(const Card &one, const Card &two) : m_one(one), m_two(two) {}
Hand::Hand(const Hand &other) : m_one(other.m_one), m_two(other.m_two) {}

void Hand::drawHand(const Card &one, const Card &two) {
#if DEBUG
	if (!m_one.isNull() || !m_two.isNull())
		throw std::logic_error("attempted to draw cards to an existing hand");
#endif
	m_one = one;
	m_two = two;
}
void Hand::clear() {
	m_one = Card();
	m_two = Card();
}

std::ostream &operator<<(std::ostream &output, const Hand &source) {
	/**
	 * TODO 
	 * 
	 * fatal error, always happens to the second card of the first player in the 
	 * m_players vector in Game class. the second card gets corrupt and isn't my 
	 * memory anymore, even though it's on the heap. gives the handler an access 
	 * violation error and values of the Card are garbage values
	 */
	output << source.m_one << source.m_two;
	return output;
}

Hand &Hand::operator=(const Hand &other) {
	if (this == &other)
		return *this;

	m_one = other.m_one;
	m_two = other.m_two;

	return *this;
}

Hand::~Hand() {
}
