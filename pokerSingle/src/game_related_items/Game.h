
#ifndef GAME_H_
#define GAME_H_

#include "../../pch.h"
#include "../general/Flags.h"

#include "Player.h"
#include "../card_related_items/Table.h"
#include "PotsHandler.h"



class Game {
private: // 'using's and statics

	using p_ptr = std::shared_ptr<Player>; // shared pointer to a Player.
	using p_vec = std::vector<p_ptr>; // vector of shared pointers to a Player.
	using p_it = p_vec::iterator; // iterator for a vector of shared pointers to a Player.

	using size_type = p_vec::size_type;

	static const chips_t s_startingChips = 10000;
	static const chips_t s_roundBuyIn = s_startingChips / 200;

private: // private member variables

	std::ostream *m_output; // the outputed stream
	Table m_table; // the table being played

	unsigned int m_matches; // number of matches
	chips_t m_curBaseRaiseAmount; // the current base raise (minimum raise amount)
	chips_t m_curCallAmount; // the current call amount

	
	PotsHandler m_potHandler; // all the pots

	p_vec m_players; // vector of unique pointers to all the players
	size_type m_folded; // amount of folded players
	size_type m_broke; // the number of players who can't bet anymore;


private: // private methods

	const char *chooseBotName(size_type id);
	bool addBot(chips_t chips);
	bool addConsolePlayer(chips_t chips);
	p_it playerExit(p_it cit);

	p_it addCircular(p_it it);
	inline size_type playersLeft() { return m_players.size() - m_broke - m_folded; }

	std::ostream &showLatestDecision(const Player &player);
	bool doAction(p_it it);
	void finalAction(p_it it);
	void noAction(p_it it);
	bool brokePlayer(p_it it);
	p_it blindsAndCards(p_it firstBetter);
	bool stage(p_it firstBetter);
	void postStageActions();
	void resetDecisions();

	void preMatch();
	void postMatch(bool interrupted);
	void interruptedMatch();

	bool checkChips();
	void ultimateWinner();

	std::ostream &playersToStream(std::ostream &output) const;
	std::ostream &newPotToStream() const;

public: // public methods
	/**
	 * Game constructor, 
	 * 
	 * @param playerAmount: the amount of players that will be in the game, minimum 2, 
	 * maximum 10.
	 * @param onlyBots: input true if you want only bots to be includded in the game, 
	 * false if you want to have a console player to be included.
	 */
	Game(size_type playerAmount = 0, bool onlyBots = true);
	Game(const Game &) = delete;
	Game &operator=(const Game &) = delete;

	int start();

	friend std::ostream &operator<<(std::ostream &output, const Game &source);

	inline ~Game() noexcept {}
};


#endif // !GAME_H_
