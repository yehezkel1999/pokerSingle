
#include "Card.h"

#include "CustomExceptions.h"

Card::Card() noexcept
	: m_value(0), m_symbol(Symbol::null) {}
Card::Card(const Card &other) noexcept
	: m_value(other.m_value), m_symbol(other.m_symbol) {}
Card::Card(int value, Symbol symbol) {
#if DEBUG
	if (value < 1)
		throw invalid_card_value("card value too low");
	if (value > 13)
		throw invalid_card_value("card value too high");
#endif
	m_value = value;
	m_symbol = symbol;
}

bool Card::sameColour(const Card &other) const {
#if DEBUG
	if (isNull() || other.isNull())
		throw nullptr_exception("cannot call method sameColour on a nulled card");
#endif
	return (m_symbol == Symbol::club && other.m_symbol == Symbol::spade) ||
		(m_symbol == Symbol::heart && other.m_symbol == Symbol::diamond);
}
bool Card::sameSymbol(const Card &other) const {
#if DEBUG
	if (isNull() || other.isNull())
		throw nullptr_exception("cannot call method sameSymbol on a nulled card");
#endif
	return m_symbol == other.m_symbol;
}
bool Card::sameValue(const Card &other) const {
#if DEBUG
	if (isNull() || other.isNull())
		throw nullptr_exception("cannot call method sameValue on a nulled card");
#endif
	return m_value == other.m_value;
}
const char *Card::valueToString() const {
#if DEBUG
	if (isNull())
		throw nullptr_exception("cannot call method valueToString on a nulled card");
#endif
	switch (m_value) {
	case 1:
		return "Ace";
	case 2:
		return "2";
	case 3:
		return "3";
	case 4:
		return "4";
	case 5:
		return "5";
	case 6:
		return "6";
	case 7:
		return "7";
	case 8:
		return "8";
	case 9:
		return "9";
	case 10:
		return "10";
	case 11:
		return "Jack";
	case 12:
		return "Queen";
	case 13:
		return "King";
	}
#if DEBUG
	throw nullptr_exception("card is null");
#endif
}
const char *Card::symbolToString() const {
#if DEBUG
	if (isNull())
		throw nullptr_exception("cannot call method symbolToString on a nulled card");
#endif
	switch (m_symbol) {
	case Symbol::club:
		return "club";
	case Symbol::heart:
		return "heart";
	case Symbol::spade:
		return "spade";
	case Symbol::diamond:
		return "diamond";
	}
#if DEBUG
	throw nullptr_exception("card is null");
#endif
}

int Card::operator+(const Card &other) const {
#if DEBUG
	if (isNull())
		throw nullptr_exception("cannot call operator + on a nulled card");
	if (other.isNull())
		throw nullptr_exception("cannot add the value of a nulled card");
#endif
	return m_value + other.m_value;
}
int Card::operator-(const Card &other) const {
#if DEBUG
	if (isNull())
		throw nullptr_exception("cannot call operator - on a nulled card");
	if (other.isNull())
		throw nullptr_exception("cannot substract the value of a nulled card");
#endif
	return m_value - other.m_value;
}
int Card::operator+(int num) const {
#if DEBUG
	if (isNull())
		throw nullptr_exception("cannot call operator + on a nulled card");
#endif
	return m_value + num;
}
int Card::operator-(int num) const {
#if DEBUG
	if (isNull())
		throw nullptr_exception("cannot call operator - on a nulled card");
#endif
	return m_value - num;
}

Card &Card::operator=(const Card &other) noexcept {
	if (this == &other)
		return *this;

	m_value = other.m_value;
	m_symbol = other.m_symbol;

	return *this;
}
bool Card::operator==(const Card &other) const {
#if DEBUG
	if (isNull())
		throw nullptr_exception("cannot call operator == on a nulled card");
	if (other.isNull())
		throw nullptr_exception("cannot use operator == on a nulled card");
#endif
	return m_value == other.m_value && m_symbol == other.m_symbol;
}
bool Card::operator<(const Card &other) const {
#if DEBUG
	if (isNull())
		throw nullptr_exception("cannot call operator < on a nulled card");
	if (other.isNull())
		throw nullptr_exception("cannot use operator < on a nulled card");
#endif
	if (m_value == other.m_value)
		return false;
	return m_value == 1 ? false : other.m_value == 1 ? true : m_value < other.m_value;
}

std::ostream &operator<<(std::ostream &output, const Card &source) noexcept {
#if DEBUG
	if (source.isNull())
		output << "nullptr";
	else
#endif
		output << '(' << source.valueToString() << ',' << source.symbolToString() << ')';
	return output;
}

bool Card::isSubsequent(const Card &one, const Card &two) noexcept {
	int first = one.m_value, second = two.m_value;
	if (first == 1 && second == 13)
		first = getOpposingValue(first);
	if (first == 13 && second == 1)
		second = getOpposingValue(second);

	return first + 1 == second || second + 1 == first;
}
