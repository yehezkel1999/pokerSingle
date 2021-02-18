
#ifndef POT_H_
#define POT_H_

#include <vector>
#include <memory>
#include "Player.h"

enum class PotState { 
	start, open, locked, finished 
};

/**
 * A struct of a player and how many chips he has contributed to this pot
 */
struct Contributer {
public:
	using p_ptr = std::shared_ptr<Player>; // shared pointer to a Player.
public:
	p_ptr _player; // the player
	chips_t _amount; // the amount this player has contributed to this pot.
	chips_t _remainder; // the remainder from when the pot is locked

	Contributer(p_ptr player, chips_t amount = 0) : _player(player), _amount(amount), _remainder(0) {}

	// changes this player's amount to the given base as he contributed the needed amount 
	// and returns the amount he contributed.
	// @param base: the amount this player needs to contribute to this pot.
	// @returns how much this player needs to add to this pot.
	inline chips_t contribute(chips_t base) { chips_t rv = base - _amount; _amount = base; return rv; }

	// @param newBase: the new base of this pot.
	// @returns how much chips from this player needs to go to the new pot.
	chips_t baseChange(chips_t newBase);
};

class Pot {
public: // 'using's

	using p_ptr = Contributer::p_ptr; // shared pointer to a Player.
	using p_vec = std::vector<p_ptr>; // vector of shared pointers to a Player.
	using p_it = p_vec::iterator; // iterator for a vector of shared pointers to a Player.
	using c_vec = std::vector<Contributer>;	// vector of Contributers.
	using c_it = c_vec::iterator; // iterator for a vector of type Contributer.
	using cc_it = c_vec::const_iterator; // iterator for a vector of type const Contributer.

private: // member variables
	static id_t s_ids;

	id_t m_id;

	chips_t m_base;
	PotState m_state; // indicates whether the baseAmount can be raised or not
	p_ptr m_reason; // smart pointer to the player who is the reason for this pot's existance.

	chips_t m_amount;
	c_vec m_eligible;

	/**
	 * Locks the pot completely, marking it fininshed. no actions on this pot will take place
	 * after this method is called.
	 */
	inline void finished() { m_state = PotState::finished; }
	void reset();
	// call this at the end of every stage for an open pot
	chips_t setBase();

	/**
	 * checks if the given player has already contributed to the pot, if he hasn't, the 
	 * player is added to the pot.
	 * iterator.
	 *
	 * @param player: the player that is checked.
	 * @returns an iterator to that player in the pot
	 */
	c_it getOrPush(p_ptr player);

	/**
	 * adds the given amount to the pot.
	 *
	 * @returns 0 if no new pot is needed, otherwise it retruns the amount that is left
	 * over and is needed to be added to the next pot. When this pot needs to split it
	 * returns -1.
	 */
	schips_t addToPot(p_ptr adder, chips_t amount);
	/**
	 * adds the given amount to the pot WITHOUT checking anything. use this only when
	 * splitting.
	 */
	void noCheckAdd(p_ptr adder, chips_t amount);
	/**
	 * Locks the pot. The pot's base will be the old base plus the locking player already
	 * contributed amount plus the inputted amount.
	 *
	 * @param locker: the player who locked the pot.
	 * @param amount: the amount the player has left and will be added to the player's
	 * contributed amount, which that amount will be added to the base.
	 * @returns the remaining amount from each player in the pot that exceeds the base.
	 */
	chips_t lockPot(p_ptr locker, chips_t amount);

	void calcWinner(std::ostream &output);
	bool oneNonFoldedLeft() const;
	Player &oneLeft();
	chips_t winnerCount(const Player &winner);
	bool moreThanOneWinner(std::ostream &output, const Player &winner);
	void declareWinner(std::ostream &output, Player &winner, bool oneNonFolded = false);
	std::ostream &potDeclareName(std::ostream &output) const;
public:

