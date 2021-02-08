
#ifndef GAME_H_
#define GAME_H_

#define OUTPUT_FILE_NAME "log\\output.txt"

#include "Flags.h"
#include <vector>
#include <memory>
#include "Player.h"
#include "Table.h"
#include "PotsHandler.h"



class Game {
private: // 'using's and statics

	using p_ptr = std::shared_ptr<Player>; // shared pointer to a Player.
	using p_vec = std::vector<p_ptr>; // vector of shared pointers to a Player.
	using p_it = p_vec::iterator; // iterator for a vector of shared pointers to a Player.

	static const int s_startingChips = 10000;
	static const int s_roundBuyIn = s_startingChips / 200;

private: // private member variables

	std::ostream *m_output; // the outputed stream
	Table m_table; // the table being played

	int m_matches; // number of matches
	int m_curBaseRaiseAmount; // the current base raise (minimum raise amount)
	int m_curCallAmount; // the current call amount

	
	PotsHandler m_potHandler; // all the pots

	p_vec m_players; // vector of unique pointers to all the players
	int m_folded; // amount of folded players
	int m_broke; // the number of players who can't bet anymore;


private: // private methods

	const char *chooseBotName(int id);
	bool addBot(int chips);
	bool addConsolePlayer(int chips);
	p_it playerExit(p_it cit);

	p_it addCircular(p_it it);
	inline size_t playersLeft() { return m_players.size() - m_broke - m_folded; }

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

	Game(int playerAmount = 0, bool console = true);
	Game(const Game &) = delete;
	Game &operator=(const Game &) = delete;

	int start();

	friend std::ostream &operator<<(std::ostream &output, const Game &source);

	~Game();
};


#endif // !GAME_H_
