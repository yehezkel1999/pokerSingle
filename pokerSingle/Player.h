
#ifndef PLAYER_H_
#define PLAYER_H_

#include "Flags.h"

#include "Hand.h"
#include "HandAttempt.h"
#include "Decision.h"
#include "Table.h"
#include <string>
#include <memory>

enum class PlayerState {
	playing, folded, broke
};

class Player {
	static id_t s_ids;

	id_t m_id;
	PlayerState m_state;
	Hand m_hand;

	chips_t m_chips;
	chips_t m_chipsTaken;

	chips_t m_callAmount;
	chips_t m_baseRaise;

	std::shared_ptr<HandAttempt> m_bestHand;

	const Table *m_table;

	void reset(chips_t baseRaise, chips_t callAmount);

	inline void resetDecision() noexcept { m_latestDecision.reset(); }
	inline void smallBlind() noexcept 
	{ m_latestDecision.newDecision(Action::call, m_callAmount /= 2); }
	inline void bigBlind() noexcept 
	{ m_latestDecision.newDecision(Action::call, m_callAmount); }

	void allIn();
	void raise();
	void call();
	inline void fold() noexcept { m_state = PlayerState::folded; }

	chips_t finalAction();
	inline void setTable(const Table *table) noexcept { m_table = table; }
	inline void drawHand(const Card &one, const Card &two) noexcept 
	{ m_hand.drawHand(one, two); }
	inline void setCallAmount(chips_t callAmount) noexcept { m_callAmount = callAmount; }
	inline void addChips(chips_t amount) noexcept { m_chips += amount; }
	inline bool hadFinalAction() noexcept { return m_state == PlayerState::broke; }
	void takeChips(chips_t amount, bool allIn = false);

	Action doAction();

protected:
	Decision m_latestDecision;
	std::string m_name;

public:
	Player();
	Player(const Player &other);
	Player(chips_t chips, const char *m_name, const Table *table);

	inline id_t getId() const { return m_id; }
	inline bool isFolded() const { return m_state == PlayerState::folded; }
	inline bool isBroke() { return m_state == PlayerState::broke; }
	inline const Hand &getHand() { return m_hand; }
	inline chips_t getChips() const { return m_chips; }
	inline chips_t latestChipsTaken() const { return m_chipsTaken; }
	// the amount that the player has to get to in order to call at a certain turn
	inline chips_t getCallAmount() const { return m_callAmount; }
	// both the original call amount and the amount that can be raised n times every turn
	inline chips_t getBaseRaise() const { return m_baseRaise; }
	inline const HandAttempt &getBestHand() const { return *m_bestHand; }
	inline const Decision &getLatestDecision() const { return m_latestDecision; }
	inline Stage getCurrentStage() const { return m_table->currentStage(); }
	inline const char *getName() const { return m_name.c_str(); }
	inline chips_t chipsTakenThisRound() const 
	{ return m_chipsTaken + m_latestDecision.getPreviousAmount(); }

	/**
	 * This method checks if this player can call the given amount (if the player's chips 
	 * are greater or equal to the given amount minus the amount this player has already 
	 * called this stage of the match. [chips] >= [amount] - [already called this stage].
	 */
	inline bool canCallAmount(chips_t amount) const
	{ return m_chips >= amount - m_latestDecision.getAmount(); }

	/**
	 * callAmount is the amount you need to get to in order 
	 */
	virtual const Decision &doTurn() = 0;
	/**
	 * This method is called when the player doesn't have enough chips to call the current 
	 * amount, but only if his chips are greater than 0, after this he won't be eligable to 
	 * any more chips than he already called.
	 * In this method a player can ONLY decide to either call all his chips or fold.
	 */
	virtual const Decision &notEnoughChips() = 0;

	void checkHand();

	virtual Player &Player::operator=(const Player &other);

	friend class Pot;
	friend class Game;
	friend std::ostream &operator<<(std::ostream &output, const Player &source);

	/**
	 * Static function for Player class, sorts the given sequence of players by their best hand frow
	 * low to high in a time complexety of (n^2+n)/2 (does not consider card symbols).
	 *
	 * @param cards: a pointer to the array of players to sort.
	 * @param size: the size of the array.
	 * @exceptsafe This function does not throw exceptions.
	 */
	static void sortPlayers(Player *players, unsigned int size);

	inline virtual ~Player() noexcept {}
};

#endif // !PLAYER_H_
