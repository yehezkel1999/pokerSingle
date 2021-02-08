
#ifndef BOT_H_
#define BOT_H_

#include "Player.h"


/**
 * A struct that describes a raise, how many baseRaises have been added to the call amount
 * tuppled with the actual amount that will be raised.
 */
struct Raise {
	static int s_base; // this turn's base raise

	int m_times; // how many baiseRaises to add to the current call amount
	int m_amount; // the actual amount the bot raises to

	inline Raise(int times = 0) : m_times(times), m_amount(s_base + s_base * times) {}
	inline Raise(const Raise &other) : m_times(other.m_times), m_amount(other.m_amount) {}
	inline ~Raise() {}

	inline bool operator==(const Raise &other) const { return m_times == other.m_times; }
	inline bool operator!=(const Raise &other) const { return !(*this == other); }
	inline bool operator<(const Raise &other) const { return m_times < other.m_times; }
	inline bool operator>(const Raise &other) const { return other < *this; }
	inline bool operator<=(const Raise &other) const { return !(other < *this); }
	inline bool operator>=(const Raise &other) const { return !(*this < other); }

	inline operator bool() { return (bool)m_times; }
	inline bool operator!() { return !(bool)m_times; }

	inline Raise operator+(const Raise &other) const { return Raise(m_times + other.m_times); }
	Raise &operator=(const Raise &other);
};


class Bot : public Player {
	int m_turns;
	Raise m_standards;

	// @returns rais or call depending on who raised more
	const Decision &raiseCall(const Raise &me, const Raise &enemy);
	// @returns the amount of base raises this bot should go up to this round
	void setStandards();
	/**
	 * NOTE: this method is only relevant before there are open cards on the table.
	 * method for this bot, rates its hand (only cards heald by him). Gives each card a 
	 * value from 2 - 14 (Aces) and returns that number.
	 * 
	 * @returns the amount of max baseRaises he can go up to, depending on how good his 
	 * cards are.
	 */
	int rateHand();
	const Decision &preFlop();
public:
	static int s_call; // this turn's call amount
	
	Bot();
	Bot(const Bot &other);
	Bot(int chips, const char *name, const Table *table);

	virtual const Decision &doTurn();
	virtual const Decision &notEnoughChips();

	virtual Bot &Bot::operator=(const Bot &other);

	virtual ~Bot();
};


#endif // !BOT_H_