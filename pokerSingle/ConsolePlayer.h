
#ifndef CONSOLE_PLAYER_H_
#define CONSOLE_PLAYER_H_

#include "Player.h"


class ConsolePlayer : public Player {
public:
	ConsolePlayer();
	ConsolePlayer(const ConsolePlayer &other);
	ConsolePlayer(int chips, const Table *table);

	virtual const Decision &doTurn();
	virtual const Decision &notEnoughChips();

	virtual ~ConsolePlayer();
};

#endif // !CONSOLE_PLAYER_H_
