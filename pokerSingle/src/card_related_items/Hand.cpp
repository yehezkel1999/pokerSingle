
#include "pch.h"
#include "Hand.h"
#include "../general/Flags.h"


Hand::Hand() noexcept
	: m_one(), m_two() {
}
Hand::Hand(const Card &one, const Card &two) noexcept
	: m_one(one), m_two(two) {}
Hand::Hand(const Hand &other) noexcept
	: m_one(other.m_one), m_two(other.m_two) {}

void Hand::drawHand(const Card &one, const Card &two) {
#if DEBUG
	if (!m_one.isNull() || !m_two.isNull())
		assert("attempted to draw cards to an existing hand");
#endif
	m_one = one;
	m_two = two;
}
void Hand::clear() noexcept {
	m_one = Card();
	m_two = Card();
}

Hand &Hand::operator=(const Hand &other) noexcept {
	if (this == &other)
		return *this;

	m_one = other.m_one;
	m_two = other.m_two;

	return *this;
}

std::ostream &operator<<(std::ostream &output, const Hand &source) noexcept {
	output << source.m_one << source.m_two;
	return output;
}
