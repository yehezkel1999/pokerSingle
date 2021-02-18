
#include "Decision.h"
#include "../tools/Functions.h"

Decision::Decision() noexcept
	: m_action(Action::fold), m_amount(0), m_previousAmount(0)
#if DEBUG
	, m_made(false)
#endif
{}
Decision::Decision(const Decision &other) noexcept
	: m_action(other.m_action), m_amount(other.m_amount),
	m_previousAmount(other.m_previousAmount) 
#if DEBUG
	, m_made(other.m_made)
#endif
{}

void Decision::reset() noexcept {
	m_action = Action::call; 
	m_amount = 0; 
	m_previousAmount = 0; 
#if DEBUG
	m_made = false;
#endif
}

const char *Decision::actionToString() const noexcept {
	switch (m_action) {
	case Action::fold:
		return "folded";
	case Action::call:
		return "called";
	case Action::raise:
		return "raised to";
	case Action::allIn:
		return "went ALL IN";
	}
	return nullptr;
}

chips_t Decision::difference() const {
#if DEBUG
	if (m_action != Action::fold && m_amount < m_previousAmount)
		throw std::logic_error("current decision amount cannot be smaller than previous amount");
#endif
	return m_amount - m_previousAmount;
}

const Decision &Decision::newDecision(Action action, chips_t amount) {
	m_action = action;

	m_previousAmount = m_amount;
	m_amount = amount;

#if DEBUG
	if (m_made)
		throw std::logic_error("cannot make a new decision more than once per turn");
	else
		m_made = true;
#endif

	return *this;
}

Decision &Decision::operator=(const Decision &other) noexcept {
	if (this == &other)
		return *this;

	m_action = other.m_action;
	m_amount = other.m_amount;
	m_previousAmount = other.m_previousAmount;
#if DEBUG
	m_made = other.m_made;
#endif

	return *this;
}
std::ostream &operator<<(std::ostream &output, const Decision &source) noexcept {
	if (source.m_action == Action::call && !source.difference())
		return output << "checked";
	output << source.actionToString();
	if (source.m_action == Action::raise) {
		output << ' ';
		func::commas(output, source.m_amount) << '$';
	}
	if (source.m_action == Action::allIn || source.m_action == Action::call){
		output << " (";
		func::commas(output, source.m_amount) << "$)";
	}
	return output;
}
