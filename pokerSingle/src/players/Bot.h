
#ifndef BOT_H_
#define BOT_H_

#include "../game_related_items/Player.h"


/**
 * A struct that describes a raise, how many baseRaises have been added to the call amount
 * tuppled with the actual amount that will be raised.
 */
struct Raise {
	static chips_t s_base; // this turn's base raise

	chips_t m_times; // how many baiseRaises to add to the current call amount
	chips_t m_amount; // the actual amount the bot raises to

	inline Raise(int times = 0) : m_times(times), m_amount(s_base + s_base * times) {}
	inline Raise(const Raise &other) : m_times(other.m_times), m_amount(other.m_amount) {}
	inline ~Raise() noexcept {}

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
	unsigned int m_turns;
	Raise m_standards;

	bool m_gender;

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
	static chips_t s_call; // this turn's call amount
	
	Bot(const Bot &other);
	Bot(chips_t chips, const char *name, bool gender, const Table *table);

	virtual const Decision &doTurn();
	virtual const Decision &notEnoughChips();
	virtual const char *possessiveAdjective();

	virtual Bot &Bot::operator=(const Bot &other);

	inline virtual ~Bot() noexcept {}
};


#endif // !BOT_H_