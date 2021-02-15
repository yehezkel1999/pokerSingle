
#include "Decision.h"
#include "../tools/Functions.h"

Decision::Decision() noexcept
	: m_action(Action::fold), m_amount(0), m_previousAmount(0) {}
Decision::Decision(const Decision &other) noexcept
	: m_action(other.m_action), m_amount(other.m_amount),
	m_previousAmount(other.m_previousAmount) {}

const Decision &Decision::newDecision(Action action, chips_t amount) noexcept {
	m_action = action;

	m_previousAmount = m_amount;
	m_amount = amount;

	return *this;
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
Decision &Decision::operator=(const Decision &other) noexcept {
	if (this == &other)
		return *this;

	m_action = other.m_action;
	m_amount = other.m_amount;
	m_previousAmount = other.m_previousAmount;

	return *this;
}
std::ostream &operator<<(std::ostream &output, const Decision &source) noexcept {
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