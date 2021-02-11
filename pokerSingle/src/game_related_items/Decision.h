
#ifndef DECISION_H_
#define DECISION_H_

#include "../Flags.h"
#include <iostream>

/**
 * Implamentation of a decision's Action enum.
 *
 * This enum represents the actions a player can decide to do. The actual value of the enum
 * is irrelevant, only the enum itself. Each of this enum's value is an action a player can
 * decide to do in a poker game: fold, call, raise and go all in.
 */
enum class Action {
	fold, call, raise, allIn
};


/**
 * Implamentation of a Decision type.
 *
 * A Decision type represents the decision a player makes every turn. The action is then
 * executed with its associated amount. A Decision type consists of three members: the
 * acion the player decides to do, the amount of chips associated with this action (0 if
 * folded, or the call, raise or all in amount) and the previous amount of a certain object.
 */
class Decision {
	Action m_action; // the action the player decides to do.
	chips_t m_amount; // The amount this player will add to the pot, this turn.
	chips_t m_previousAmount; // The previous amount.

	const char *actionToString() const noexcept;
	// resets action to call and both amounts to 0.
	inline void reset() noexcept { m_action = Action::call; m_amount = 0; m_previousAmount = 0; }
	// changes the decision manually (without changing previous amount)
	inline void changeDecision(Action action) noexcept { m_action = action; }
	// changes the decision manually (without changing previous amount)
	inline void changeDecision(Action action, chips_t amount) noexcept
	{ m_action = action; m_amount = amount; }
public:
	/**
	 * Defualt constructor for Decision class, sets both amounts to 0 and action to fold.
	 */
	Decision() noexcept;
	/**
	 * Copy constructor for Decision class, simply copies member values from the other
	 * decision.
	 *
	 * @param other: the Decision that is being copied.
	 */
	Decision(const Decision &other) noexcept;

	/**
	 * Inline method for this decision, gets the action that was decided.
	 *
	 * @return the action that was decided.
	 */
	inline Action getAction() const noexcept { return m_action; }
	/**
	 * Inline method for this decision, gets the amount that was decided. If the action
	 * that was decided was to fold then this has no meaning.
	 *
	 * @return the amount that was decided.
	 */
	inline chips_t getAmount() const noexcept { return m_amount; }
	/**
	 * Inline method for this decision, gets the previous amount that was decided. If the
	 * action that was decided was to fold then this has no meaning.
	 *
	 * @return the previous amount that was decided.
	 */
	inline chips_t getPreviousAmount() const noexcept { return m_previousAmount; }
	/**
	 * when making a new decision, the amount parameter isn't used for Action::fold.
	 * When calling, call this function with the call-> (Action::call, callAmount), the
	 * actual amount that will be taken is the call amount minus the amount of your latest
	 * decision: chipsTaken = callAmount - latestDecision.getAmount().
	 * When raising, call this function with the amount you would like to raise TO, not the
	 * amount you want to raise BY. For example, if the call amount is 600 and you want to
	 * raise to 800 write (Action::raise, 800).
	 * When going all in, simply put in all your chips.
	 *
	 * @param action: the action you want to do this turn (fold, call, raise, all in).
	 * @param amount: the amount of chips adjacent to your decision.
	 * @returns the decision itself.
	 */
	const Decision &newDecision(Action action, chips_t amount = 0) noexcept;

	/**
	 * Assignment operator for Decision class, simply copies member values from the other
	 * decision.
	 *
	 * @param other: the Decision that is being copied.
	 */
	Decision &Decision::operator=(const Decision &other) noexcept;

	// player needs to use private methods since it needs to change decisions artificially 
	// for security.
	friend class Player;

	/**
	 * Overloaded operator << of std::ostream. Allows std::ostream to accept a Decision
	 * type to its << operator. In action, allows you to input a decision to an output
	 * stream in this fasion: std::ostream << Decision. The decision outputs like so:
	 * action + ' ' + amount + '$', called (800$), raised 500$.
	 *
	 * @param output: the stream that the card is outputted to.
	 * @param source: the decision that is outputted to the stream.
	 * @return the output stream that is recieved by the operator.
	 */
	friend std::ostream &operator<<(std::ostream &output, const Decision &source) noexcept;

	inline ~Decision() noexcept {}
};


#endif // !DECISION_H_
