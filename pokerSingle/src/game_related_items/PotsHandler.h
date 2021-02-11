
#ifndef POTS_HANDLER_H_
#define POTS_HANDLER_H_

#include "../tools/sorted_vector.h"
#include "Pot.h"

/****************************************************************************************
 * 
 * Pots Handler Thinking Process:
 * 
 * Pots' lock:
 *     throughout the game only the latest pot in the list is open, the rest are locked, 
 *     when all the eligible players in that pot contribute the base amount then that pot 
 *     is finished.
 * 
 * Pots' base:
 *     unless a player can't call, a pot's base only changes every round, meaning when 
 *     betting stops and only calls are made. When the last call is made the base will be 
 *     that call amount.
 * 
 * Player cannot call:
 *     1. change the current pot's base to the max amount this player can call plus the old 
 *        of this pot: [old base] + [max player call] and lock it there.
 *     2. leave in that pot: the [base] * [amount of players in pot] (including the player who 
 *        can't call) and save the rest of the chips in the pot: [total] - [base] * [players].
 *     3. add a new pot with the saved value in it and add all the players from the 
 *        previous pot to it.
 * 
 * Player can call/raise:
 *     go through all the pots (from beginning to end) and check if the amount he has 
 *     contributed to the pot is less then the base, if it is then the player needs to 
 *     add to that pot the remaining amount: [base] - [already contributed].
 *     In the instance when the player can't call anymore the pot where he can't call to 
 *     gets split into two. If the pot that was split locked then the new pot will be 
 *     locked aswell. The split will follow the rules of Player cannot call.
 * 
 * Winner:
 *     each and every pot has its own winner (or winners).
 * 
 ****************************************************************************************/


class PotsHandler {
public: // 'using's

	using pot_vec = sorted_vector<Pot>; // sorted vector of type Pot
	using pot_it = pot_vec::iterator; // iterator for a sorted vector of type Pot

	using p_ptr = Pot::p_ptr; // shared pointer to a Player.
	using p_vec = Pot::p_vec; // vector of shared pointers to a Player.
	using p_it = Pot::p_it; // iterator for a vector of shared pointers to a Player.

	using c_vec = Pot::c_vec;	// vector of Contributers.
	using c_it = Pot::c_it; // iterator for a vector of Contributers.

private: // member variables
	
	std::ostream *m_output;
	pot_vec m_pots;
	const Pot *m_latest;
	p_ptr m_reason; // the player the last pot was opened because

	/**
	 * opens a new pot depending on a recently locked pot.
	 *
	 * @param it: an iterator to the pot that the new pot "copy".
	 * @param locker: the player who can't call anymore.
	 * @param state: the state of the previous pot to be transffered to the new pot.
	 * @param remainder: the remainder from the previous pot to be transffered to the new 
	 * pot.
	 */
	pot_it openNewPot(pot_it it, p_ptr locker, PotState state, chips_t remainder);
	 /**
	  * opens a new pot depending on the last pot in m_pots with a staring contributer.
	  *
	  * @param player: the player who will be added to the new pot.
	  * @param leftOver: the amount the player has left and will be contributed to the new 
	  * pot.
	  */
	pot_it openNewPot(p_ptr player, chips_t leftOver);
	/**
	 * locks given pot, adds locker to pot if he is not there already, contributes given
	 * amount to the pot and adds another pot if necessary. If the player went all in then 
	 * a new pot doesn't need to be opened right now, but only if and when someone raises.
	 *
	 * @param it: an iterator to the pot that needs to be locked.
	 * @param locker: the player who can't call anymore.
	 * @param amount: the amount he has left.
	 * @param allIn: a bool indicating whether the player went all in or not.
	 */
	bool lockPot(pot_it it, p_ptr locker, chips_t amount, bool allIn = false);
public:

	PotsHandler();
	// need the players vector to actually construct
	void create(std::ostream *output, const p_vec &players);
	PotsHandler(const PotsHandler &) = delete;
	PotsHandler &operator=(const PotsHandler &) = delete;

	inline const Pot &latestPotAdded() const { return *m_latest; }
	inline const Player &latestReason() const { return *m_reason; }

	void reset(const p_vec &players);
	void postStage();

	bool addToPots(p_ptr player, chips_t amount, bool cantCall = false);
	void calcWinners(bool oneLeft);

	friend std::ostream &operator<<(std::ostream &output, const PotsHandler &source);

	inline ~PotsHandler() noexcept {}
};


#endif // !POTS_HANDLER_H_