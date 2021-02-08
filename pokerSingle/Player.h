
#ifndef PLAYER_H_
#define PLAYER_H_

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
	static int s_ids;

	int m_id;
	PlayerState m_state;
	Hand m_hand;

	int m_chips;
	int m_chipsTaken;

	int m_callAmount;
	int m_baseRaise;

	std::shared_ptr<HandAttempt> m_bestHand;

	const Table *m_table;

	void reset(int baseRaise, int callAmount);

	inline void resetDecision() { m_latestDecision.reset(); }
	inline void smallBlind() { m_latestDecision.newDecision(Action::call, m_callAmount /= 2); }
	inline void bigBlind() { m_latestDecision.newDecision(Action::call, m_callAmount); }

	bool allIn();
	bool raise();
	bool call();
	inline void fold() { m_state = PlayerState::folded; }

	int finalAction();
	inline void setTable(const Table *table) { m_table = table; }
	inline void drawHand(const Card &one, const Card &two) { m_hand.drawHand(one, two); }
	inline void setCallAmount(int callAmount) { m_callAmount = callAmount; }
	inline void addChips(int amount) { m_chips += amount; }
	inline bool hadFinalAction() { return m_state == PlayerState::broke; }
	bool takeChips(int amount);

	Action doAction();

protected:
	Decision m_latestDecision;
	std::string m_name;

public:
	Player();
	Player(const Player &other);
	Player(int chips, const char *m_name, const Table *table);

	inline int getId() const { return m_id; }
	inline bool isFolded() const { return m_state == PlayerState::folded; }
	inline bool isBroke() { return m_state == PlayerState::broke; }
	inline const Hand &getHand() { return m_hand; }
	inline int getChips() const { return m_chips; }
	inline int latestChipsTaken() const { return m_chipsTaken; }
	// the amount that the player has to get to in order to call at a certain turn
	inline int getCallAmount() const { return m_callAmount; }
	// both the original call amount and the amount that can be raised n times every turn
	inline int getBaseRaise() const { return m_baseRaise; }
	inline const HandAttempt &getBestHand() const { return *m_bestHand; }
	inline const Decision &getLatestDecision() const { return m_latestDecision; }
	inline Stage getCurrentStage() const { return m_table->currentStage(); }
	inline const char *getName() const { return m_name.c_str(); }
	inline size_t chipsTakenThisRound() const 
	{ return m_chipsTaken + m_latestDecision.getPreviousAmount(); }

	/**
	 * This method checks if this player can call the given amount (if the player's chips 
	 * are greater or equal to the given amount minus the amount this player has already 
	 * called this stage of the match. [chips] >= [amount] - [already called this stage].
	 */
	inline bool canCallAmount(int amount) const 
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
	static void sortPlayers(Player *players, int size);

	virtual ~Player();
};

#endif // !PLAYER_H_
