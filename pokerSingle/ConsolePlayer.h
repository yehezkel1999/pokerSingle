
#ifndef CONSOLE_PLAYER_H_
#define CONSOLE_PLAYER_H_

#include "Player.h"


class ConsolePlayer : public Player {
public:
	ConsolePlayer();
	ConsolePlayer(const ConsolePlayer &other);
	ConsolePlayer(chips_t chips, const Table *table);

	virtual const Decision &doTurn();
	virtual const Decision &notEnoughChips();

	inline virtual ~ConsolePlayer() noexcept {}
};

#endif // !CONSOLE_PLAYER_H_