	Pot(p_vec::size_type capacity = 0);
	/**
	 * Constructor for this pot, Copies the eligible players from the other pot except the
	 * given player, as he is the reason another pot is needed. This constructor also gives \
	 * the pot its starting amount.
	 *
	 * @param other: the Pot that is being "copied".
	 * @param player: the player that won't be in this pot.
	 * @param state: the pot's state.
	 * @param amount: the pot's starting amount.
	 */
	Pot(const Pot &other, p_ptr player, PotState state, chips_t amount);
	/**
	 * Constructor for this pot, constructs a pot with only one player, copies the inputted 
	 * pot's capacity minus one.
	 *
	 * @param other: the last active pot.
	 * @param player: the player that won't be in this pot.
	 * @param amount: the amount this player contributed to this pot.
	 * @param reason: the player that because of him this pot was opened.
	 */
	Pot(const Pot &other, p_ptr player, chips_t amount, p_ptr reason);

	Pot(Pot &&other) noexcept;
	Pot &operator=(Pot &&other) noexcept;

	Pot(const Pot &) = delete;
	Pot &operator=(const Pot &) = delete;

	inline PotState getState() { return m_state; }
	// @returns true if the pot is open for new betting, false otherwise.
	inline bool isOpen() const { return isOpen(m_state); }
	inline bool isFinished() const { return m_state == PotState::finished; }
	inline chips_t getAmount() const { return m_amount; }
	inline chips_t getSize() const { return m_eligible.size(); }
	inline const Player &getReason() const { return *m_reason; }
	bool isEligible(const Contributer &contributer) const;

	const char *stateToString() const;

	/**
	 * checks if the given player has already contributed to the pot and returns its
	 * iterator.
	 *
	 * @param player: the player that is checked.
	 * @returns an iterator to that player in the pot, if the player did not contribute yet
	 * then end() iterator is returned.
	 */
	c_it exists(p_ptr player);
	// @returns a const iterator to the first non folded player that contributed to the pot.
	// if there is non returns end().
	cc_it firstNonFolded() const;
	chips_t nonFolded() const;

	/**
	 * Operator for this pot, checks if the other pot has the same values. For the pots
	 * to be the same the pot's eligible players vector's lengths have to be the same.
	 *
	 * @param other: the Pot that is being compared.
	 * @return true if the other pot is the same, false if it isn't.
	 */
	inline bool operator==(const Pot &other) const 
	{ return m_eligible.size() == other.m_eligible.size(); }
	/**
	 * Operator for this pot, checks if the other pot has different values. For the pots
	 * to be the different the pot's eligible players vector's lengths have to be different.
	 *
	 * @param other: the Pot that is being compared.
	 * @return true if the other pot is different, false if it isn't.
	 */
	inline bool operator!=(const Pot &other) const { return !operator==(other); }
	/**
	 * Operator for this pot, checks if the other pot has lesser values than the inputted
	 * pot. For the pot to have lesser values the pot's eligible players vector's length
	 * has to be larger.
	 *
	 * @param other: the Pot that is being compared.
	 * @return true if the this pot is smaller than the other pot, false if it isn't.
	 */
	bool operator<(const Pot &other) const;
	/**
	 * Operator for this pot, checks if the other pot has greater values than the inputted 
	 * pot. For the pot to have greater values the pot's eligible players vector's length 
	 * has to be smaller.
	 *
	 * @param other: the Pot that is being compared.
	 * @return true if the this pot is greater than the other pot, false if it isn't.
	 */
	inline bool operator>(const Pot &other) const { return other < *this; }
	/**
	 * Operator for this pot, checks if the other pot has lesser values than the inputted
	 * pot. For the pot to have lesser values the pot's eligible players vector's length
	 * has to be larger or equal.
	 *
	 * @param other: the Pot that is being compared.
	 * @return true if the this pot is smaller or equal to the other pot, false if it isn't.
	 */
	inline bool operator<=(const Pot &other) const { return !(other < *this); }
	/**
	 * Operator for this pot, checks if the other pot has greater values than the inputted
	 * pot. For the pot to have greater values the pot's eligible players vector's length
	 * has to be smaller or equal.
	 *
	 * @param other: the Pot that is being compared.
	 * @return true if the this pot is greater or equal to the other pot, false if it isn't.
	 */
	inline bool operator>=(const Pot &other) const { return !(*this < other); }

	friend class PotsHandler;
	friend std::ostream &operator<<(std::ostream &output, const Pot &source);
	inline static bool isOpen(PotState state) { return state == PotState::start || state == PotState::open; }

	inline ~Pot() noexcept {}
};


#endif // !POT_H_